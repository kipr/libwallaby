#ifndef _KIPR_TELLO_TELLO_H_
#define _KIPR_TELLO_TELLO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define TELLO_SSID_LENGTH 13
#define TELLO_BUFSIZE 1024
#define TELLO_CMD_PORT 8889
#define TELLO_STATE_PORT 8890
#define TELLO_VIDEO_PORT 11111
#define TELLO_TIMEOUT 2000
#define TELLO_SUBNET "192.168.10"

#define NET_WAIT_COUNT 30
#define NET_WAIT_TIME  1000
#define TELLO_WAIT 15000

/* holds information about each tello that is received
   from the wpa SCAN.
*/

struct tello_ssid { char ssid[TELLO_SSID_LENGTH]; };

/* Data structures that are unique to each tello.
*/

struct tello_info { 
		char ssid[TELLO_SSID_LENGTH];
		struct sockaddr_in tello_cmd_addr;
		int tello_cmd_socket;
		};

/* uses wpa SCAN to find tellos within its range
   returns a packed data structure tello_ssid
*/

struct tello_ssid * tellos_find();

/* uses a tello ssid to connect to an individual tello.
   does other administrative setup as well
*/

int  tello_connect(struct tello_info *, char const * tello_ssid);

/* sends a command to the tello represented by tello_info
   This command will wait for the tello to respond within 
   5 seconds
*/

int tello_send(struct tello_info * tello, char const * command);

/* sends a command to the tello represented by tello_info
   will wait for the tello to respond within 'seconds'
*/

int tello_send_wait(struct tello_info *, char const * command, int seconds);

/* sends a command the tello represented by tello_info and does 
   not wait.  Supports the undocumented (in v2 but documented in v3)
   RC no wait command.
*/
int tello_send_no_wait(struct tello_info *, char const * command);

/* base tello_send function */
int tello_send_wait_noretry(struct tello_info * tello, char const * command, int wait);

/* return mesages from the Tello */

#define TELLO_RETURN_OK                 0
#define TELLO_RETURN_GARBAGE           -1       /* usually happens when command is sent */
#define TELLO_RETURN_ERROR             -2       /* error with no description */ 
#define TELLO_RETURN_TIMEOUT           -3
#define TELLO_RETURN_CMD_NOT_COMPLETE  -4       /* previous command not complete */
#define TELLO_RETURN_INSUFFICIANT_LIGHT -5       /* need more light for position tracking */
#define TELLO_RETURN_UNKNOWN           -99      /* unkown tello error */


/* connects the user program to wpa_suppplicant */
int wpa_sup_connect();

int wpa_cmd(char const  * cmd, char * buf);

#ifdef __cplusplus
}
#endif
#endif

