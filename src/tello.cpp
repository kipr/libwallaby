#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wallaby/wpa_ctrl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <wallaby/camera.h>
#include <wallaby/util.h>
#include <wallaby/tello.h>

#define CONFIG_CTRL_IFACE_DIR "/var/run/wpa_supplicant/wlan0"

static struct wpa_ctrl * ctrl_conn;
static struct wpa_ctrl * mon_conn;

static int tello_cmd_socket;
static struct sockaddr_in tello_cmd_addr;

static void wpa_cli_msg_cb(char *msg, size_t len)
{
        printf ("call back \n");
        printf("%s\n", msg);
}

int wpa_cmd(char const * cmd, char * buf)
{
	int result;
	size_t len;
	size_t cmd_len;
	
	printf("wpa_cmd: %s\n", cmd);
	cmd_len = (size_t)strlen(cmd);
	//printf("cmd_len %d\n", cmd_len);
	len = TELLO_BUFSIZE;
	result = wpa_ctrl_request(ctrl_conn,
			cmd,
			cmd_len,
			buf,
			&len,
			wpa_cli_msg_cb);

	//printf("len %d\n", len);
	buf[len] = '\0';
	printf("%s\n", buf);

	if( result >= 0)
		return len;
	return result;
}

struct tello_ssid * tellos_find(void)
{
	char * tello = NULL;
	char * tello_found;
	char buf[TELLO_BUFSIZE];

	struct tello_ssid * tello_ssid;

	char * ssid_found = NULL;
	uint wpa_index;

	char bss_cmd[12];

	struct timespec sleep;
        struct timespec sleep_left;

	printf("tello_find - looking for Tellos\n");

	tello_ssid = (struct tello_ssid *) calloc(1, sizeof(struct tello_ssid));

	// scan the network
	// note: use the even network to speed it up
	//

	wpa_cmd ("SCAN", buf);

        sleep.tv_sec = 2;
        sleep.tv_nsec = 0;
        nanosleep(&sleep, &sleep_left);
	wpa_index = 0;

	sprintf(bss_cmd, "BSS RANGE=ALL MASK=%x", WPA_BSS_MASK_SSID);

	wpa_cmd(bss_cmd, buf);
	// see if there is an ssid
	ssid_found = strstr(buf, "ssid");

	if (ssid_found == NULL)
	{
		printf("tellos_find - no ssids found\n");
		return tello_ssid;    // we are done
	}

while(1)
{
	int tello_size;
	tello_found = strstr(ssid_found, "TELLO-");

	if(tello_found == NULL)
		break;

	tello_size = strnlen(tello_found, TELLO_SSID_LENGTH);
	if( tello_size > TELLO_SSID_LENGTH - 1)
		tello_size = TELLO_SSID_LENGTH - 1;

        strncpy(tello_ssid[wpa_index].ssid, tello_found, tello_size);
	tello_ssid[wpa_index].ssid[tello_size] = '\0';
	printf("tello found - %s\n", tello_ssid[wpa_index].ssid);
	wpa_index++;

	tello_ssid = (struct tello_ssid *) realloc((void *) tello_ssid, sizeof(struct tello_ssid)*(wpa_index + 1));
	memset(&tello_ssid[wpa_index], 0, sizeof(struct tello_ssid));

	ssid_found = tello_found + tello_size;
}
	return tello_ssid;
}

#define SSID_CMD_SIZE 50

int tello_connect(char const * tello)
{
	char buf[TELLO_BUFSIZE];

	char tello_ssid[SSID_CMD_SIZE];

	sprintf(tello_ssid, "SET_NETWORK 0 ssid \"%s\"", tello);
	wpa_cmd ("LIST_NETWORKS", buf);

	wpa_cmd ("ADD_NETWORK", buf);

        wpa_cmd (tello_ssid, buf);

        wpa_cmd ("SET_NETWORK 0 key_mgmt NONE", buf);

        wpa_cmd ("ENABLE_NETWORK 0", buf);

	if ( (tello_cmd_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
        {
                perror ("socket creation failed");
                return -1;
        }
	return 0;
}

int tello_send(char const * command)
{
	return tello_send_wait(command, TELLO_TIMEOUT);
}

int tello_send_no_wait(char const * command)
{
	return tello_send_wait(command, 0);
}

int tello_send_wait(char const * command, int wait)
{
	size_t cmd_len;
	char buf[TELLO_BUFSIZE];
	int n;
	socklen_t len;
	int max_fd = tello_cmd_socket + 1;

        fd_set rfds;
        struct timeval tv;
        int retval;

	tello_cmd_addr.sin_family = AF_INET;
        tello_cmd_addr.sin_port = htons (TELLO_CMD_PORT);
        tello_cmd_addr.sin_addr.s_addr = inet_addr("192.168.10.1");
	printf("Command: %s\n", command);

        cmd_len = (size_t)strlen(command);

        sendto (tello_cmd_socket, command, cmd_len,
                MSG_CONFIRM, (const struct sockaddr *) &tello_cmd_addr,
                        sizeof (tello_cmd_addr));
	// make sure the response comes back within
	// a given time

	FD_ZERO(&rfds);
        FD_SET(tello_cmd_socket, &rfds);

        /* Wait up to .2 seconds. */

           tv.tv_sec = wait;
           tv.tv_usec = 10000;

           retval = select(max_fd, &rfds, NULL, NULL, &tv);
	printf("retval ; %d\n", retval); fflush(NULL);
	
	if (retval == -1)
	{
		perror("select()"); fflush(NULL);
		return (-1);
	}

	if (retval == 0)
	{
		if (wait !=0)
		{
			printf("send_to_tello - timeout\n"); fflush(NULL);
			return (-2);
		}
		else
			return 0; // if not waiting - then okay 
	}

        n = recvfrom ( tello_cmd_socket, (char *) buf, TELLO_BUFSIZE,
                        MSG_WAITALL, (struct sockaddr *) &tello_cmd_addr,
                        &len);
        buf[n] = '\0';

        printf ("Tello: %s\n", buf); fflush(NULL);

	if(strncmp(buf, "ok", n) == 0)
		return 0;
	if(strncmp(buf, "error", n) == 0)
		return 1;
	else
		return 2;
}

int wpa_sup_connect()
{
	ctrl_conn = wpa_ctrl_open (CONFIG_CTRL_IFACE_DIR);
        mon_conn = wpa_ctrl_open (CONFIG_CTRL_IFACE_DIR);

        if (ctrl_conn == NULL)
        {
                printf ("failed to open wpa_supplicant\n");
                return -1;
        }
        else
                printf ("wpa_supplicant opened\n");

        wpa_ctrl_attach (mon_conn);

        //wpa_ctrl_get_fd(mon_conn),
        //wpa_cli_mon_receive
	
	return 0;
}
