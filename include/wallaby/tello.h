
#ifndef INCLUDE_WALLABY_TELLO_H
#define INCLUDE_WALLABY_TELLO_H

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

