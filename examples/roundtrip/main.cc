#include "client.h"

int main(int argc, char *argv[])
{
	if (argc == 3)
	{
		InetAddr servAddr(argv[1], argv[2]);
		Client client(servAddr);
	}

	else
	{
		std::cerr << "Usage: " << argv[0] 
				  << " host service" << std::endl;
	}
}
