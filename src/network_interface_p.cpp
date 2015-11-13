#ifndef _WIN32

#include "network_interface_p.hpp"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>

using namespace Private;

char *NetworkInterface::macAddress(const char *const interface)
{
#ifndef __linux__
	return 0;
#else
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(fd < 0) return 0;
	
	struct ifreq ifr;
	struct ifconf ifc;
	
	char buf[1024];
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if(ioctl(fd, SIOCGIFCONF, &ifc) == -1) return 0;

	ifreq *it = ifc.ifc_req;
	const ifreq *const end = it + (ifc.ifc_len / sizeof(ifreq));

	bool success = false;
	for(; it != end; ++it) {
		printf("ifr_name: %s\n", it->ifr_name);
		strcpy(ifr.ifr_name, it->ifr_name);
		if(ioctl(fd, SIOCGIFFLAGS, &ifr) != 0) continue;
		if(ifr.ifr_flags & IFF_LOOPBACK) continue; // don't count loopback
		if(ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
			success = true;
			break;
		}
	}
	
	
	if(!success) return 0;
	
	static const char *const exampleMacAddress = "01:23:45:67:89:ab";
	const size_t macLength = strlen(exampleMacAddress);
	
	char *const ret = new char[macLength + 1];
	
	static const char hexLookup[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};
	
	char *walker = ret;
	size_t i = 0;
	while(walker <= ret + macLength) {
		const unsigned char &c = ifr.ifr_hwaddr.sa_data[i++];
		*(walker++) = hexLookup[(c & 0x0F) >> 0];
		*(walker++) = hexLookup[(c & 0xF0) >> 4];
		if(walker <= ret + macLength) *(walker++) = ':';
	}
	ret[macLength] = 0;
	
	return ret;
#endif
}

#endif