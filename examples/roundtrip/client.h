#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <inetaddr.h>

class Client
{
public:
	Client(const InetAddr &);

//	void connect();

private:
	int sockFd_ = -1;
	const InetAddr &servAddr_;
};

#endif	//CLIENT_H
