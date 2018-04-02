#include <connection.h>
#include <loop.h>

Connection::Connection(int fd, Loop &lp) 
		: fd_(fd), 
		  loop_(lp),
		  inputBuffer_(fd),
		  outputBuffer_(fd),
		  channel_(fd),
		  socket_(fd),
		  state_(kConnected)
{
	struct sockaddr_in localAddr, peerAddr;
	socklen_t localLen = sizeof localAddr;
	socklen_t peerLen = sizeof peerAddr;
	getsockname(fd_, (struct sockaddr *)&localAddr, &localLen);
	getpeername(fd_, (struct sockaddr *)&peerAddr, &peerLen);
	localAddr_ = InetAddr(&localAddr);
	peerAddr_ =	InetAddr(&peerAddr);

	static int connNum = 0;
	number_ = ++connNum;
	channel_.setReadCallback(std::bind(&Connection::handleRead, this));
	channel_.setWriteCallback(std::bind(&Connection::handleWrite, this));
	channel_.enableReading();
	loop_.updateChannel(&channel_);
	//channel_.setWriteCallback(std::bind(&Connection::handleWrite, this));
	//channel_.setErrorCallback(std::bind(&Connection::handleError, this));
}

Connection::~Connection()
{
	loop_.removeChannel(&channel_);
	assert(state_ == kDisconnected);
}
	
void Connection::handleRead()
{
//	std::cerr << "handleRead start" << std::endl;
	ssize_t nread = inputBuffer_.readData();

//	std::cerr << "nread is " << nread << std::endl;
	if (nread > 0)
	{
//		std::cerr << "append()\n";
		if (messageCallback_)
		{
//			std::cerr << "Connection::handleRead() - messageCallback_\n";
			messageCallback_(shared_from_this(), inputBuffer_);
		}
	}
	else if (nread == 0)
	{
		handleClose();
	}
	else
	{
		//TODO
	}
//	std::cerr << "After handleClose()\n";
}

void Connection::handleWrite()
{
	std::cerr << "handleWrite\n";
	ssize_t nsend = ::send(fd_, outputBuffer_.peek(), outputBuffer_.readableSize(), 0);
	if (nsend == -1)
	{
		//TODO
	}
	else
	{
		if (nsend == outputBuffer_.readableSize())
		{
			channel_.disableWriting();
		}
		outputBuffer_.retrieveData(nsend);
	}
}

void Connection::handleClose()
{
	assert(state_ == kConnected || state_ == kDisconnecting);
	state_ = kDisconnected;
	if (connectionCallback_)
	{
		connectionCallback_(shared_from_this());
	}
	std::weak_ptr<Connection> weakConn = shared_from_this();
	{
		auto conn = weakConn.lock();
		if (conn)
		{
			std::cerr << "Connection existent before closeCallback_()\n";
		}
	}
	closeCallback_(shared_from_this());
	auto conn = weakConn.lock();
	if (conn)
	{
		std::cerr << "Connection existent after closeCallback)()\n";
	} 
}

void Connection::shutdown()
{
	if (state_ == kConnected)
	{
		socket_.shutdownWrite();
		state_ = kDisconnecting;
	}
}

void Connection::send(const std::string &data)
{
	send(data.data(), data.size());
}

void Connection::send(const void *data, size_t len)
{
	size_t remaining = len;
	ssize_t nsend = 0;
	bool savedErrno = 0;
	const char *sendBuf = static_cast<const char *>(data);

	if (::signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		std::cerr << "signal: SIGPIPE - " 
				  << ::strerror(errno) << std::endl;

	if (!channel_.isWriting() && outputBuffer_.readableSize() == 0)
	{	
		nsend = ::send(fd_, sendBuf, remaining, 0);
		std::cerr << "Connection::send() - nsend returned by ::send() is " 
				  << nsend << std::endl;
		if (nsend == -1)
		{
			if (errno == EPIPE || errno == ECONNRESET)
			{
				savedErrno = 1;
			//	std::cerr << "Connection#" << number_ << " - "
			//			  << peerAddr_.toAddrStr() << " -> "
			//			  << localAddr_.toAddrStr() << " - Recv RST"
			//			  << " - Error: Broken pipe" << std::endl;
			}
		}
		else
		{
			remaining -= nsend;
		}
	}

	if (remaining > 0 && !savedErrno)
	{
		outputBuffer_.append(sendBuf + nsend, remaining);
		if (!channel_.isWriting())
		{
			channel_.enableWriting();
			loop_.updateChannel(&channel_);
		}
	}
}
