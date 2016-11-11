#include <string>

namespace Private
{

class TCPClient
{
public:

	TCPClient(const std::string & address, int port);
	virtual ~TCPClient();

	bool connect();
	bool disconnect();

	ssize_t send(const void * data, int size);
	ssize_t receive(void * buff, int buff_size);

private:
	int sock_;
	std::string address_;
	int port_;

};

}
