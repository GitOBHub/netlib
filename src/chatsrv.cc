#include "chatsrv.h"
#include "loop.h"
#include "inetaddr.h"

#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

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
	time_t endTime = ::time(NULL);
	double time = endTime - startTime_;
	
	std::cout << nrecv_ / time 
			  << " bytes/s" << std::endl;

	startTime_ = endTime;
	nrecv_ = 0;
	loop_.runAfter(3, std::bind(&ChatSrv::print, this));
}

//void ChatSrv::onMessage(const ConnectionPtr& conn, Buffer &buf)
//{
//	std::cout << "hi" << std::endl;
/*	int connFd = conn->getConnFd();

	int flag = ::fcntl(connFd, F_GETFL, 0);
	::fcntl(connFd, F_SETFL, flag | O_NONBLOCK);

	if (::signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		std::cerr << "Signal() system call error: "
				  << strerror(errno) << std::endl;

	if ( ( nrecv_ = ::recv(connFd, buf, BUFSIZ, 0)) > 0 )
	{
		ssize_t nsend = ::send(connFd, buf, nrecv_, 0);
		if (nsend == -1)
		{
			if (errno == EPIPE)
			{
				std::cout << "Connection#" << conn->getConnNo() << " - "
					      << conn->getPeerAddr().toAddrStr() << " -> "
						  << conn->getLocalAddr().toAddrStr() << " - Recv RST"
						  << " - Error: Broken pipe" << std::endl;
				conn->shutdown();
			}
		}
		else std::cout << "Connection#" << conn->getConnNo() << " - "
		    		   << conn->getPeerAddr().toAddrStr() << " -> "
					   << conn->getLocalAddr().toAddrStr() << " - Recv "
					   << nrecv_ << (nrecv_ == 1 ? " byte, Send " : " bytes, Send ")
					   << nsend << (nsend == 1 ? " byte" : " bytes") << std::endl;
	}
	else
	{
		conn->shutdown();
	}*/
//}
	 
