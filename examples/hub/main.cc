#include "hub.h"

int main(int argc, char *argv[])
{
	uint16_t port = 5000;
	if (argc == 2)
	{
		port = static_cast<uint16_t>(atoi(argv[1]));
	}

	std::cerr << "pid = " << ::getpid()
			  << " port = " << port << std::endl;
	InetAddr listenAddr(port);
	Loop loop;
	PubSubServer server(loop, listenAddr);

	server.start();
	loop.loop();
}


