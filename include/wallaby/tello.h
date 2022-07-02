
#ifndef INCLUDE_WALLABY_TELLO_H
#define INCLUDE_WALLABY_TELLO_H

#ifdef __cplusplus
extern "C" {
#endif

#define TELLO_SSID_LENGTH 13
#define TELLO_BUFSIZE 1024
#define TELLO_CMD_PORT 8889
#define TELLO_STATE_PORT 8890
#define TELLO_VIDEO_PORT 11111

struct tello_ssid { char ssid[TELLO_SSID_LENGTH]; };

struct tello_ssid * tellos_find();

int tello_connect(char const * tello);

int wpa_cmd(char const  * cmd, char * buf);

int tello_send(char const * command);

int wpa_sup_connect();

#ifdef __cplusplus
}
#endif
#endif

