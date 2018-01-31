#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>

#include <sys/socket.h>
#include <stdio.h>

#include "inetaddr.h"
#include "buffer.h"

class Connection
{
public:
	Connection(int no, int conn) 
		: number_(no), connFd_(conn), recvBuf_(conn)
	{
		socklen_t localLen = sizeof localAddr_;
		socklen_t peerLen = sizeof peerAddr_;
		getsockname(connFd_, (struct sockaddr *)&localAddr_, &localLen);
		getpeername(connFd_, (struct sockaddr *)&peerAddr_, &peerLen);
	}
	
	void shutdown()
	{
		if (isConnected_)
		{
			::shutdown(connFd_, SHUT_WR);
			isConnected_ = 0;
		}
	}
	int getConnNo() { return number_; } const
	int getConnFd() { return connFd_; } const 
	int connected() { return isConnected_; } const 
	int disconnected() { return isConnected_ ? 0 : 1; } const

	InetAddr getPeerAddr() const
	{
		return InetAddr(&peerAddr_); 
	}
	InetAddr getLocalAddr() const
	{
		return InetAddr(&localAddr_); 
	}

	void readDataIntoBuffer()
	{
		if (recvBuf_.readData() == 0)
			shutdown();
	}

	Buffer &getBuffer()	
	{
		return recvBuf_;
	}

	bool isBufferWritable()
	{
		return !recvBuf_.isBufferFull();
	}

private:
	int number_;
	int connFd_;
	Buffer recvBuf_;
	bool isConnected_ = 1;
	struct sockaddr_in peerAddr_;
	struct sockaddr_in localAddr_;
};

typedef std::shared_ptr<Connection> ConnectionPtr;

#endif	//CONNECTION_H

