//This is an internal header file, you should not include this

#ifndef POLLER_H
#define POLLER_H

#include <map>
#include <vector>

#include <noncopyable.h>

class Channel;
struct pollfd;

typedef std::vector<Channel*> ChannelList;

class Poller : noncopyable
{
public:
	Poller() = default;
	~Poller() = default;

	void poll(int timeout, ChannelList *activeChannels);
	void updateChannel(Channel *channel);
	void removeChannel(Channel *channel);

private:
	std::map<int, Channel*> channelMap_;
	std::vector<struct pollfd> pollfds_;
};

#endif	//POLLER_H

