#ifndef CHATSRV_H
#define CHATSRV_H

#include <iostream>

#include <netdb.h>

#include "loop.h"
#include "inetaddr.h"
#include "server.h"


class ChatSrv : public Server
{
public:
	ChatSrv(Loop &lp, const InetAddr &addr);

private:
	void onMessage(const ConnectionPtr& conn, Buffer &buf) override;
//	void onConnection(const ConnectionPtr& conn) override;
	void print();

	time_t startTime_ = 0;
};

#endif	//CHATSRV_H

