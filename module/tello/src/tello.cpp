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

#include <kipr/camera/camera.h>
#include <kipr/time/time.h>
#include <kipr/tello/tello.h>

#define CONFIG_CTRL_IFACE_DIR "/var/run/wpa_supplicant/wlan0"
#define BUFSIZE 1024
#define TELLO_LENGTH 13

static struct wpa_ctrl *ctrl_conn;
static struct wpa_ctrl *mon_conn;

#define TELLO_CMD_PORT 8889
#define TELLO_STATE_PORT 8890
#define TELLO_VIDEO_PORT 11111

static int tello_cmd_socket;
static struct sockaddr_in tello_cmd_addr;

static void wpa_cli_msg_cb(char *msg, size_t len)
{
  printf("call back \n");
  printf("%s\n", msg);
}

int wpa_cmd(char const *cmd, char *buf)
{
  int result;
  size_t len;
  size_t cmd_len;

  printf("wpa_cmd: %s\n", cmd);
  cmd_len = (size_t)strlen(cmd);
  printf("cmd_len %zu\n", cmd_len);
  len = BUFSIZE;
  result = wpa_ctrl_request(ctrl_conn,
                            cmd,
                            cmd_len,
                            buf,
                            &len,
                            wpa_cli_msg_cb);
  printf("len %zu\n", len);
  buf[len] = '\0';
  printf("%s\n", buf);

  if (result >= 0)
    return len;
  return result;
}

char *tellos_find(void)
{
  char *tello = NULL;
  char *tello_found;
  char buf[BUFSIZE];

  struct timespec sleep;
  struct timespec sleep_left;

  printf("looking for Tellos\n");

  // scan the network
  // note: use the even network to speed it up
  //

  wpa_cmd("SCAN", buf);

  sleep.tv_sec = 2;
  sleep.tv_nsec = 0;
  nanosleep(&sleep, &sleep_left);

  wpa_cmd("SCAN_RESULTS", buf);

  tello_found = strstr(buf, "TELLO-");

  if (tello_found != NULL)
  {
    tello = (char *)malloc(TELLO_LENGTH);
    strncpy(tello, tello_found, 12);
    tello[12] = '\0';
    printf("tello found - %s\n", tello);
    return tello;
  }
  else
    printf("tello not found\n");

  return tello;
}

#define SSID_CMD_SIZE 50

int tello_connect(char const *tello)
{
  char buf[BUFSIZE];

  char tello_ssid[SSID_CMD_SIZE];

  sprintf(tello_ssid, "SET_NETWORK 0 ssid \"%s\"", tello);
  wpa_cmd("LIST_NETWORKS", buf);

  wpa_cmd("ADD_NETWORK", buf);

  wpa_cmd(tello_ssid, buf);

  wpa_cmd("SET_NETWORK 0 key_mgmt NONE", buf);

  wpa_cmd("ENABLE_NETWORK 0", buf);

  if ((tello_cmd_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    return -1;
  }
  return 0;
}

int tello_send(char const *command)
{
  size_t cmd_len;
  char buf[BUFSIZE];
  int n;
  socklen_t len;
  int max_fd = tello_cmd_socket + 1;

  fd_set rfds;
  struct timeval tv;
  int retval;

  tello_cmd_addr.sin_family = AF_INET;
  tello_cmd_addr.sin_port = htons(TELLO_CMD_PORT);
  tello_cmd_addr.sin_addr.s_addr = inet_addr("192.168.10.1");
  printf("Command: %s\n", command);

  cmd_len = (size_t)strlen(command);

  sendto(tello_cmd_socket, command, cmd_len,
         MSG_CONFIRM, (const struct sockaddr *)&tello_cmd_addr,
         sizeof(tello_cmd_addr));
  // make sure the response comes back within
  // a given time

  FD_ZERO(&rfds);
  FD_SET(tello_cmd_socket, &rfds);

  /* Wait up to .2 seconds. */

  tv.tv_sec = 10;
  tv.tv_usec = 0;

  retval = select(max_fd, &rfds, NULL, NULL, &tv);
  printf("retval ; %d\n", retval);
  fflush(NULL);

  if (retval == -1)
  {
    perror("select()");
    fflush(NULL);
    return (-1);
  }

  if (retval == 0)
  {
    printf("send_to_tello - timeout\n");
    fflush(NULL);
    return (-2);
  }

  n = recvfrom(tello_cmd_socket, (char *)buf, BUFSIZE,
               MSG_WAITALL, (struct sockaddr *)&tello_cmd_addr,
               &len);
  buf[n] = '\0';

  printf("Tello: %s\n", buf);
  fflush(NULL);
  return 0;
}

int wpa_sup_connect()
{
  ctrl_conn = wpa_ctrl_open(CONFIG_CTRL_IFACE_DIR);
  mon_conn = wpa_ctrl_open(CONFIG_CTRL_IFACE_DIR);

  if (ctrl_conn == NULL)
  {
    printf("failed to open wpa_supplicant\n");
    return -1;
  }
  else
    printf("wpa_supplicant opened\n");

  wpa_ctrl_attach(mon_conn);

  // wpa_ctrl_get_fd(mon_conn),
  // wpa_cli_mon_receive

  return 0;
}
