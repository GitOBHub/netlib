#include <inetaddr.h>

InetAddr::InetAddr(uint16_t port)
		: port_(port) 
{ 
	struct addrinfo *res, *aip;
	struct addrinfo hints;

	snprintf(service_, 255, "%d", port_);
	::memset(&hints, 0, sizeof hints);
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int err;
	if ((err = ::getaddrinfo(NULL, service_, &hints, &res)) != 0 )
	{
		std::cerr << "getaddrinfo error: " 
				  << ::gai_strerror(err) << std::endl;
		::exit(EXIT_FAILURE);
	}
	//for (aip = res; aip != NULL; aip = aip->ai_next)
	::memcpy(&sockAddr_, res->ai_addr, sizeof sockAddr_);
	::freeaddrinfo(res);
}

InetAddr::InetAddr(char* host, char* serv) 
{ 
	struct addrinfo *res, *aip;
	struct addrinfo hints;

	strncpy(host_, host, 256);
	strncpy(service_, serv, 256);

	::memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int err;
	if ((err = ::getaddrinfo(host_, service_, &hints, &res)) != 0 )
	{
		std::cerr << "getaddrinfo error: " 
				  << ::gai_strerror(err) << std::endl;
		::exit(EXIT_FAILURE);
	}
	//for (aip = res; aip != NULL; aip = aip->ai_next)
	::memcpy(&sockAddr_, res->ai_addr, sizeof sockAddr_);
	::freeaddrinfo(res);
}

InetAddr::InetAddr(const struct sockaddr_in *sockAddr)
{
	if (::inet_ntop(AF_INET, &sockAddr->sin_addr, host_, 255) == NULL)
		std::cerr << "inet_ntop error: "
				  << ::strerror(errno) << std::endl;
		::snprintf(service_, 255, "%d", ::ntohs(sockAddr->sin_port));
}
	
//InetAddr(std::string &addr)
//	: inAddr_(addr) { }

const struct sockaddr_in *InetAddr::getSockAddrIn() const
{
	return &sockAddr_;
}

std::string InetAddr::toAddrStr() const 
{	
	std::string hostname(host_);
	std::string service(service_);
	return std::string(hostname + ':' + service);
}		
