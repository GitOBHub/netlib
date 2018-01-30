#ifndef LOOP_H
#define LOOP_H

#include <set>
#include <functional>

#include <sys/epoll.h>

#include "connection.h"
#include "inetaddr.h"

const int maxEvents = 1024;

class Loop
{
public:
	Loop() = default;

	void loop();

	void setMessageCallback(
			const std::function<void(const ConnectionPtr&, char *)> &func);
	void setConnectionCallback(
			const std::function<void(const ConnectionPtr&)> &func);

	void setListenSocket(int fd, const InetAddr &addr) 
	{ 
		listenFd_ = fd;
		listenAddrPtr_ = &addr;
	};

private:
	int listenFd_ = -1;
	const InetAddr *listenAddrPtr_;
//	struct pollfd fds[maxEvents];
	struct epoll_event ev_, events_[maxEvents];
	std::set<ConnectionPtr> connSet_;
	int nfds = 1;
	std::function<void(const ConnectionPtr&, char *)> messageCallback_;
	std::function<void(const ConnectionPtr&)> connectionCallback_;
};

#endif	//LOOP_H
