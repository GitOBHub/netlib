#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <functional>

#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>

#include <inetaddr.h>
#include <buffer.h>
#include <loop.h>
#include <channel.h>

class Connection;
class Loop;

typedef std::shared_ptr<Connection> ConnectionPtr;
typedef std::function<void(const ConnectionPtr &, Buffer &)> MessageCallback;
typedef std::function<void(const ConnectionPtr &)> ConnectionCallback;
typedef std::function<void(const ConnectionPtr &)> CloseCallback;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	Connection(int fd, Loop &lp); 
	~Connection();
	
	void shutdown();
	int number() const { return number_; };
	int getConnFd() const;
	int connected() const;
	int disconnected() const;

	const InetAddr &getPeerAddr() const;
	const InetAddr &getLocalAddr() const;
	void readDataIntoBuffer();
	Buffer &getBuffer();
	bool isBufferWritable();
	void send(const std::string &data);

	void setMessageCallback(const MessageCallback &cb)
	{ messageCallback_ = cb; }
	void setConnectionCallback(const ConnectionCallback &cb)
	{ connectionCallback_ = cb; }
	void setCloseCallback(const ConnectionCallback &cb)
	{ closeCallback_ = cb; }

	void handleRead();
	void handleClose(const ConnectionPtr &conn);

private:
	int number_;
	int connFd_;
	Loop &loop_;
	Buffer inputBuffer_;
	Channel channel_;
	bool isConnected_ = 1;
	InetAddr peerAddr_;
	InetAddr localAddr_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	CloseCallback closeCallback_;
};


#endif	//CONNECTION_H

