//This is an internal header file, you should not include this

#ifndef SOCKET_H
#define SOCKET_H

#include <base.h>

class Socket : noncopyable
{
public:
	Socket();

	int fd() const { return fd_; }

private:
	int fd_;
};

#endif	//SOCKET_H
