#ifndef HUB_H
#define HUB_H

#include <server.h>
#include <loop.h>
#include "codec.h"
#include "topic.h"

#include <map>

class PubSubServer
{
public:
	PubSubServer(Loop &lp, const InetAddr &addr);

	void start() { server_.start(); }

private:
//	void onConnection(const ConnectionPtr &conn) override;
	void onMessage(const ConnectionPtr &conn);

	Server server_;
	Codec codec_;
	std::map<std::string, Topic> topics_;
};

#endif	//HUB_H
