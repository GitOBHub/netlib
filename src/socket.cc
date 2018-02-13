#include <socket.h>

#include <stdio.h>
#include <sys/socket.h>

Socket::Socket()
{
	fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ == -1)
	{
		perror("system call socket");
		assert(fd_ != -1);
	}
}
