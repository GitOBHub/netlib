#include <channel.h>

Channel::Channel(int fd)
	: fd_(fd) { }

Channel::~Channel()
{
//	assert(!eventHandling_);
}

void Channel::handleEvents()
{
	eventHandling_ = true;
	if ((revents_ & POLLIN) && readCallback_)
	{
		readCallback_();
	}
	if ((revents_ &  POLLOUT) && writeCallback_)
	{
		writeCallback_();
	}
	if ((revents_ &  POLLERR) && errorCallback_)
	{
		errorCallback_();
	}
	else
	{
		//TODO
	}
	eventHandling_ = false;
}
