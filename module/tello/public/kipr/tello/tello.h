#ifndef _KIPR_TELLO_TELLO_H_
#define _KIPR_TELLO_TELLO_H_

#ifdef __cplusplus
extern "C" {
#endif

char * tellos_find();

int tello_connect(char const * tello);

int wpa_cmd(char const  * cmd, char * buf);

int tello_send(char const * command);

int wpa_sup_connect();

#ifdef __cplusplus
}
#endif

#endif