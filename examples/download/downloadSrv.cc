#include "downloadSrv.h"

#include <memory>

void DownloadSrv::onConnection(const ConnectionPtr &conn)
{
	if (conn->connected())
	{
		std::cout << "Connection UP" << std::endl;

	}
	else 
	{
		std::cout << "Connection DOWN" << std::endl;
	}
}

void DownloadSrv::onMessage(const ConnectionPtr &conn, Buffer &buf)
{
	int connFd = conn->getConnFd();
	const int bufsize = 1000000;
	char sendBuf[bufsize];
	char filename[256];
	if (buf.readableSize() > 256)
	{
		::sprintf(sendBuf, "File name too large\n");
		if (::send(connFd, sendBuf, 20, 0) != 20)
			std::cerr << "send: " << ::strerror(errno) << std::endl;
	}
	else
	{
		std::string msg = buf.retrieveAllData();
		int i = 0;
		for (auto c : msg)
		{
			filename[i++] = c;
		}
		filename[--i] = '\0';

		FILE *fp;
		std::unique_ptr<FILE> uptr;
		uptr.reset(fp);
		if ( (fp = ::fopen(filename, "r")) == NULL)
		{
			std::cerr << "fopen: " << ::strerror(errno) << std::endl;
			::sprintf(sendBuf, "No such file\n");
			if (::send(connFd, sendBuf, 13, 0) != 13)
				std::cerr << "send: " << ::strerror(errno) << std::endl;
			conn->shutdown();
			return;
		}

		int size = ::fread(sendBuf, 1, bufsize, fp);
		//size = ::fgets(sendBuf, bufsize, fp);
		//std::cout << size << std::endl;
		if (::send(connFd, sendBuf, size, 0) == -1)
				std::cerr << "send: " << ::strerror(errno) << std::endl;
		conn->shutdown();
	}
}





