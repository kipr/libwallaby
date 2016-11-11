#include "tcp_client.hpp"


#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <string>
#include <cstdio>
#include <unistd.h>

namespace Private
{

TCPClient::TCPClient(const std::string & address, int port)
: address_(address), port_(port)
{
	sock_ = socket(AF_INET , SOCK_STREAM , 0);
	if (sock_ == -1)
	{
		perror("Could not create socket");
	}
}


TCPClient::~TCPClient()
{
	disconnect();
}

bool TCPClient::connect()
{
	struct sockaddr_in server;

	// TODO: non-IP case?

	server.sin_addr.s_addr = inet_addr( address_.c_str() );
	
	server.sin_family = AF_INET;
	server.sin_port = htons( port_ );
	
	//Connect to remote server
	if (::connect(sock_ , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return false;
	}

	return true;
}


bool TCPClient::disconnect()
{
	if (sock_) close(sock_);
	return true; // TODO
}

ssize_t TCPClient::send(const void * data, int size)
{
	ssize_t num_sent = ::send(sock_, data, size , 0);

	if( num_sent < 0)
	{
		perror("Send failed : ");
		return false;
	}
	return num_sent;
}

ssize_t TCPClient::receive(void * buff, int buff_size)
{
	ssize_t num_read = ::recv(sock_ , buff , buff_size , 0);
	if(  num_read < 0)
	{
		puts("recv failed");
	}
	return num_read;
}

}

