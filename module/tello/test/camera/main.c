#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "wpa_ctrl.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <kipr/camera/camera.h>
#include <kipr/time/time.h>
#include <kipr/tello/tello.h>

#define BUFSIZE 1024
#define TELLO_LENGTH 13

#define TELLO_CMD_PORT 8889
#define TELLO_STATE_PORT 8890
#define TELLO_VIDEO_PORT 11111

static int tello_cmd_socket;
static struct sockaddr_in tello_cmd_addr;

int main(void)
{
  int result;
  char buf[BUFSIZE];
  int len;
  int n;
  char *tellos;
  int send_result;

  if (wpa_sup_connect() == -1)
  {
    printf("WPA Supplicant not active\n");
    return -1;
  }
  tellos = tellos_find();

  if (tellos == NULL)
    return -1;
  else
    printf("Tellos: %s\n", tellos);

  // connect to the first tello on the list
  tello_connect(tellos);

  // wpa_cmd ("LIST_NETWORKS", buf);

  do
  {
    send_result = tello_send("command");
    printf("send_result %d\n", send_result);

  } while (send_result != 0);

  tello_send("streamon");

  camera_open_device_model_at_res(0, TELLO, HIGH_RES);

  printf("load config\n");
  fflush(NULL);
  int ret = camera_load_config("1");
  if (ret == 1)
    printf("...success\n");
  fflush(NULL);
  printf("waiting for camera_update\n");
  fflush(NULL);
  camera_update();
  printf("Cam width x height:  %d x %d\n", get_camera_width(), get_camera_height());
  fflush(NULL);

  printf("Num channels = %d\n", get_channel_count());
  fflush(NULL);

  int i;
  double start = seconds();
  const int num_imgs = 600;
  for (i = 0; i < num_imgs; ++i)
  {
    camera_update();
    int x = get_object_count(0) > 0 ? get_object_center_x(0, 0) : -1;
    int y = get_object_count(0) > 0 ? get_object_center_y(0, 0) : -1;
    printf("%f objs: %d  (%d,%d)\n", seconds(), get_object_count(0), x, y);
    fflush(NULL);
    // printf("%f\n", seconds());
  }
  double stop = seconds();

  printf("%f fps\n", ((double)num_imgs / (stop - start)));

  printf("Camera close\n");
  camera_close();
  close(tello_cmd_socket);

  return 0;
}
