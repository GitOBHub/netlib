#ifndef INETADDR_H
#define INETADDR_H

#include <iostream>

#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

class InetAddr
{
public:
	InetAddr() = default;

	InetAddr(uint16_t port);

	InetAddr(char* host, char* serv);

	InetAddr(const struct sockaddr_in *sockAddr);

	//InetAddr(std::string &addr)
	//	: inAddr_(addr) { }

	const struct sockaddr_in *getSockAddrIn() const;

	std::string toAddrStr() const; 

private:
	std::string inAddr_;
	uint16_t port_;
	struct sockaddr_in sockAddr_;
	char host_[256] = "0.0.0.0";
	char service_[256];
};

#endif	//INETADDR_H
