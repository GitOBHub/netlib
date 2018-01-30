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
}


void ChatSrv::onMessage(const ConnectionPtr& conn, char *buf)
{
	ssize_t nrecv;
	int connFd = conn->getConnFd();

	int flag = ::fcntl(connFd, F_GETFL, 0);
	::fcntl(connFd, F_SETFL, flag | O_NONBLOCK);

	if (::signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		std::cerr << "Signal() system call error: "
				  << strerror(errno) << std::endl;

	if ( ( nrecv = ::recv(connFd, buf, BUFSIZ, 0)) > 0 )
	{
		ssize_t nsend = ::send(connFd, buf, nrecv, 0);
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
					   << nrecv << (nrecv == 1 ? " byte, Send " : " bytes, Send ")
					   << nsend << (nsend == 1 ? " byte" : " bytes") << std::endl;
	}
	else
	{
		conn->shutdown();
	}
}
	 
