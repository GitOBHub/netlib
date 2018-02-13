#include <poller.h>
#include <channel.h>

#include <cassert>
#include <iostream>

#include <stdio.h>
#include <poll.h>


void Poller::poll(int timeout, ChannelList *activeChannels)
{
//	std::cerr << "Prepare to poll" << std::endl;
	int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeout);

//	std::cerr << "numEvents is " << numEvents << std::endl;
	if (numEvents == -1)
	{
		perror("System call poll");
		assert(numEvents != -1);
	}
	else if (numEvents > 0)
	{
		for (const auto &pfd : pollfds_)
		{
			if (pfd.revents > 0)
			{
				auto iter = channelMap_.find(pfd.fd);
				assert(iter != channelMap_.end());
				Channel *channel = iter->second;
				assert(channel->fd() == pfd.fd);
				channel->setRevents(pfd.revents);
				activeChannels->push_back(channel);
				if (--numEvents == 0)
					break;
			}
		}
	}
}

void Poller::updateChannel(Channel *channel)
{
	int index = channel->index();
	if (index < 0)
	{
		auto ret = channelMap_.insert({channel->fd(), channel});
		assert(ret.second);
		struct pollfd pfd;
		pfd.fd = channel->fd();
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = 0;
		int index = static_cast<int>(pollfds_.size());
		channel->setIndex(index);
		pollfds_.push_back(pfd);
		//std::cerr << "pfd.fd = " << pfd.fd << std::endl;
		//std::cerr << "pfd.events is" << (pfd.events == POLLIN ? " POLLIN" : " unknown") << std::endl;
		//std::cerr << "Now pollfd number is " << pollfds_.size() << std::endl;
	}
	else 
	{
		assert(index >= 0 && index < static_cast<int>(pollfds_.size()));
		struct pollfd &pfd = pollfds_[index];
		pfd.fd = (channel->isNoneEvent() ? -channel->fd() - 1 : channel->fd());
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = 0;
	}
}

void Poller::removeChannel(Channel *channel)
{
	auto ret = channelMap_.erase(channel->fd());
	assert(ret == 1);

	int index = channel->index();
	//TODO: if not to use static_cast?
	assert(index >= 0 && index < static_cast<int>(pollfds_.size()));
	if (index == pollfds_.size() - 1)
	{
		pollfds_.pop_back();
	}
	else
	{
		std::iter_swap(pollfds_.begin() + index, pollfds_.end() - 1);
		pollfds_.pop_back();

		int fd = pollfds_[index].fd;
		if (fd < 0)
		{
			fd = -fd - 1;
		}
		auto ch = channelMap_.find(fd);
		assert(ch != channelMap_.end());
		ch->second->setIndex(index);
	}
}
