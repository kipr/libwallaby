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
		printf("WPA Supplicant not active\n");
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

	// connect to the first tello on the list
	tello_connect(&tello, tellos[0].ssid);

	// Place the tello in sdk mode. We caanot porgress ustil this is done
	// so block until complete.
	do 
	{	
		send_result = tello_send(&tello, "command");
		printf("send_result %d\n", send_result);
	
	} while(send_result != 0);

	// Turn  on video streaming from the tello
	tello_send(&tello, "streamon");

	camera_open_device_model_at_res(0, TELLO, TELLO_RES);

	// Load a color channel configuration file
	printf("load config\n");fflush(stdout);
	int ret = camera_load_config("red");
	if (ret == 1) { 
		printf("...success\n");fflush(stdout);
	}

	printf("waiting for camera_update\n");fflush(stdout);
    camera_update();
    printf("Cam width x height:  %d x %d\n", get_camera_width(), get_camera_height());fflush(stdout);

    printf("Num channels = %d\n", get_channel_count());fflush(stdout);


	// Image processing has started so now launch the tello and start blob tracking

	// Launch the tello. This cammond will not return until the tello has completed the takeoff and
	// is holding position at the defeulat altitude

	do
	{
		send_result = tello_send(&tello, "takeoff");
		if(send_result != 0)
			printf("Error received on takeoff command\n");
		        printf("send_result %d\n", send_result); fflush(stdout);
	} while(send_result != 0);
	 msleep(500);

	// Autonomous loop...
	int i;
	double start = seconds();
	const int num_imgs = 600;
	for (i = 0; i < num_imgs; ++i)
	{
		// Process a video frame
		camera_update();

		// If an object is detected, the goal is to keep the tello centered on and at a constant
		// distance from the object. We can do this by tracking the largest blob and moving the
		// tello to keep the blob's center in the center of the display

		// The tello coordinate system is a top view looking down orientation
		// a values move the tello left(-), right(+)
		// b values move the tello forward(+) and backwards(-)
		// c values increase(+) and decrease(-) the tello's altitude.
		// d values yaw cw (+) and ccw (-)

		// Blobs are tracked by size, with blob 0 being the largest blob found

		// The tello camera resolution is 1280 x 720, so the center of the image is 640, 360

		// Check if an object (blob) is dected one the zero color channel
		int obj_count = get_object_count(0);

		if (obj_count > 0)
		{
			// May want to add confidence and size checks
		
			// Retrieve blob zero's center coordinates
			int obj_center_x = get_object_center_x(0,0);
			int obj_center_y = get_object_center_y(0,0);
			
			printf("objs: %d (%d,%d)\n", obj_count, obj_center_x, obj_center_y); fflush(stdout);
	
			// Compute the blob's offset from center
			int obj_offset_x = obj_center_x - 640;
			int obj_offset_y = obj_center_y - 320;
			
			printf("offset (%d,%d)\n", obj_offset_x, obj_offset_y); fflush(stdout);

			// Compute a,c movement to keep the blob in the center of the image
			// If the blob is not in the center, we will make very small position adjustments
			// to prevent overshooting the target position
			int a_movement = 0;
			int b_movement = 0;
			int c_movement = 0;
					
			if (obj_offset_x < -64)			// obj is left of center, so move left
				a_movement = -10;
			else if (obj_offset_x > 64)		// obj is right of center, so move right
				a_movement = 10;
				
			if (obj_offset_y < -32)			// obj is below center, so descend
				c_movement = -10;
			else if (obj_offset_y > 32)		// obj is above center so ascend
				c_movement = 10;

			// Compute y movement to keep constant distance (fixed bounding box area)
			// Determine the area as a percentage of the total image size (128x720)
			float area = (float)get_object_area(0,0) / 921600.0;
			printf("area = %f\n", area); fflush(stdout);

			if (area < 0.04)				// Area is small, so move forward
				b_movement = 10;
			else if (area > 0.4)			// Area is large, so move back
				b_movement = -10;
			// else: y_movement = 0
			
			printf("movement x=%d, y=%d, z=%d\n", a_movement, b_movement, c_movement); fflush(stdout);

			sprintf(cmd_buffer, "rc %d %d %d %d", a_movement, b_movement, c_movement, 0 );
			send_result = tello_send_no_wait(&tello, cmd_buffer);
			printf("send_result %d\n", send_result); fflush(stdout);
			msleep(500);
		}
			
		else
		{
			// We do not see or lost track of the blob, so stop moving
			sprintf(cmd_buffer, "rc %d %d %d %d", 0, 0, 0, 0 );
			send_result = tello_send_no_wait(&tello, cmd_buffer);
			printf("send_result %d\n", send_result); fflush(stdout);
			msleep(500);
		}                    
	}
		
	double stop = seconds();
	printf("%f fps\n", ((double)num_imgs/(stop-start)));

    printf("Camera close\n");
    camera_close();
    close (tello_cmd_socket);
    return 0;
}

