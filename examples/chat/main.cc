#include "loop.h"
#include "chatsrv.h"

#include <unistd.h>

int main()
{
	int16_t port = 5000;
	std::cout << "pid = " << ::getpid()
			  << " - " << "port = " 
			  << port << std::endl;
	InetAddr listenAddr(port);
	Loop loop;
	ChatSrv chatSrv(loop, listenAddr);

	chatSrv.start();
	loop.loop();
}

