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
	void onMessage(const ConnectionPtr& conn, char *buf) override;
//	void onConnection(const ConnectionPtr& conn) override;
};

#endif	//CHATSRV_H

