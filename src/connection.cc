#include <connection.h>
#include <loop.h>

Connection::Connection(int fd, Loop &lp) 
		: connFd_(fd), 
		  loop_(lp),
		  inputBuffer_(fd),
		  channel_(fd)
{
	struct sockaddr_in localAddr, peerAddr;
	socklen_t localLen = sizeof localAddr;
	socklen_t peerLen = sizeof peerAddr;
	getsockname(connFd_, (struct sockaddr *)&localAddr, &localLen);
	getpeername(connFd_, (struct sockaddr *)&peerAddr, &peerLen);
	localAddr_ = InetAddr(&localAddr);
	peerAddr_ =	InetAddr(&peerAddr);

	static int connNum = 0;
	number_ = ++connNum;
	channel_.setReadCallback(std::bind(&Connection::handleRead, this));
	channel_.enableReading();
	loop_.updateChannel(&channel_);
	//channel_.setWriteCallback(std::bind(&Connection::handleWrite, this));
	//channel_.setErrorCallback(std::bind(&Connection::handleError, this));
}

Connection::~Connection()
{
	loop_.removeChannel(&channel_);
	::close(connFd_);
}
	
void Connection::handleRead()
{
//	std::cerr << "handleRead start" << std::endl;
	ssize_t nread = inputBuffer_.readData();

//FIXME: ConnectionPtr guardThis(shared_from_this());
//	std::cerr << "nread is " << nread << std::endl;
	if (nread > 0)
	{
//		std::cerr << "append()\n";
		if (messageCallback_)
		{
//			std::cerr << "messageCallback_\n";
			messageCallback_(shared_from_this(), inputBuffer_);
		}
	}
	else if (nread == 0)
	{
		handleClose(shared_from_this());
	}
	else
	{
		//TODO
	}
//	std::cerr << "After handleClose()\n";
}

void Connection::handleClose(const ConnectionPtr &conn)
{
	shutdown();
	if (connectionCallback_)
	{
		connectionCallback_(conn);
	}
	if (closeCallback_)
	{
		//closeCallback_(guardThis);
//		std::cerr << "After closeCalback_\n";
	}
	closeCallback_(conn);
}

void Connection::shutdown()
{
	if (isConnected_)
	{
		::shutdown(connFd_, SHUT_WR);
		isConnected_ = 0;
	}
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

Buffer &Connection::getBuffer()	
{
	return inputBuffer_;
}

bool Connection::isBufferWritable()
{
	return !inputBuffer_.isBufferFull();
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
