#include "client.h"

#include <sys/time.h>

Client::Client(const InetAddr &addr)
	: servAddr_(addr) 
//void Client::connect()
{
	if ( (sockFd_ = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
		std::cerr << "socket: " 
				  << ::strerror(errno) << std::endl;

	const struct sockaddr_in *sockAddrIn = servAddr_.getSockAddrIn();
	if (::connect(sockFd_, (const struct sockaddr *)sockAddrIn, 
				  sizeof(struct sockaddr_in)) == -1)
		std::cerr << "connect: " 
				  << ::strerror(errno) << std::endl;

	size_t len = 16;
	for (;;)
	{
		char buf[16];
		struct timeval tv;
		if (::gettimeofday(&tv, NULL) == -1)
			std::cerr << "gettimeofday: " 
					  << ::strerror(errno) << std::endl;
		long tmStart = tv.tv_usec;
		::memcpy(buf, &tmStart, sizeof(long));

		if (::write(sockFd_, buf, len) != len)
			std::cerr << "write: " 
					  << ::strerror(errno) << std::endl;

		if (::read(sockFd_, buf, len) == 0)
		{
			::shutdown(sockFd_, SHUT_WR);
			std::cerr << "Server shutdown connection first" << std::endl;
		}
		else 
		{
			if (::gettimeofday(&tv, NULL) == -1)
				std::cerr << "gettimeofday: " 
						  << ::strerror(errno) << std::endl;
			long tmEnd = tv.tv_usec;

			long servTime;
			::memcpy(&servTime, buf + len / 2, sizeof(long));
			std::cout << "Network delay(RTT) is " 
					  << tmEnd - tmStart << " usecs, " 
					  << "clock offset is " 
					  << servTime - (tmEnd + tmStart) / 2
					  << " usecs" << std::endl;
		}
		::sleep(1);
	}
}
			





