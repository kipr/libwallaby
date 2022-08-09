#ifndef _WIN32

#include "kipr/network/socket.hpp"

#ifndef WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#endif

#include <cerrno>
#include <cstdio>
#include <cstring>

Address::Address(const char *const host, const unsigned short port)
	: m_valid(true)
{
	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	
	setHost(host);
	setPort(port);
}

Address::Address(const sockaddr_in &addr)
	: m_valid(true),
	m_addr(addr)
{
}

Address::Address()
	: m_valid(false)
{
	
}

bool Address::isValid() const
{
	return m_valid;
}

bool Address::setHost(const char *const host)
{
	if(inet_aton(host, &m_addr.sin_addr) == 0) {
		perror("inet_aton");
		return false;
	}
	
	m_valid = true;
	
	return true;
}

void Address::setPort(const unsigned short port)
{
	m_addr.sin_port = htons(port);
	m_valid = true;
}

unsigned short Address::port() const
{
	return ntohs(m_addr.sin_port);
}

const sockaddr *Address::addr() const
{
	if(!m_valid) return 0;
	return (sockaddr *)&m_addr;
}

socklen_t Address::addrLength() const
{
	return sizeof(m_addr);
}

const char *Address::ip() const
{
	return inet_ntoa(m_addr.sin_addr);
}

Socket::Socket()
	: m_fd(-1)
{
}

bool Socket::open(int domain, int type, int protocol)
{
	m_fd = ::socket(domain, type, protocol);
	return m_fd >= 0;
}

bool Socket::isOpen() const
{
	return m_fd >= 0;
}

bool Socket::setBlocking(const bool blocking)
{
	if(m_fd < 0) return false;
	
#ifdef _WIN32
	u_long arg = blocking ? 1 : 0;
	return ioctlsocket(m_fd, FIONBIO, &arg);
#else
	const int flags = fcntl(m_fd, F_GETFL);
	return fcntl(m_fd, F_SETFL, blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK)) >= 0;
#endif
}

bool Socket::setReusable(const bool reusable)
{
	if(m_fd < 0) return false;
	
	const int v = reusable ? 1 : 0;
	return setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v)) >= 0;
}

bool Socket::bind(const unsigned short port)
{
	if(m_fd < 0) return false;
	
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	return ::bind(m_fd, (sockaddr *)&addr, sizeof(addr)) >= 0;
}

bool Socket::connect(const Address &addr)
{
	if(m_fd < 0) return false;
	return ::connect(m_fd, addr.addr(), addr.addrLength()) == 0;
}

socket_fd_t Socket::fd() const
{
	return m_fd;
}

bool Socket::close()
{
	if(m_fd < 0) return false;
	bool ret = ::close(m_fd) == 0;
	m_fd = -1;
	return ret;
}

ssize_t Socket::recv(void *const buffer, const size_t length, int flags)
{
	return ::recv(m_fd, reinterpret_cast<char *>(buffer), length, flags);
}

ssize_t Socket::recvfrom(void *const buffer, const size_t length, Address &address, int flags)
{
	sockaddr_in rawAddress;
	socklen_t rawAddressLength;
	ssize_t ret = ::recvfrom(m_fd, reinterpret_cast<char *>(buffer), length, flags,
		(sockaddr *)&rawAddress, &rawAddressLength);
	address = Address(rawAddress);
	return ret;
}

ssize_t Socket::send(const void *const buffer, const size_t length, int flags)
{
	return ::send(m_fd, reinterpret_cast<const char *>(buffer), length, flags);
}

ssize_t Socket::sendto(const void *const buffer, const size_t length, const Address &dest, int flags)
{
	return ::sendto(m_fd, reinterpret_cast<const char *>(buffer), length, 0, dest.addr(), dest.addrLength());
}

Socket Socket::udp()
{
	Socket ret;
	ret.open(AF_INET, SOCK_DGRAM, 0);
	return ret;
}

Socket Socket::tcp()
{
	Socket ret;
	ret.open(AF_INET, SOCK_STREAM, 0);
	return ret;
}

#endif
