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
#include <ifaddrs.h>
#include <netdb.h>

#include <kipr/camera/camera.h>
#include <kipr/time/time.h>
#include <kipr/tello/tello.h>

#define CONFIG_CTRL_IFACE_DIR "/var/run/wpa_supplicant/wlan0"

static struct wpa_ctrl * ctrl_conn;
static struct wpa_ctrl * mon_conn;

//static int tello_cmd_socket;
//static struct sockaddr_in tello_cmd_addr;

static void wpa_cli_msg_cb(char *msg, size_t len)
{
        printf ("call back \n");
        printf("%s\n", msg);
}

int check_for_address(const char * address24);

int wait_for_address(const char * address24)
{
    int address24_size = strlen(address24);
    int ret_val = -1;
    int net_check_count = 0;

    while((ret_val == -1)||(net_check_count < NET_WAIT_COUNT))
    {
	ret_val = check_for_address(address24);
	if(ret_val == 0)
		break;
	net_check_count++;
	msleep(1000);
    }
    if(ret_val != 0)
    {
        printf("**ERROR** - no address established, check drone (%d)\n", net_check_count); fflush(NULL);
        ret_val = -1;
    }
    return ret_val;
}

int check_for_address(const char * address24)
{
    int address24_size = strlen(address24);
    int ret_val = 0;
    int net_check_count = 0;
    struct ifaddrs *addresses;
    if (getifaddrs(&addresses) == -1)
    {
        printf("wait_for_address - getifaddrs call failed\n");
        return -1;
    }

    struct ifaddrs *address = addresses;

    while(address)
    {
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6)
        {
            char ap[100];
            const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(address->ifa_addr,family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            if(strncmp(address24, ap, address24_size) == 0)
            {
               printf("Network Address Established:\n");
                printf("%s\t", address->ifa_name);
                printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");
                printf("\t%s (%d)\n", ap, net_check_count); fflush(NULL);
		ret_val = 0;
		goto done;
            }
        }
        address = address->ifa_next;
    }
    ret_val = -1;
done:
    freeifaddrs(addresses);
    return ret_val;
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

	sprintf(bss_cmd, "BSS RANGE=ALL MASK=%x", WPA_BSS_MASK_SSID|WPA_BSS_MASK_LEVEL);

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

int tello_connect(struct tello_info * tello, char const * tello_ssid)
{
	int ret_val = 0;

	char buf[TELLO_BUFSIZE];

	char tello_ssid_cmd[SSID_CMD_SIZE];

	printf("tello_connect - connecting to %s\n", tello_ssid);

	memcpy((void *) tello->ssid, tello_ssid, TELLO_SSID_LENGTH);

	sprintf(tello_ssid_cmd, "SET_NETWORK 0 ssid \"%s\"", tello_ssid);
	wpa_cmd ("LIST_NETWORKS", buf);

	if(check_for_address(TELLO_SUBNET) != 0)
	{
		printf("tello_connect - network not there, connect to it\n");
		wpa_cmd ("ADD_NETWORK", buf);
		wpa_cmd (tello_ssid_cmd, buf);
		wpa_cmd ("SET_NETWORK 0 key_mgmt NONE", buf);
		wpa_cmd ("ENABLE_NETWORK 0", buf);

		printf("tello_connect - waiting for network address to be assigned\n");
		ret_val = wait_for_address(TELLO_SUBNET);
		if(ret_val !=0)
		{
			printf("***ERROR*** (tello_connect) Drone not connected\n");
			return -1;
		}
		printf("tello_connect - address assigned, continuing\n");
	}

	if ( (tello->tello_cmd_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		perror ("tello_connect - socket creation failed");
		return -1;
	}

	// setup the network address

	tello->tello_cmd_addr.sin_family = AF_INET;
	tello->tello_cmd_addr.sin_port = htons (TELLO_CMD_PORT);
	tello->tello_cmd_addr.sin_addr.s_addr = inet_addr("192.168.10.1");
	return 0;
}

int tello_send(struct tello_info * tello, char const * command)
{
	if(strncmp(command, "rc", 2) == 0)
		return tello_send_no_wait(tello, command);
	else
		return tello_send_wait(tello, command, TELLO_WAIT);
}

int tello_send_no_wait(struct tello_info * tello, char const * command)
{
	return tello_send_wait_noretry(tello, command, 0);
}

int tello_send_wait(struct tello_info * tello, char const * command, int wait)
{
	int retrys = (wait / TELLO_TIMEOUT);
	int retval = -1;

	while( (retval == TELLO_RETURN_TIMEOUT) ||
		(retval == TELLO_RETURN_GARBAGE))
	{
		retval = tello_send_wait_noretry(tello, command, wait);
		retrys--;
		if(retrys < 0)
		{
			printf("tello_send_wait - ERROR - command not sent\n"); fflush(NULL);
			return retval;
		}

	}
	return retval;
}

int tello_send_wait_noretry(struct tello_info * tello, char const * command, int wait)
{
	size_t cmd_len;
	char buf[TELLO_BUFSIZE];
	int n;
	socklen_t len;
	int max_fd = tello->tello_cmd_socket + 1;

        fd_set rfds;
        struct timeval tv;
        int retval;

	//tello_cmd_addr.sin_family = AF_INET;
        //tello_cmd_addr.sin_port = htons (TELLO_CMD_PORT);
        //tello_cmd_addr.sin_addr.s_addr = inet_addr("192.168.10.1");
	printf("Command: %s\n", command);

        cmd_len = (size_t)strlen(command);

        sendto (tello->tello_cmd_socket, command, cmd_len,
                MSG_CONFIRM, (const struct sockaddr *) &tello->tello_cmd_addr,
                        sizeof (tello->tello_cmd_addr));
	// make sure the response comes back within
	// a given time

	FD_ZERO(&rfds);
        FD_SET(tello->tello_cmd_socket, &rfds);

        /* setup Wait */

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
			return TELLO_RETURN_TIMEOUT;
		}
		else
			return TELLO_RETURN_OK; // if not waiting - then okay 
	}

        n = recvfrom ( tello->tello_cmd_socket, (char *) buf, TELLO_BUFSIZE,
                        MSG_WAITALL, (struct sockaddr *) &tello->tello_cmd_addr,
                        &len);
        buf[n] = '\0';

        printf ("Tello return: %s\n", buf); fflush(NULL);

	if(strncmp(buf, "ok", n) == 0)
		return TELLO_RETURN_OK;
	if(strncmp(buf, "error", 5) == 0)
	{
		if(strncmp(buf, "error Not joystick", n))
		{
			printf("TELLO ERROR - command before %s not complete\n",
				command);
			return TELLO_RETURN_CMD_NOT_COMPLETE;
		}
		if(strncmp(buf, "error No valid imu", n))
		{
			printf("TELLO ERROR - not enough light for position tracking\n");
			return TELLO_RETURN_INSUFFICIANT_LIGHT;
		}

		printf("TELLO ERROR\n");
		return TELLO_RETURN_ERROR;
	}

	printf("send to tello - tello return unknown\n");
	return TELLO_RETURN_UNKNOWN;

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
