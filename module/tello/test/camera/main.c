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

static int tello_cmd_socket;
static struct sockaddr_in tello_cmd_addr;

int main(void)
{
	int result;
	char buf[TELLO_BUFSIZE];
	int len;
	int n;
	struct tello_ssid * tellos;

	struct tello_info tello;
	int send_result;

	if( wpa_sup_connect() == -1)
	{
		printf("WPA Supplicant not active\n");
		return -1;
	}
	tellos = tellos_find();

	printf("List of Tellos:\n");
	n = 0;

	while(tellos[n].ssid[0] != '\0')
	{
		printf("Tellos: %s\n", tellos[n].ssid);
		n++;
	}

	// connect to the first tello on the list
	tello_connect(&tello, tellos[0].ssid);

	//wpa_cmd ("LIST_NETWORKS", buf);

	do 
	{	
		send_result = tello_send(&tello, "command");
		printf("send_result %d\n", send_result);
	
	} while(send_result != 0);

	tello_send(&tello, "streamon");

	camera_open_device_model_at_res(0, TELLO, HIGH_RES);

	printf("load config\n");fflush(NULL);

	int ret = camera_load_config("1");
	if (ret == 1) printf("...success\n");
	printf("waiting for camera_update\n");fflush(NULL);
	camera_update();
	printf("Cam width x height:  %d x %d\n",
		get_camera_width(), get_camera_height());fflush(NULL);

	printf("Num channels = %d\n", get_channel_count());

	int i;
	double start = seconds();
	const int num_imgs = 600;

	for (i = 0; i < num_imgs; ++i)
	{
		camera_update();
		int x = get_object_count(0) > 0 ? get_object_center_x(0,0) : -1;
		int y = get_object_count(0) > 0 ? get_object_center_y(0,0) : -1;
		printf("%f objs: %d  (%d,%d)\n", seconds(), get_object_count(0), x, y);
		fflush(NULL);
	}
	double stop = seconds();

	printf("%f fps\n", ((double)num_imgs/(stop-start)));

	printf("Camera close\n");
	camera_close();
	close (tello_cmd_socket);
	return 0;
}

