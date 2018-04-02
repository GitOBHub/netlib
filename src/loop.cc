#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/timerfd.h>
#include <string.h>

#include <iostream>

#include <loop.h>

Loop::Loop()
{
	timerFd_ = ::timerfd_create(CLOCK_REALTIME, 0);
	if (timerFd_ == -1)
		std::cerr << "timerfd_create: " 
				  << strerror(errno) << std::endl;
}

struct itimerspec newVal;
void Loop::runAfter(time_t tm, const std::function<void(void)> &func)
{
	timerCallback_ = func;

	newVal.it_value.tv_sec = tm;
	if (::timerfd_settime(timerFd_, 0, &newVal, NULL) == -1)
		std::cerr << "timerfd_settime: " 
				  << strerror(errno) << std::endl;
}

void Loop::loop()
{
	//std::cerr << "Prepare to loop" << std::endl;
	while (!quit_)
	{
		//std::cerr << "Start loop" << std::endl;
		poller_.poll(-1, &activeChannels_);
	
		//std::cerr << "Poll return back to loop" << std::endl;
		for (auto &ch : activeChannels_)
		{
			ch->handleEvents();
		}
//		std::cerr << "After handle active channels\n";
		activeChannels_.clear();
//		std::cerr << "End one time loop\n";
	}
}

void Loop::quit()
{
	quit_ = 1;
}

void Loop::updateChannel(Channel *channel)
{
	poller_.updateChannel(channel);
}

void Loop::removeChannel(Channel *channel)
{
	poller_.removeChannel(channel);
}

