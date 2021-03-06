//This is a public header file

#ifndef LOOP_H
#define LOOP_H

#include <set>
#include <functional>

#include <sys/epoll.h>
#include <time.h>

#include <poller.h>
#include <channel.h>
#include <noncopyable.h>

//This is an interface class, so don't expose too much details
class Loop : noncopyable
{
public:
	Loop();

	void loop();
	void quit();

	void runAfter(time_t tm, const std::function<void(void)> &func);

	void updateChannel(Channel *channel);
	void removeChannel(Channel *channel);

private:
	Poller poller_;
	ChannelList activeChannels_;
	int listenFd_ = -1;
	int timerFd_;
	int quit_ = 0;
	std::function<void(void)> timerCallback_;
};

#endif	//LOOP_H
