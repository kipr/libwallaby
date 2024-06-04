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

#include <kipr/time/time.h>
#include <kipr/camera/camera.h>
#include <kipr/tello/tello.h>

static int tello_cmd_socket;
static struct sockaddr_in tello_cmd_addr;

int main(void)
{
	// int result;
	char buf[TELLO_BUFSIZE];
	int len;
	int n;
	struct tello_ssid * tellos;
	struct tello_info tello;
	
	char cmd_buffer[128];
	int send_result;

	if( wpa_sup_connect() == -1)
	{
		printf("WPA Supplicant not active, wpa_sup_connect(): %d\n", wpa_sup_connect());
		return -1;
	}

	tellos = tellos_find();

	printf("List of Tellos:\n");
	n = 0;

	while(tellos[n].ssid[0] != '\0')
	{
		printf(" Tellos: %s\n", tellos[n].ssid);
		n++;
	}
	fflush(NULL);
	// connect to the first tello on the list
	tello_connect(&tello, tellos[0].ssid);

	do
	{
		send_result = tello_send(&tello, "takeoff");
		if(send_result != 0)
			printf("Error received on takeoff command\n");
		        printf("send_result %d\n", send_result); fflush(stdout);
	} while(send_result != 0);
	 msleep(500);

    
    do
	{
		send_result = tello_send(&tello, "land");
		if(send_result != 0)
			printf("Error received on land command\n");
		        printf("send_result %d\n", send_result); fflush(stdout);
	} while(send_result != 0);
	 msleep(500);

    
	tello_disconnect(&tello);
	
    return 0;
}

