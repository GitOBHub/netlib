#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>

#include <base.h>

#include <poll.h>

class Channel : noncopyable
{
public:
	typedef std::function<void(void)> ReadCallback;
	typedef std::function<void(void)> WriteCallback;
	typedef std::function<void(void)> ErrorCallback;

	Channel(int fd);
	~Channel();

	int fd() const { return fd_; } 
	void setFd(int fd) { fd_ = fd; }
	int events() const { return events_; }
	void setEvents(int events) { events_ = events; }
	void setRevents(int revents) { revents_ = revents; }
	int index() const { return index_; }
	void setIndex(int index) { index_ = index; }
	bool isNoneEvent() const { return events_ == 0; }
	void enableReading() { events_ |= POLLIN; }
	void enableWriting() { events_ |= POLLOUT; }
	void disableWriting() { events_ &= ~POLLOUT; }
	bool isWriting() const { return events_ & POLLOUT; }

	void setReadCallback(const ReadCallback &cb)
	{ readCallback_ = cb; }
	void setWriteCallback(const WriteCallback &cb)
	{ writeCallback_ = cb; }
	void setErrorCallback(const ErrorCallback &cb)
	{ errorCallback_ = cb; }
	void handleEvents();

private:
	int fd_;
	int events_ = 0;
	int revents_ = 0;
	int index_ = -1;
	bool eventHandling_ = 0;
	ReadCallback readCallback_;
	WriteCallback writeCallback_;
	ErrorCallback errorCallback_;
};

#endif	//CHANNEL_H
