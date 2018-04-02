#include <server.h>
#include <loop.h>

#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include <functional>
#include <iostream>
#include <cassert>

Server::Server(Loop &lp, const InetAddr &addr)
	: loop_(lp), 
	  listenAddr_(addr), 
	  listenFd_(socket_.fd()),
	  channel_(listenFd_)
{ 

}


void Server::onMessage(const ConnectionPtr& conn, Buffer &buf)
{
	ssize_t nrecv = 4;
	int connFd = conn->fd();
	
//	int flag = ::fcntl(connFd, F_GETFL, 0);
//	::fcntl(connFd, F_SETFL, flag | O_NONBLOCK);

	while (buf.readableSize() >= 4)
	{
		std::string msg = buf.retrieveData(4);

		msg.append("\n");
		conn->send(msg);
	}
}

void Server::onConnection(const ConnectionPtr &conn)
{
	if (conn->isConnected())
		std::cout << "New connection - " << conn->peerAddr().toAddrStr()
			      << " -> " << conn->localAddr().toAddrStr() << std::endl;
	else
		std::cout << "Connection#" << conn->number() << " - "
	              << conn->peerAddr().toAddrStr() << " -> "
		          << conn->localAddr().toAddrStr()
		          << " - Done " << std::endl;
}

void Server::start()
{
	int on = 1;

	if (::setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		//
	}

	const struct sockaddr_in *sockAddrPtr = listenAddr_.getSockAddrIn();
	if (::bind(listenFd_, (const struct sockaddr *)sockAddrPtr, 
			   sizeof(struct sockaddr_in)) == -1)
	{
		perror("system call bind");
	}

	if (::listen(listenFd_, 10) == -1)
	{
		//
	}  
	
	channel_.setReadCallback(std::bind(&Server::newConnection, this));
	channel_.enableReading();
	loop_.updateChannel(&channel_);
}

void Server::newConnection()
{
	int connFd = ::accept4(listenFd_, NULL, NULL, SOCK_NONBLOCK | SOCK_CLOEXEC);
	if (connFd == -1)
	{
		//TODO:
	}
	auto conn = std::make_shared<Connection>(connFd, loop_);
	conn->setMessageCallback(messageCallback_ 
							 ? messageCallback_
							 : std::bind(&Server::onMessage, this, _1, _2));
	conn->setConnectionCallback(connectionCallback_
							    ? connectionCallback_
								: std::bind(&Server::onConnection, this, _1));
	conn->setCloseCallback(std::bind(&Server::removeConnection, this, _1));
	connectionMap_[conn->number()] = conn;
	if (connectionCallback_)
	{
		connectionCallback_(conn);
	}
	else
	{
		onConnection(conn);
	}
}

void Server::removeConnection(const ConnectionPtr &conn)
{
	auto n = connectionMap_.erase(conn->number());
	assert(n == 1);
	//FIXME: Connection and its data members like Channel was deleted
}

	

