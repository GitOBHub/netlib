#include <server.h>

#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include <functional>
#include <iostream>

Server::Server(Loop &lp, const InetAddr &addr)
	: loop_(lp), listenAddr_(addr)
{ 
	loop_.setMessageCallback(std::bind(&Server::onMessage, this, _1, _2));
	loop_.setConnectionCallback(std::bind(&Server::onConnection, this, _1));
}


void Server::onMessage(const ConnectionPtr& conn, Buffer &buf)
{
	ssize_t nrecv = 4;
	int connFd = conn->getConnFd();
	
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
	if (conn->connected())
		std::cout << "New connection - " << conn->getPeerAddr().toAddrStr()
			      << " -> " << conn->getLocalAddr().toAddrStr() << std::endl;
	else
		std::cout << "Connection#" << conn->getConnNo() << " - "
	              << conn->getPeerAddr().toAddrStr() << " -> "
		          << conn->getLocalAddr().toAddrStr()
		          << " - Done " << std::endl;
}

void Server::start()
{
	int on = 1;

	if ( (listenFd_ = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		//
	}

	if (::setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		//
	}

	const struct sockaddr_in *sockAddrPtr = listenAddr_.getSockAddrIn();
	if (::bind(listenFd_, (const struct sockaddr *)sockAddrPtr, 
			   sizeof(struct sockaddr_in)) == -1)
	{
		//
	}

	if (::listen(listenFd_, 10) == -1)
	{
		//
	}  
	
	loop_.setListenSocket(listenFd_, listenAddr_);
}

