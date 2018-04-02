#include "chatsrv.h"

#include <loop.h>
#include <inetaddr.h>

#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/time.h>

#include <functional>

using namespace std::placeholders;

ChatSrv::ChatSrv(Loop &lp, const InetAddr &addr)
	: Server(lp, addr)
{ 
//	loop_.setMessageCallback(std::bind(&ChatSrv::onMessage, this, _1, _2));
//	loop_.runAfter(3, std::bind(&ChatSrv::print, this));
}

void ChatSrv::print()
{
/*	time_t endTime = ::time(NULL);
	double time = endTime - startTime_;
	
	std::cout << nrecv_ / time 
			  << " bytes/s" << std::endl;

	startTime_ = endTime;
	nrecv_ = 0;
	loop_.runAfter(3, std::bind(&ChatSrv::print, this));*/
}

void ChatSrv::onMessage(const ConnectionPtr& conn, Buffer &buf)
{
	int connFd = conn->fd();

//	int flag = ::fcntl(connFd, F_GETFL, 0);
//	::fcntl(connFd, F_SETFL, flag | O_NONBLOCK);

	if (::signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		std::cerr << "Signal() system call error: "
				  << strerror(errno) << std::endl;

	while (buf.readableSize() >= 16)
	{
		std::string msg(buf.retrieveData(16));
		auto nrecv = msg.size();
		char sendBuf[nrecv];
		int i = 0;
		for (auto c : msg)
			sendBuf[i++] = c;

		struct timeval tv;
		if (::gettimeofday(&tv, NULL) == -1)
			std::cerr << "gettimeofday: " 
					  << strerror(errno) << std::endl;
		long tm = tv.tv_usec;
		::memcpy(sendBuf + nrecv / 2, &tm, sizeof(long));

		ssize_t nsend = ::send(connFd, sendBuf, nrecv, 0);
		if (nsend == -1)
		{
			if (errno == EPIPE)
			{
				std::cout << "Connection#" << conn->getConnNo() << " - "
					      << conn->peerAddr().toAddrStr() << " -> "
						  << conn->localAddr().toAddrStr() << " - Recv RST"
						  << " - Error: Broken pipe" << std::endl;
				conn->shutdown();
			}
		}
		else std::cout << "Connection#" << conn->getConnNo() << " - "
		    		   << conn->peerAddr().toAddrStr() << " -> "
					   << conn->localAddr().toAddrStr() << " - Recv "
					   << nrecv << (nrecv == 1 ? " byte, Send " : " bytes, Send ")
					   << nsend << (nsend == 1 ? " byte" : " bytes") << std::endl;
	}
}
	 
