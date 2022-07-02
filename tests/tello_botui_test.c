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

#include <wallaby/tello.h>

static int tello_cmd_socket;
static struct sockaddr_in tello_cmd_addr;

int main(void)
{
	int result;
	char buf[TELLO_BUFSIZE];
	int len;
	int n;
	struct tello_ssid * tellos;
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

	tello_connect(tellos[0].ssid);

	if ( (tello_cmd_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		perror ("socket creation failed");
		exit(EXIT_FAILURE);
	}

	do 
	{	
		send_result = tello_send("command");
	} while(send_result != 0);

	// tell tello to send video
	tello_send("streamon");
	
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

