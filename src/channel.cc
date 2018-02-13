#include <channel.h>

Channel::Channel(int fd)
	: fd_(fd) { }

void Channel::handleEvents()
{
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
}
