#include <string>

class TCPClient
{
public:

	TCPClient(const std::string & address, int port);
	virtual ~TCPClient();

	bool connect();
	bool disconnect();

	bool send(const void * data, int size);
	bool receive(void * buff, int buff_size);

private:
	int sock_;
	std::string address_;
	int port_;

};
