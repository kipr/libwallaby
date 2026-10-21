/*
 * Interactive libwallaby API exerciser for KIPR IDE / botui.
 *
 * Change the port constants below to match the attached hardware before
 * running. Motor 0 runs in both directions, then all four motor ports run
 * together; clear the mechanisms and raise the wheels first. Servo 0 is
 * moved to its midpoint.
 *
 * Controls at every review: B = correct/continue, A = incorrect/show debug,
 * C = stop immediately. C-button click/wait APIs run together at the final
 * exit check, while the main thread still polls c_button() for fast exit.
 */

#include <kipr/wombat.h>
#include <kipr/wait_for/wait_for.h>
#include <kipr/core/core.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_ANALOG_PORT 0
#define TEST_DIGITAL_PORT 0
#define TEST_MOTOR_PORT 0
#define TEST_SERVO_PORT 0
#define TEST_CAMERA_PORT 0
#define MOTOR_COUNT 4
#define MAX_ISSUES 128

typedef enum ReviewChoice {
  REVIEW_OK,
  REVIEW_FAILED
} ReviewChoice;

typedef int (*ButtonProbe)(void);

static int step_number = 0;
static int failures = 0;
static struct {
  int step;
  const char *name;
} issues[MAX_ISSUES];

static volatile int worker_done = 0;
static volatile int worker_kind = 0;
static volatile int worker_result = 0;
static volatile int worker_has_result = 0;
static volatile int quick_worker_ran = 0;
static volatile int worker_probe_result = 0;
static ButtonProbe worker_button_probe = 0;
static volatile int c_wait_started = 0;
static volatile int c_clicked_wait_started = 0;
static volatile int c_click_probe_started = 0;
static unsigned long last_live_refresh = 0;

enum LiveReading {
  LIVE_NONE,
  LIVE_ANALOG,
  LIVE_DIGITAL,
  LIVE_ACCEL,
  LIVE_GYRO,
  LIVE_MOTOR,
  LIVE_ALL_MOTORS,
  LIVE_SERVO
};

static enum LiveReading live_reading = LIVE_NONE;

enum WorkerKind {
  WORKER_TOUCH = 1,
  WORKER_MILLISECONDS,
  WORKER_ACCEL_CALIBRATE,
  WORKER_GYRO_CALIBRATE,
  WORKER_CAMERA_OPEN,
  WORKER_CAMERA_OPEN_BLACK,
  WORKER_CAMERA_OPEN_AT_RES,
  WORKER_CAMERA_OPEN_DEVICE,
  WORKER_CAMERA_OPEN_MODEL,
  WORKER_GRAPHICS_OPEN,
  WORKER_WAIT_A,
  WORKER_WAIT_B,
  WORKER_WAIT_X,
  WORKER_WAIT_Y,
  WORKER_WAIT_Z,
  WORKER_WAIT_ANY,
  WORKER_CLICK_A,
  WORKER_CLICK_B,
  WORKER_CLICK_X,
  WORKER_CLICK_Y,
  WORKER_CLICK_Z,
  WORKER_LIGHT
};

static void stop_now(void)
{
  int i;
  alloff();
  disable_servos();
  display_clear();
  printf("API TEST STOPPED (C pressed; motors off)\n");
  printf("User-marked issues (%d):\n", failures);
  if (!failures) printf("None\n");
  for (i = 0; i < failures && i < MAX_ISSUES; ++i)
    printf("[%03d] %s\n", issues[i].step, issues[i].name);
  fflush(stdout);
  exit(0);
}

static void check_quit(void)
{
  if (c_button()) stop_now();
}

static void clear_screen(void)
{
  console_clear();
  display_clear();
}

static void screen_line(int row, const char *text)
{
  display_printf(0, row, "%-40.40s", text ? text : "");
}

static void screen_wrapped(int row, const char *text, int max_rows)
{
  int line = 0;
  const char *cursor = text ? text : "";

  while (*cursor && line < max_rows) {
    char part[41];
    int length = 0;
    int copy_length;
    while (length < 40 && cursor[length]) ++length;
    copy_length = length;
    if (cursor[length] && length == 40) {
      int last_space = -1;
      int i;
      for (i = 0; i < length; ++i) {
        if (cursor[i] == ' ') last_space = i;
      }
      if (last_space > 0) copy_length = last_space;
    }
    memcpy(part, cursor, copy_length);
    part[copy_length] = '\0';
    screen_line(row + line, part);
    cursor += copy_length;
    while (*cursor == ' ') ++cursor;
    ++line;
  }
}

static void refresh_live_values(void)
{
  switch (live_reading) {
    case LIVE_ANALOG:
      display_printf(0, 4, "AN%d: %5d          ", TEST_ANALOG_PORT,
                     analog(TEST_ANALOG_PORT));
      break;
    case LIVE_DIGITAL:
      display_printf(0, 4, "DI%d: %d              ", TEST_DIGITAL_PORT,
                     digital(TEST_DIGITAL_PORT));
      break;
    case LIVE_ACCEL:
      display_printf(0, 4, "ACC: %5d %5d %5d   ", accel_x(), accel_y(), accel_z());
      break;
    case LIVE_GYRO:
      display_printf(0, 4, "GYR: %5d %5d %5d   ", gyro_x(), gyro_y(), gyro_z());
      break;
    case LIVE_MOTOR:
      display_printf(0, 4, "M%d counter: %6d    ", TEST_MOTOR_PORT,
                     get_motor_position_counter(TEST_MOTOR_PORT));
      break;
    case LIVE_ALL_MOTORS:
      display_printf(0, 4, "M0-3: %d %d %d %d   ",
                     get_motor_position_counter(0),
                     get_motor_position_counter(1),
                     get_motor_position_counter(2),
                     get_motor_position_counter(3));
      break;
    case LIVE_SERVO:
      display_printf(0, 4, "S%d target: %4d enabled: %d   ", TEST_SERVO_PORT,
                     get_servo_position(TEST_SERVO_PORT),
                     get_servo_enabled(TEST_SERVO_PORT));
      break;
    case LIVE_NONE:
      break;
  }
  last_live_refresh = systime();
}

static void refresh_live_if_due(void)
{
  const unsigned long now = systime();
  if (live_reading != LIVE_NONE && now - last_live_refresh >= 100)
    refresh_live_values();
}

static void begin_action(const char *name, const char *instruction)
{
  char heading[80];
  clear_screen();
  screen_line(0, "API EXERCISER");
  snprintf(heading, sizeof(heading), "RUNNING: %s", name);
  screen_line(1, heading);
  screen_wrapped(2, instruction, 2);
  screen_line(5, "C: stop now");
  refresh_live_values();
}

static void begin_check_screen(const char *name, const char *expected,
                               const char *debug, int is_debug)
{
  char heading[80];
  clear_screen();
  snprintf(heading, sizeof(heading), "%s[%03d] %s",
           is_debug ? "ISSUE " : "", step_number, name);
  screen_line(0, heading);
  if (is_debug) {
    screen_wrapped(1, expected, 1);
    screen_wrapped(2, debug, 2);
  } else {
    screen_wrapped(1, expected, 2);
    screen_wrapped(3, debug, live_reading == LIVE_NONE ? 2 : 1);
  }
  screen_line(5, is_debug ? "B: continue  C: quit" : "B: correct A: issue C: quit");
  refresh_live_values();
}

static void sleep_interruptible(long milliseconds)
{
  long elapsed = 0;
  while (elapsed < milliseconds) {
    check_quit();
    refresh_live_if_due();
    msleep(10);
    elapsed += 10;
  }
}

static void wait_for_release(void)
{
  while (1) {
    check_quit();
    refresh_live_if_due();
    if (!a_button() && !b_button()) return;
    msleep(20);
  }
}

static int get_review_choice(void)
{
  wait_for_release();
  while (1) {
    check_quit();
    refresh_live_if_due();
    if (a_button()) {
      wait_for_release();
      return 0;
    }
    if (b_button()) {
      wait_for_release();
      return 1;
    }
    msleep(20);
  }
}

static void show_failure(const char *name, const char *expected,
                         const char *debug)
{
  begin_check_screen(name, expected, debug, 1);
  while (1) {
    check_quit();
    refresh_live_if_due();
    if (b_button()) {
      wait_for_release();
      return;
    }
    msleep(20);
  }
}

static ReviewChoice review(const char *name, const char *expected,
                           const char *debug)
{
  int accepted;
  ++step_number;
  begin_check_screen(name, expected, debug, 0);
  accepted = get_review_choice();
  if (accepted) return REVIEW_OK;
  if (failures < MAX_ISSUES) {
    issues[failures].step = step_number;
    issues[failures].name = name;
  }
  ++failures;
  show_failure(name, expected, debug);
  return REVIEW_FAILED;
}

static int probe_a(void) { return a_button(); }
static int probe_b(void) { return b_button(); }
static int probe_x(void) { return x_button(); }
static int probe_y(void) { return y_button(); }
static int probe_z(void) { return z_button(); }
static int probe_push(void) { return push_button(); }
static int probe_any(void) { return any_button(); }
static int probe_a_clicked(void) { return a_button_clicked(); }
static int probe_b_clicked(void) { return b_button_clicked(); }
static int probe_x_clicked(void) { return x_button_clicked(); }
static int probe_y_clicked(void) { return y_button_clicked(); }
static int probe_z_clicked(void) { return z_button_clicked(); }

static void click_probe_worker(void)
{
  while (!worker_probe_result) {
    worker_probe_result = worker_button_probe();
    if (!worker_probe_result) msleep(10);
  }
  worker_done = 1;
}

static void c_wait_worker(void)
{
  c_wait_started = 1;
  wait_for_c_button();
}

static void c_clicked_wait_worker(void)
{
  c_clicked_wait_started = 1;
  wait_for_c_button_clicked();
}

static void c_click_probe_worker(void)
{
  c_click_probe_started = 1;
  while (!c_button_clicked()) msleep(10);
}

static void test_c_quit_api(void)
{
  thread wait_task = thread_create(c_wait_worker);
  thread clicked_wait_task = thread_create(c_clicked_wait_worker);
  thread click_probe_task = thread_create(c_click_probe_worker);

  thread_start(wait_task);
  thread_start(clicked_wait_task);
  thread_start(click_probe_task);
  while (!c_wait_started || !c_clicked_wait_started || !c_click_probe_started) {
    msleep(1);
  }
  begin_action("C button exit",
               "C button APIs are active. Press C to stop and print the issue list.");
  display_printf(0, 6, "User-marked issues: %d", failures);
  while (1) {
    check_quit();
    refresh_live_if_due();
    msleep(10);
  }
}

static int run_button_probe(const char *name, const char *instruction,
                            ButtonProbe probe)
{
  char debug[192];
  int sampled = 0;
  begin_action(name, instruction);
  while (!sampled) {
    check_quit();
    refresh_live_if_due();
    sampled = probe();
    if (!sampled) msleep(20);
  }
  snprintf(debug, sizeof(debug), "%s returned %d after the requested input",
           name, sampled);
  review(name, "the API detects the requested button/input", debug);
  return sampled;
}

static void run_button_click_probe(const char *name, const char *instruction,
                                   ButtonProbe probe)
{
  char debug[192];
  thread task;

  begin_action(name, instruction);
  worker_button_probe = probe;
  worker_probe_result = 0;
  worker_done = 0;
  task = thread_create(click_probe_worker);
  thread_start(task);
  while (!worker_done) {
    check_quit();
    refresh_live_if_due();
    msleep(10);
  }
  thread_wait(task);
  thread_destroy(task);
  snprintf(debug, sizeof(debug), "%s returned %d after press and release",
           name, worker_probe_result);
  review(name, "the click API reports the requested press-and-release", debug);
}

static void worker_function(void)
{
  switch (worker_kind) {
    case WORKER_TOUCH: wait_for_touch(TEST_DIGITAL_PORT); break;
    case WORKER_MILLISECONDS: wait_for_milliseconds(100); break;
    case WORKER_ACCEL_CALIBRATE:
      worker_result = accel_calibrate();
      worker_has_result = 1;
      break;
    case WORKER_GYRO_CALIBRATE:
      worker_result = gyro_calibrate();
      worker_has_result = 1;
      break;
    case WORKER_CAMERA_OPEN:
      worker_result = camera_open();
      worker_has_result = 1;
      break;
    case WORKER_CAMERA_OPEN_BLACK:
      worker_result = camera_open_black();
      worker_has_result = 1;
      break;
    case WORKER_CAMERA_OPEN_AT_RES:
      worker_result = camera_open_at_res(LOW_RES);
      worker_has_result = 1;
      break;
    case WORKER_CAMERA_OPEN_DEVICE:
      worker_result = camera_open_device(TEST_CAMERA_PORT, LOW_RES);
      worker_has_result = 1;
      break;
    case WORKER_CAMERA_OPEN_MODEL:
      worker_result = camera_open_device_model_at_res(
          TEST_CAMERA_PORT, BLACK_2017, LOW_RES);
      worker_has_result = 1;
      break;
    case WORKER_GRAPHICS_OPEN:
      worker_result = graphics_open(160, 120);
      worker_has_result = 1;
      break;
    case WORKER_WAIT_A: wait_for_a_button(); break;
    case WORKER_WAIT_B: wait_for_b_button(); break;
    case WORKER_WAIT_X: wait_for_x_button(); break;
    case WORKER_WAIT_Y: wait_for_y_button(); break;
    case WORKER_WAIT_Z: wait_for_z_button(); break;
    case WORKER_WAIT_ANY: wait_for_any_button(); break;
    case WORKER_CLICK_A: wait_for_a_button_clicked(); break;
    case WORKER_CLICK_B: wait_for_b_button_clicked(); break;
    case WORKER_CLICK_X: wait_for_x_button_clicked(); break;
    case WORKER_CLICK_Y: wait_for_y_button_clicked(); break;
    case WORKER_CLICK_Z: wait_for_z_button_clicked(); break;
    case WORKER_LIGHT: wait_for_light(TEST_ANALOG_PORT); break;
    default: break;
  }
  worker_done = 1;
}

static void run_wait_test(const char *name, int kind, const char *instruction,
                          const char *expected)
{
  char debug[256];
  thread task;

  if (kind == WORKER_TOUCH) live_reading = LIVE_DIGITAL;
  else if (kind == WORKER_ACCEL_CALIBRATE) live_reading = LIVE_ACCEL;
  else if (kind == WORKER_GYRO_CALIBRATE) live_reading = LIVE_GYRO;
  else live_reading = LIVE_NONE;
  begin_action(name, instruction);

  worker_kind = kind;
  worker_done = 0;
  worker_has_result = 0;
  worker_result = 0;
  task = thread_create(worker_function);
  thread_start(task);
  while (!worker_done) {
    check_quit();
    refresh_live_if_due();
    msleep(10);
  }
  thread_wait(task);
  thread_destroy(task);

  if (worker_has_result) {
    snprintf(debug, sizeof(debug),
             "%s returned %d; helper thread was started, joined, and destroyed",
             name, worker_result);
  } else {
    snprintf(debug, sizeof(debug),
             "%s returned; helper thread was started, joined, and destroyed",
             name);
  }
  review(name, expected, debug);
}

static void quick_worker(void)
{
  quick_worker_ran = 1;
}

static void test_thread_api(void)
{
  thread task;
  char debug[192];

  quick_worker_ran = 0;
  task = thread_create(quick_worker);
  snprintf(debug, sizeof(debug), "thread handle data=%p",
           task.data);
  review("thread_create", "a usable thread handle is returned", debug);

  thread_start(task);
  while (!quick_worker_ran) sleep_interruptible(10);
  review("thread_start", "the worker starts and sets its completion flag",
         "quick_worker_ran=1 after thread_start");

  thread_wait(task);
  review("thread_wait", "the completed worker can be joined",
         "quick_worker_ran=1; thread_wait returned");

  thread_destroy(task);
  review("thread_destroy", "the joined thread handle is released",
         "thread_destroy returned after thread_wait");
}

static void test_mutex_api(void)
{
  mutex lock = mutex_create();
  int got_lock = mutex_trylock(lock);
  char debug[128];

  snprintf(debug, sizeof(debug), "mutex_trylock returned %d", got_lock);
  review("mutex_create / mutex_trylock", "a new mutex can be locked once",
         debug);

  if (got_lock) mutex_unlock(lock);
  mutex_lock(lock);
  review("mutex_lock", "the unlocked mutex can be locked", "mutex_lock returned");

  mutex_unlock(lock);
  review("mutex_unlock", "the mutex becomes available again",
         "mutex_unlock returned");

  mutex_destroy(lock);
  review("mutex_destroy", "the mutex is released without an error",
         "mutex_destroy returned after unlock");
}

static void camera_data_sweep(void)
{
  char debug[512];
  int updated;
  int width;
  int height;
  int channels;
  int count;
  const char *data;
  int code;
  int data_length;
  double confidence;
  int area;
  rectangle bbox;
  int ulx;
  int uly;
  int brx;
  int bry;
  int bbox_width;
  int bbox_height;
  point2 centroid;
  int centroid_column;
  int centroid_x;
  int centroid_row;
  int centroid_y;
  point2 center;
  int center_column;
  int center_x;
  int center_row;
  int center_y;
  pixel p;
  const unsigned char *row = 0;
  const unsigned char *frame = 0;
  unsigned element_size;

  begin_action("camera frame/object access", "Read a frame and query channel 0 object data.");
  updated = camera_update();
  width = get_camera_width();
  height = get_camera_height();
  channels = get_channel_count();
  count = get_object_count(0);
  data = get_object_data(0, 0);
  code = get_code_num(0, 0);
  data_length = get_object_data_length(0, 0);
  confidence = get_object_confidence(0, 0);
  area = get_object_area(0, 0);
  bbox = get_object_bbox(0, 0);
  ulx = get_object_bbox_ulx(0, 0);
  uly = get_object_bbox_uly(0, 0);
  brx = get_object_bbox_brx(0, 0);
  bry = get_object_bbox_bry(0, 0);
  bbox_width = get_object_bbox_width(0, 0);
  bbox_height = get_object_bbox_height(0, 0);
  centroid = get_object_centroid(0, 0);
  centroid_column = get_object_centroid_column(0, 0);
  centroid_x = get_object_centroid_x(0, 0);
  centroid_row = get_object_centroid_row(0, 0);
  centroid_y = get_object_centroid_y(0, 0);
  center = get_object_center(0, 0);
  center_column = get_object_center_column(0, 0);
  center_x = get_object_center_x(0, 0);
  center_row = get_object_center_row(0, 0);
  center_y = get_object_center_y(0, 0);
  element_size = get_camera_element_size();

  if (updated) {
    p = get_camera_pixel(create_point2(0, 0));
    row = get_camera_frame_row(0);
    frame = get_camera_frame();
  } else {
    p.r = p.g = p.b = 0;
  }

  snprintf(debug, sizeof(debug),
           "update=%d size=%dx%d channels=%d objects(ch0)=%d "
           "data=%s code=%d len=%d conf=%.3f area=%d "
           "bbox=(%d,%d %dx%d; edges %d,%d..%d,%d; accessors=%dx%d) "
           "centroid=(%d,%d; cols/rows %d,%d,%d,%d) "
           "center=(%d,%d; cols/rows %d,%d,%d,%d) "
           "pixel=(%d,%d,%d) row=%p frame=%p element_size=%u",
           updated, width, height, channels, count, data ? data : "(null)",
           code, data_length, confidence, area, bbox.ulx, bbox.uly,
           bbox.width, bbox.height, ulx, uly, brx, bry,
           bbox_width, bbox_height,
           centroid.x, centroid.y, centroid_column, centroid_x,
           centroid_row, centroid_y, center.x, center.y,
           center_column, center_x, center_row, center_y,
           p.r, p.g, p.b, (const void *)row, (const void *)frame,
           element_size);
  review("camera frame and object accessors",
         "a current frame is readable; object accessors return data or safe empty results",
         debug);
}

static void test_camera_api(void)
{
  int ready = 0;
  int opened;
  int data_tested = 0;

  set_camera_config_base_path(".");
  review("set_camera_config_base_path", "the config search base is set to the current directory",
         "set_camera_config_base_path(\".\") returned");

  opened = camera_load_config("api_exerciser");
  review("camera_load_config", "returns 1 if api_exerciser.conf exists, otherwise 0",
         opened ? "api_exerciser.conf loaded" : "api_exerciser.conf was not found");

  run_wait_test("camera_open", WORKER_CAMERA_OPEN,
                "Open the default camera at its default resolution.",
                "returns 1 when the requested camera opens, otherwise 0");
  opened = worker_result;
  ready = opened != 0;
  if (ready) {
    data_tested = 1;
    camera_data_sweep();
    camera_close();
    review("camera_close", "the opened camera is released", "camera_close returned");
    ready = 0;
  }

  run_wait_test("camera_open_black", WORKER_CAMERA_OPEN_BLACK,
                "Open the default black camera.",
                "returns 1 when the requested camera opens, otherwise 0");
  opened = worker_result;
  ready = opened != 0;
  if (ready) {
    data_tested = 1;
    camera_data_sweep();
    camera_close();
    review("camera_close", "the opened camera is released", "camera_close returned");
    ready = 0;
  }

  run_wait_test("camera_open_at_res(LOW_RES)", WORKER_CAMERA_OPEN_AT_RES,
                "Open the default camera at low resolution.",
                "returns 1 when the requested camera opens, otherwise 0");
  opened = worker_result;
  ready = opened != 0;
  if (ready) {
    data_tested = 1;
    camera_data_sweep();
    camera_close();
    review("camera_close", "the opened camera is released", "camera_close returned");
    ready = 0;
  }

  run_wait_test("camera_open_device", WORKER_CAMERA_OPEN_DEVICE,
                "Open camera device 0 at low resolution.",
                "returns 1 when the requested camera opens, otherwise 0");
  opened = worker_result;
  ready = opened != 0;
  if (ready) {
    data_tested = 1;
    camera_data_sweep();
    camera_close();
    review("camera_close", "the opened camera is released", "camera_close returned");
    ready = 0;
  }

  run_wait_test("camera_open_device_model_at_res", WORKER_CAMERA_OPEN_MODEL,
                "Open camera device 0 as BLACK_2017 at low resolution.",
                "returns 1 when the requested camera opens, otherwise 0");
  opened = worker_result;
  ready = opened != 0;
  if (ready) {
    data_tested = 1;
    camera_data_sweep();
    camera_close();
    review("camera_close", "the opened camera is released", "camera_close returned");
    ready = 0;
  }

  camera_close();
  review("camera_close (closed-camera call)", "closing an already closed camera is harmless",
         "camera_close returned");

  if (!data_tested) {
    review("camera data access", "an attached camera opens before frame/object access",
           "all camera open calls returned 0, so frame and object getters were skipped");
  }
}

static void test_graphics_api(void)
{
  unsigned char pixels[4 * 4 * 3] = {
    255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 0,
    0, 255, 255, 255, 0, 255, 64, 64, 64, 200, 100, 50,
    50, 100, 200, 20, 40, 60, 90, 120, 150, 180, 210, 240,
    12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144
  };
  int segments[NUMSEG] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
                          1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
                          1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  char text[] = "API";
  int x = -1;
  int y = -1;
  int opened;
  char debug[256];

  run_wait_test("graphics_open", WORKER_GRAPHICS_OPEN,
                "Open a 160x120 graphics window.",
                "the graphics window opens and returns success");
  opened = worker_result;
  if (!opened) {
    review("graphics drawing/input access", "graphics_open succeeds before drawing/input checks",
           "graphics_open returned 0, so window-dependent calls were skipped");
    graphics_close();
    review("graphics_close", "closing an unopened graphics window is harmless",
           "graphics_close returned");
    return;
  }

  begin_action("graphics drawing sweep", "Draw the full set of shapes, blits, and text.");
  graphics_clear();
  graphics_fill(8, 8, 8);
  graphics_pixel(4, 4, 255, 255, 255);
  graphics_line(8, 8, 30, 8, 255, 0, 0);
  graphics_circle(20, 24, 8, 0, 255, 0);
  graphics_circle_fill(45, 24, 8, 0, 0, 255);
  graphics_rectangle(60, 8, 82, 28, 255, 255, 0);
  graphics_rectangle_fill(88, 8, 110, 28, 0, 255, 255);
  graphics_triangle(8, 50, 24, 34, 40, 50, 255, 0, 255);
  graphics_triangle_fill(48, 50, 64, 34, 80, 50, 255, 128, 0);
  graphics_blit(pixels, 112, 8, 4, 4);
  graphics_blit_region(pixels, 0, 0, 2, 2, 4, 4, 128, 8);
  graphics_blit_enc(pixels, BGR, 112, 24, 4, 4);
  graphics_blit_region_enc(pixels, RGB, 0, 0, 2, 2, 4, 4, 128, 24);
  graphics_segment_display(segments, 8, 62, 255, 255, 255, 1.0f);
  graphics_print_character('A', 48, 62, 255, 255, 255, 1.0f);
  graphics_print_string(text, 64, 62, 255, 255, 255, 1.0f);
  graphics_print_int(123, 0, 88, 62, 255, 255, 255, 1.0f);
  graphics_print_float(3.14f, 2, 112, 62, 255, 255, 255, 1.0f);
  graphics_update();
  review("graphics drawing sweep",
         "the window shows pixels, lines, circles, shapes, blits, and text",
         "all drawing calls returned; graphics_update was called");

  {
    int key = get_key_state(KeyA);
    int middle = get_mouse_middle_button();
    int left = get_mouse_left_button();
    int right = get_mouse_right_button();
    get_mouse_position(&x, &y);
    snprintf(debug, sizeof(debug),
             "KeyA=%d mouse=(%d,%d) buttons middle/left/right=%d/%d/%d",
             key, x, y, middle, left, right);
  }
  review("graphics keyboard and mouse accessors",
         "keyboard state is 0/1 and mouse position/buttons match current input",
         debug);

  graphics_close();
  review("graphics_close", "the graphics window closes", "graphics_close returned");
}

static void test_core_and_geometry(void)
{
  point2 p2 = create_point2(12, 34);
  point3 p3 = create_point3(5, 6, 7);
  rectangle r = create_rectangle(10, 20, 30, 40);
  char debug[384];
  const char *branch = kipr_git_branch();
  const char *commit = kipr_git_commit_hash();
  const char *version = kipr_version();
  const char *built = kipr_build_datetime();

  snprintf(debug, sizeof(debug),
           "branch=%s commit=%s version=%s (%d.%d.%d) build=%s",
           branch ? branch : "(null)", commit ? commit : "(null)",
           version ? version : "(null)", kipr_version_major(),
           kipr_version_minor(), kipr_version_patch(),
           built ? built : "(null)");
  review("core version/build metadata",
         "metadata strings and version components describe this libwallaby build",
         debug);

  snprintf(debug, sizeof(debug),
           "point2=(%d,%d) point3=(%d,%d,%d) rectangle=(%d,%d %dx%d)",
           p2.x, p2.y, p3.x, p3.y, p3.z, r.ulx, r.uly, r.width, r.height);
  review("geometry constructors",
         "constructed values retain the coordinates and dimensions passed in",
         debug);
}

static void test_camera_path_and_display(void)
{
  display_clear();
  display_printf(0, 0, "libwallaby API check");
  display_printf(0, 1, "Display API call OK");
  sleep_interruptible(2000);
  review("display_clear / display_printf",
         "the display clears and shows two lines of text",
         "display_clear returned; display_printf wrote rows 0 and 1");
  console_clear();
  sleep_interruptible(500);
  review("console_clear", "the text console clears", "console_clear returned");
}

static void test_sensor_api(void)
{
  int value;
  int minimum;
  int maximum;
  int i;
  char debug[128];

  live_reading = LIVE_ANALOG;
  begin_action("analog", "Change the analog sensor on the selected port for 3 seconds.");
  minimum = maximum = analog(TEST_ANALOG_PORT);
  for (i = 0; i < 30; ++i) {
    sleep_interruptible(100);
    value = analog(TEST_ANALOG_PORT);
    if (value < minimum) minimum = value;
    if (value > maximum) maximum = value;
  }
  snprintf(debug, sizeof(debug), "AN%d min=%d max=%d (%s)", TEST_ANALOG_PORT,
           minimum, maximum, minimum != maximum ? "changed" : "unchanged");
  review("analog", "the reading changes when the analog sensor changes",
         debug);

  run_wait_test("wait_for_light", WORKER_LIGHT,
                "Follow the built-in B prompts for light on/off, then illuminate the start light.",
                "the call returns after the calibrated light is detected");

  live_reading = LIVE_DIGITAL;
  begin_action("digital", "Activate and release the digital sensor for 3 seconds.");
  minimum = maximum = digital(TEST_DIGITAL_PORT);
  for (i = 0; i < 30; ++i) {
    sleep_interruptible(100);
    value = digital(TEST_DIGITAL_PORT);
    if (value < minimum) minimum = value;
    if (value > maximum) maximum = value;
  }
  snprintf(debug, sizeof(debug), "DI%d min=%d max=%d (%s)", TEST_DIGITAL_PORT,
           minimum, maximum, minimum != maximum ? "changed" : "unchanged");
  review("digital", "the input reports both low and high as it is activated",
         debug);

  run_wait_test("wait_for_touch", WORKER_TOUCH,
                "Activate the digital touch sensor on the selected port.",
                "the call returns after the digital input becomes active");
}

static void test_motion_sensors(void)
{
  char debug[160];
  run_wait_test("accel_calibrate", WORKER_ACCEL_CALIBRATE,
                "Keep the controller still for about half a second.",
                "calibration returns success while the controller is still");

  {
    signed short x = accel_x();
    signed short y = accel_y();
    signed short z = accel_z();
    snprintf(debug, sizeof(debug), "accel_x/y/z = %d / %d / %d", x, y, z);
  }
  review("accel_x / accel_y / accel_z",
         "the axes react to orientation; flat and still is near (0,0,-512)",
         debug);

  run_wait_test("gyro_calibrate", WORKER_GYRO_CALIBRATE,
                "Keep the controller still for about half a second.",
                "calibration completes successfully with the controller still");

  {
    signed short x = gyro_x();
    signed short y = gyro_y();
    signed short z = gyro_z();
    snprintf(debug, sizeof(debug), "gyro_x/y/z = %d / %d / %d", x, y, z);
  }
  review("gyro_x / gyro_y / gyro_z",
         "the axes are near zero while still and change when rotated",
         debug);
}

static void test_motor_api(void)
{
  int start;
  int forward;
  int reverse;
  int cleared;
  int forward_result;
  int reverse_result;
  char debug[192];

  live_reading = LIVE_MOTOR;
  clear_motor_position_counter(TEST_MOTOR_PORT);
  start = get_motor_position_counter(TEST_MOTOR_PORT);
  begin_action("move_at_velocity forward", "With wheels raised, run the motor forward at 400 ticks/s for 2 seconds.");
  forward_result = move_at_velocity(TEST_MOTOR_PORT, 400);
  sleep_interruptible(2000);
  off(TEST_MOTOR_PORT);
  forward = get_motor_position_counter(TEST_MOTOR_PORT);
  snprintf(debug, sizeof(debug),
           "command=%d; counter %d -> %d (%s); off called",
           forward_result, start, forward,
           forward > start + 100 ? "increased" : "did not increase");
  review("move_at_velocity forward / off",
         "motor turns forward, stops, and counter rises by at least 100 ticks",
         debug);

  begin_action("move_at_velocity reverse", "Run the motor in reverse at 400 ticks/s for 2 seconds.");
  reverse_result = move_at_velocity(TEST_MOTOR_PORT, -400);
  sleep_interruptible(2000);
  off(TEST_MOTOR_PORT);
  reverse = get_motor_position_counter(TEST_MOTOR_PORT);
  snprintf(debug, sizeof(debug),
           "command=%d; counter %d -> %d (%s); off called",
           reverse_result, forward, reverse,
           reverse < forward - 100 ? "decreased" : "did not decrease");
  review("motor position counter / reverse",
         "motor turns backward, stops, and counter falls by at least 100 ticks",
         debug);

  clear_motor_position_counter(TEST_MOTOR_PORT);
  cleared = get_motor_position_counter(TEST_MOTOR_PORT);
  snprintf(debug, sizeof(debug), "counter %d -> %d after clear", reverse, cleared);
  review("clear_motor_position_counter",
         "the counter returns near zero after the motor has moved", debug);
}

static void test_alloff(void)
{
  int start[MOTOR_COUNT];
  int running[MOTOR_COUNT];
  int settling[MOTOR_COUNT];
  int stopped[MOTOR_COUNT];
  int moved_count = 0;
  int stopped_count = 0;
  int i;
  char debug[192];

  live_reading = LIVE_ALL_MOTORS;
  begin_action("alloff", "With all wheels raised, run motors 0-3 for 2 seconds, then watch them stop.");
  for (i = 0; i < MOTOR_COUNT; ++i)
    start[i] = get_motor_position_counter(i);
  for (i = 0; i < MOTOR_COUNT; ++i)
    move_at_velocity(i, 400);
  sleep_interruptible(2000);
  for (i = 0; i < MOTOR_COUNT; ++i)
    running[i] = get_motor_position_counter(i);
  alloff();
  sleep_interruptible(300);
  for (i = 0; i < MOTOR_COUNT; ++i)
    settling[i] = get_motor_position_counter(i);
  sleep_interruptible(1000);
  for (i = 0; i < MOTOR_COUNT; ++i) {
    stopped[i] = get_motor_position_counter(i);
    if (abs(running[i] - start[i]) >= 100) ++moved_count;
    if (abs(stopped[i] - settling[i]) <= 30) ++stopped_count;
  }
  snprintf(debug, sizeof(debug),
           "moved=%d/4 stopped=%d/4; post-off drift M0-3=%d/%d/%d/%d ticks",
           moved_count, stopped_count,
           abs(stopped[0] - settling[0]), abs(stopped[1] - settling[1]),
           abs(stopped[2] - settling[2]), abs(stopped[3] - settling[3]));
  live_reading = LIVE_NONE;
  review("alloff", "all four motors turn, then stop; counters stay steady after stopping",
         debug);
}

static void test_servo_api(void)
{
  int before = get_servo_enabled(TEST_SERVO_PORT);
  char debug[160];
  int position;

  live_reading = LIVE_SERVO;

  snprintf(debug, sizeof(debug), "port=%d enabled=%d", TEST_SERVO_PORT, before);
  review("get_servo_enabled", "the query returns 0 or 1 for servo 0", debug);

  begin_action("set_servo_position", "Set servo 0 target to midpoint 1024.");
  set_servo_position(TEST_SERVO_PORT, 1024);
  position = get_servo_position(TEST_SERVO_PORT);
  snprintf(debug, sizeof(debug), "commanded midpoint; get_servo_position=%d", position);
  review("set_servo_position / get_servo_position",
         "the stored target is near the midpoint (1024)", debug);

  begin_action("enable_servo", "Enable servo 0; it should move toward its midpoint.");
  enable_servo(TEST_SERVO_PORT);
  sleep_interruptible(250);
  before = get_servo_enabled(TEST_SERVO_PORT);
  snprintf(debug, sizeof(debug), "enable_servo called; enabled=%d", before);
  review("enable_servo", "servo 0 is enabled", debug);

  set_servo_enabled(TEST_SERVO_PORT, 0);
  before = get_servo_enabled(TEST_SERVO_PORT);
  snprintf(debug, sizeof(debug), "set_servo_enabled(0); enabled=%d", before);
  review("set_servo_enabled(0)", "servo 0 is disabled", debug);

  set_servo_enabled(TEST_SERVO_PORT, 1);
  before = get_servo_enabled(TEST_SERVO_PORT);
  snprintf(debug, sizeof(debug), "set_servo_enabled(1); enabled=%d", before);
  review("set_servo_enabled(1)", "servo 0 is enabled", debug);

  disable_servo(TEST_SERVO_PORT);
  before = get_servo_enabled(TEST_SERVO_PORT);
  snprintf(debug, sizeof(debug), "disable_servo called; enabled=%d", before);
  review("disable_servo", "servo 0 is disabled", debug);

  enable_servos();
  before = get_servo_enabled(TEST_SERVO_PORT);
  snprintf(debug, sizeof(debug), "enable_servos called; servo 0 enabled=%d", before);
  review("enable_servos", "all servo channels are enabled", debug);

  disable_servos();
  before = get_servo_enabled(TEST_SERVO_PORT);
  snprintf(debug, sizeof(debug), "disable_servos called; servo 0 enabled=%d", before);
  review("disable_servos", "all servo channels are disabled", debug);
}

static void test_buttons_api(void)
{
  int visible;
  char debug[192];

  live_reading = LIVE_NONE;

  extra_buttons_show();
  visible = get_extra_buttons_visible();
  snprintf(debug, sizeof(debug), "extra_buttons_show; visible=%d", visible);
  review("extra_buttons_show / get_extra_buttons_visible",
         "X, Y, and Z controls become visible and the query returns 1", debug);

  run_button_probe("a_button", "Press and hold A until it is detected.", probe_a);
  run_button_click_probe("a_button_clicked", "Press and release A to generate a click.", probe_a_clicked);
  run_wait_test("wait_for_a_button", WORKER_WAIT_A,
                "Press and hold A to release the waiting call.",
                "the call returns while A is pressed");
  run_wait_test("wait_for_a_button_clicked", WORKER_CLICK_A,
                "Press and release A to release the waiting call.",
                "the call returns after an A press and release");

  run_button_probe("b_button", "Press and hold B until it is detected.", probe_b);
  run_button_click_probe("b_button_clicked", "Press and release B to generate a click.", probe_b_clicked);
  run_wait_test("wait_for_b_button", WORKER_WAIT_B,
                "Press and hold B to release the waiting call.",
                "the call returns while B is pressed");
  run_wait_test("wait_for_b_button_clicked", WORKER_CLICK_B,
                "Press and release B to release the waiting call.",
                "the call returns after a B press and release");

  run_button_probe("x_button", "Press and hold X until it is detected.", probe_x);
  run_button_click_probe("x_button_clicked", "Press and release X to generate a click.", probe_x_clicked);
  run_wait_test("wait_for_x_button", WORKER_WAIT_X,
                "Press and hold X to release the waiting call.",
                "the call returns while X is pressed");
  run_wait_test("wait_for_x_button_clicked", WORKER_CLICK_X,
                "Press and release X to release the waiting call.",
                "the call returns after an X press and release");

  run_button_probe("y_button", "Press and hold Y until it is detected.", probe_y);
  run_button_click_probe("y_button_clicked", "Press and release Y to generate a click.", probe_y_clicked);
  run_wait_test("wait_for_y_button", WORKER_WAIT_Y,
                "Press and hold Y to release the waiting call.",
                "the call returns while Y is pressed");
  run_wait_test("wait_for_y_button_clicked", WORKER_CLICK_Y,
                "Press and release Y to release the waiting call.",
                "the call returns after a Y press and release");

  run_button_probe("z_button", "Press and hold Z until it is detected.", probe_z);
  run_button_click_probe("z_button_clicked", "Press and release Z to generate a click.", probe_z_clicked);
  run_wait_test("wait_for_z_button", WORKER_WAIT_Z,
                "Press and hold Z to release the waiting call.",
                "the call returns while Z is pressed");
  run_wait_test("wait_for_z_button_clicked", WORKER_CLICK_Z,
                "Press and release Z to release the waiting call.",
                "the call returns after a Z press and release");

  run_button_probe("push_button", "Press and hold the physical push button until detected.", probe_push);
  run_button_probe("any_button", "Press and hold X; any_button should become true.", probe_any);
  run_wait_test("wait_for_any_button", WORKER_WAIT_ANY,
                "Press X to release the waiting call.",
                "the call returns after any supported button is pressed");

  /* a_button/b_button are also used by all review controls; c_button is the
     immediate quit path. */
  snprintf(debug, sizeof(debug), "control API sample: A=%d B=%d C=%d",
           a_button(), b_button(), c_button());
  review("a_button / b_button / c_button",
         "the button reads reflect the current pressed state", debug);

  set_extra_buttons_visible(0);
  visible = get_extra_buttons_visible();
  snprintf(debug, sizeof(debug), "set_extra_buttons_visible(0); visible=%d", visible);
  review("set_extra_buttons_visible(0)", "the extra controls become hidden", debug);

  set_extra_buttons_visible(1);
  visible = get_extra_buttons_visible();
  snprintf(debug, sizeof(debug), "set_extra_buttons_visible(1); visible=%d", visible);
  review("set_extra_buttons_visible(1)", "the extra controls become visible", debug);

  extra_buttons_hide();
  visible = get_extra_buttons_visible();
  snprintf(debug, sizeof(debug), "extra_buttons_hide; visible=%d", visible);
  review("extra_buttons_hide", "X, Y, and Z controls are hidden", debug);
}

static void test_time_api(void)
{
  char debug[128];
  unsigned long before;
  double seconds_before;

  before = systime();
  seconds_before = seconds();
  sleep_interruptible(100);
  snprintf(debug, sizeof(debug), "systime %lu -> %lu; seconds %.3f -> %.3f",
           before, systime(), seconds_before, seconds());
  review("msleep / systime / seconds",
         "the sleep lasts about 100 ms and both clocks advance", debug);

  run_wait_test("wait_for_milliseconds", WORKER_MILLISECONDS,
                "This is a short 100 ms pause; it should finish by itself.",
                "execution pauses for about 100 ms and then continues");
}

int main(void)
{
  char debug[256];

  snprintf(debug, sizeof(debug), "AN/DI/M/S/C=%d/%d/%d/%d/%d; edit TEST_* ports",
           TEST_ANALOG_PORT, TEST_DIGITAL_PORT, TEST_MOTOR_PORT,
           TEST_SERVO_PORT, TEST_CAMERA_PORT);
  review("ready", "raise motor wheels, clear servo linkage, then press B to start",
         debug);

  test_sensor_api();
  test_motion_sensors();
  test_motor_api();
  test_alloff();
  test_servo_api();
  test_buttons_api();
  test_time_api();
  test_core_and_geometry();
  test_camera_path_and_display();
  test_camera_api();
  test_graphics_api();
  test_mutex_api();
  test_thread_api();

  shut_down_in(86400.0);
  snprintf(debug, sizeof(debug),
           "shut_down_in(86400.0) scheduled exit in 24 hours; C ends this run");
  review("shut_down_in", "the delayed stop is scheduled; press C at the final exit check",
         debug);

  alloff();
  disable_servos();
  camera_close();
  graphics_close();
  test_c_quit_api();
  return 0;
}
