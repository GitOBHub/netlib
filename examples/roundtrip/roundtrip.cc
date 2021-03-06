#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

#include <functional>

#include <loop.h>

#include <server.h>

using namespace std::placeholders;

class RTTServer : public Server
{
public:
	RTTServer(Loop &lp, const InetAddr &addr)
		: Server(lp, addr) { }

	void onMessage(const ConnectionPtr& conn, Buffer &buf) override
	{
		int connFd = conn->fd();

	//	int flag = ::fcntl(connFd, F_GETFL, 0);
	//	::fcntl(connFd, F_SETFL, flag | O_NONBLOCK);
	
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
		
			std::string data(sendBuf, 16);
			conn->send(data);
		}
	}
};
		 
int main()
{	
	int16_t port = 5000;
	std::cout << "pid = " << ::getpid()
		      << " - " << "port = "
			  << port << std::endl;
	InetAddr listenAddr(port);
	Loop loop;
	RTTServer server(loop, listenAddr);

	server.start();
	loop.loop();
}
