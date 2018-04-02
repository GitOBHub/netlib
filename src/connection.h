#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <functional>

#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>

#include <inetaddr.h>
#include <buffer.h>
#include <channel.h>
#include <socket.h>
#include <loop.h>

class Connection;
//class Loop;

typedef std::shared_ptr<Connection> ConnectionPtr;
typedef std::function<void(const ConnectionPtr &, Buffer &)> MessageCallback;
typedef std::function<void(const ConnectionPtr &)> ConnectionCallback;
typedef std::function<void(const ConnectionPtr &)> CloseCallback;

class Connection : noncopyable,
				   public std::enable_shared_from_this<Connection>
{
public:
	Connection(int fd, Loop &lp); 
	~Connection();
	
	void shutdown();
	int number() const { return number_; };
	int fd() const { return fd_; };
	bool isConnected() const { return state_ == kConnected; }

	const InetAddr &peerAddr() const { return peerAddr_; }
	const InetAddr &localAddr() const { return localAddr_; }
	Buffer &getBuffer();
	bool isBufferWritable();
	void send(const std::string &data);
	void send(const void *data, size_t len);

	void setMessageCallback(const MessageCallback &cb)
	{ messageCallback_ = cb; }
	void setConnectionCallback(const ConnectionCallback &cb)
	{ connectionCallback_ = cb; }
	void setCloseCallback(const ConnectionCallback &cb)
	{ closeCallback_ = cb; }

	void handleRead();
	void handleWrite();
	void handleClose();

private:
	enum State { kConnected, kDisconnecting, kDisconnected };
	State state_;
	int number_;
	int fd_;
	Loop &loop_;
	Buffer inputBuffer_;
	Buffer outputBuffer_;
	Channel channel_;
	Socket socket_;
	InetAddr peerAddr_;
	InetAddr localAddr_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	CloseCallback closeCallback_;
};


#endif	//CONNECTION_H

