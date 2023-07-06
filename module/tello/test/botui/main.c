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

#include <kipr/tello/tello.h>

int main(void)
{
	int result;
	char buf[TELLO_BUFSIZE];
	int len;
	int n;
	struct tello_ssid * tellos;
	struct tello_info tello;
	int send_result;

	if (wpa_sup_connect() == -1)
	{
		printf ("failed to open wpa_supplicant\n");
		return -1;
	}
	else
		printf ("wpa_supplicant opened\n");

	tellos = tellos_find();

	n = 0;
	printf("List of Tellos\n");

	while(tellos[n].ssid[0] != '\0')
	{
		printf ("Tellos: %s\n", tellos[n].ssid);
		n++;
	}
	fflush(NULL);

	// for this example select only the first Tello in the list

	tello_connect(&tello, tellos[0].ssid);

	do 
	{	
		send_result = tello_send(&tello, "command");
	} while(send_result != 0);

	// tell tello to send video
	tello_send(&tello, "streamon");
	
	while(1)
	{
 		// loop forever so botui can get video
 		struct timespec sleep;
        	struct timespec sleep_left;

    		sleep.tv_sec = 0;
        	sleep.tv_nsec = 20000;
        	nanosleep(&sleep, &sleep_left);
	}
	return 0;
}

