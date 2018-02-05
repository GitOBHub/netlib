#ifndef LOOP_H
#define LOOP_H

#include <set>
#include <functional>

#include <sys/epoll.h>
#include <time.h>

#include "connection.h"
#include "inetaddr.h"

const int maxEvents = 1024;

class Loop
{
public:
	Loop();

	void loop();

	void setMessageCallback(
			const std::function<void(const ConnectionPtr&, Buffer&)> &func);
	void setConnectionCallback(
			const std::function<void(const ConnectionPtr&)> &func);
	void runAfter(time_t tm, const std::function<void(void)> &func);

	void setListenSocket(int fd, const InetAddr &addr) 
	{ 
		listenFd_ = fd;
		listenAddrPtr_ = &addr;
	};

private:
	int listenFd_ = -1;
	int timerFd_;
	const InetAddr *listenAddrPtr_;
	struct epoll_event ev_, events_[maxEvents];
	std::set<ConnectionPtr> connSet_;
	std::function<void(const ConnectionPtr&, Buffer&)> messageCallback_;
	std::function<void(const ConnectionPtr&)> connectionCallback_;
	std::function<void(void)> timerCallback_;
};

#endif	//LOOP_H
