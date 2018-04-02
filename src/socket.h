//This is an internal header file, you should not include this

#ifndef SOCKET_H
#define SOCKET_H

#include <base.h>

class Socket : noncopyable
{
public:
	Socket();
	Socket(int fd)
		: fd_(fd) { }
	~Socket();

	int fd() const { return fd_; }
	void shutdownWrite();

private:
	int fd_;
};

#endif	//SOCKET_H
