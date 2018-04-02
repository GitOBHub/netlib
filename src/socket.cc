#include <socket.h>

#include <sys/socket.h>

Socket::Socket()
{
	fd_ = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
	if (fd_ == -1)
	{
		perror("system call socket");
		assert(fd_ != -1);
	}
}

Socket::~Socket()
{
	::close(fd_);
}

void Socket::shutdownWrite()
{
	if (::shutdown(fd_, SHUT_WR) == -1)
	{
		//FIXME
		perror("system call shutdown");
	}
}
