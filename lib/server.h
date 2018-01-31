#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include <netdb.h>

#include "loop.h"
#include "inetaddr.h"

class Buffer;

class Server
{
public:
	Server(Loop &lp, const InetAddr &addr);

	void start();

protected:
	virtual void onMessage(const ConnectionPtr& conn, Buffer &buf);
	virtual void onConnection(const ConnectionPtr& conn);

	Loop &loop_;
private:
	const InetAddr &listenAddr_;
	int listenFd_;
};

#endif	//SERVER_H

