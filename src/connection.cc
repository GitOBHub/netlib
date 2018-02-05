#include <connection.h>

Connection::Connection(int no, int conn) 
		: number_(no), connFd_(conn), recvBuf_(conn)
{
	struct sockaddr_in localAddr, peerAddr;
	socklen_t localLen = sizeof localAddr;
	socklen_t peerLen = sizeof peerAddr;
	getsockname(connFd_, (struct sockaddr *)&localAddr, &localLen);
	getpeername(connFd_, (struct sockaddr *)&peerAddr, &peerLen);
	localAddr_ = InetAddr(&localAddr);
	peerAddr_ =	InetAddr(&peerAddr);
}
	
void Connection::shutdown()
{
	if (isConnected_)
	{
		::shutdown(connFd_, SHUT_WR);
		isConnected_ = 0;
	}
}

int Connection::Connection::getConnNo() const
{
	return number_; 
}

int Connection::getConnFd() const 
{
	return connFd_; 
} 
	
int Connection::connected() const
{
	return isConnected_; 
}


int Connection::disconnected() const
{
	return isConnected_ ? 0 : 1; 
}

const InetAddr &Connection::getPeerAddr() const
{
	return peerAddr_; 
}

const InetAddr &Connection::getLocalAddr() const
{
	return localAddr_; 
}

void Connection::readDataIntoBuffer()
{
	if (recvBuf_.readData() == 0)
		shutdown();
}

Buffer &Connection::getBuffer()	
{
	return recvBuf_;
}

bool Connection::isBufferWritable()
{
	return !recvBuf_.isBufferFull();
}

void Connection::send(const std::string &data)
{
	int sendLen = 0;
	char sendBuf[BUFSIZ];
	for (auto c : data)
	{
		sendBuf[sendLen++] = c;
	}

	if (::signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		std::cerr << "signal: SIGPIPE - " 
				  << ::strerror(errno) << std::endl;

	ssize_t nsend = ::send(connFd_, sendBuf, sendLen, 0);
	if (nsend == -1)
	{
		if (errno == EPIPE)
		{
			std::cout << "Connection#" << number_ << " - "
					  << peerAddr_.toAddrStr() << " -> "
					  << localAddr_.toAddrStr() << " - Recv RST"
					  << " - Error: Broken pipe" << std::endl;
			shutdown();
		}
	}
	else std::cout << "Connection#" << number_ << " - "
	 	           << peerAddr_.toAddrStr() << " -> "
			       << localAddr_.toAddrStr()
				   << " - Send " << nsend 
				   << (nsend == 1 ? " byte" : " bytes") 
				   << std::endl;
}
