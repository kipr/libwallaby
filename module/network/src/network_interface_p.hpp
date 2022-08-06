#ifndef _WIN32

#ifndef _NETWORK_INTERFACE_P_HPP_
#define _NETWORK_INTERFACE_P_HPP_

namespace Private
{
	class NetworkInterface
	{
	public:
		static char *macAddress(const char *const interface);
	};
}

#endif

#endif