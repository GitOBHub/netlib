#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include <netdb.h>

#include <connection.h>
#include <channel.h>
#include <inetaddr.h>
#include <socket.h>
#include <mutexlock.h>

using namespace std::placeholders;

class Buffer;

class Server
{
public:
	typedef std::map<int, ConnectionPtr> ConnectionMap;

	Server(Loop &lp, const InetAddr &addr);

	void start();
	void setMessageCallback(const MessageCallback &cb)
	{ messageCallback_ = cb; }
	void setConnectionCallback(const ConnectionCallback &cb)
	{ connectionCallback_ = cb; }

protected:
	virtual void onMessage(const ConnectionPtr& conn, Buffer &buf);
	virtual void onConnection(const ConnectionPtr& conn);
	virtual void newConnection();

	Loop &loop_;

	void removeConnection(const ConnectionPtr &conn);

	InetAddr listenAddr_;
	Socket socket_;
	int listenFd_;
	Channel channel_;
	pthread_mutex_t mutex_;
	ConnectionMap connectionMap_;
	MessageCallback messageCallback_;
	ConnectionCallback connectionCallback_;
};

#endif	//SERVER_H

