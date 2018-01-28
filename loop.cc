#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>

#include "loop.h"

void Loop::setMessageCallback(
		const std::function<void(const ConnectionPtr&, char *)> &func)
{
	messageCallback_ = func;;
}

void Loop::setConnectionCallback(
		const std::function<void(const ConnectionPtr&)> &func)
{
	connectionCallback_ = func;
}

void Loop::loop()
{
	char buf[BUFSIZ];
	int nready = 0;
	struct sockaddr_in cliAddr;
	socklen_t cliLen = sizeof cliAddr;

	int epollFd = ::epoll_create1(0);
	ev_.data.fd = listenFd_;
	ev_.events = EPOLLIN;
	if (::epoll_ctl(epollFd, EPOLL_CTL_ADD, listenFd_, &ev_) == -1)
		std::cerr << "epoll_ctl: listenFd_ - " 
				  << strerror(errno) << std::endl;

	for (;;)
	{
		nready = ::epoll_wait(epollFd, events_, maxEvents, -1);
		if (nready == -1)
			std::cerr << "epoll_wait: " 
					  << strerror(errno) << std::endl;

		for (int n = 0; n < nready; n++)
		{
			if (events_[n].data.fd == listenFd_)
			{
				int connFd;
				if ( (connFd = ::accept(listenFd_, 
					(struct sockaddr *)&cliAddr, &cliLen)) == -1 ) 
					std::cerr << "Accept: " 
							  << strerror(errno) 
							  << std::endl;
				
				static int number = 0;
				number++;
				const struct sockaddr_in *servAddr = listenAddrPtr_->getSockAddrIn();
				auto connPtr = std::make_shared<Connection>(number, connFd); 
	
				connectionCallback_(connPtr);
	
				connSet_.insert(connPtr);
				ev_.data.fd = connFd;
				ev_.events = EPOLLIN;
				if (epoll_ctl(epollFd, EPOLL_CTL_ADD, connFd, &ev_) == -1)
					std::cerr << "epoll_ctl: connFd - " 
							  << strerror(errno) << std::endl;
			}
	
			else
			{
				for (auto connPtr : connSet_) 
				{
					if (connPtr->getConnFd() == events_[n].data.fd) 
					{
						messageCallback_(connPtr, buf);
						if (connPtr->disconnected())
						{
							connectionCallback_(connPtr);
							connSet_.erase(connPtr);
						}
						break;
					}
				}
			}
		}
	}
}
			
			

