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

//#include <wallaby/camera.h>
//#include <wallaby/util.h>
#include <wallaby/tello.h>

static int tello_cmd_socket;
static struct sockaddr_in tello_cmd_addr;

void swarm_server();

int main(void)
{
	int result;
	char buf[TELLO_BUFSIZE];
	int len;
	int n;
	struct tello_ssid * tellos;
	int send_result;

	if( wpa_sup_connect() == -1)
	{
		printf("WPA Supplicant not active\n");
		return -1;
	}
	tellos = tellos_find();
	n = 0;

	while(tellos[n].ssid[0] != '\0')
	{
		printf ("Tellos: %s\n", tellos[n].ssid);
		n++;
	}

	// connect to the first tello on the list
	tello_connect(tellos[0].ssid);

	do 
	{	
		send_result = tello_send("command");
		printf("send_result %d\n", send_result);
	
	} while(send_result != 0);

	while(1)
	{
		//keep the tello connected
		msleep(1000);
		tello_send("battery?");
	}
	send_to_tello("ap 0352-wombat 29d7d200");

//	printf("starting swarm server\n");
    // note: cannot start swarm server until the wombat access point
    // is brought up.  This cannot be done until after all the
    // swarm tellos have been configured as bringing up the AP
    // will prevent any further connections on the client interface
//	swarm_server();

	while(1)
	{
		msleep(100);
	}

	return 0;
}

#define MAXLINE 1024

// below is a work in progress

void swarm_server()
{
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
      
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
      
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
      
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(TELLO_CMD_PORT);
      
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
      
    int len, n;
  
    len = sizeof(cliaddr);  //len is value/resuslt
 printf("Waiting for tello to connect\n"); 
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    printf("Hello message sent.\n"); 
      
    return;
}

