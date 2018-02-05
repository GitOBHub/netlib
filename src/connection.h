#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>

#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>

#include <inetaddr.h>
#include <buffer.h>

class Connection
{
public:
	Connection(int no, int conn); 
	
	void shutdown();
	int getConnNo() const;
	int getConnFd() const;
	int connected() const;
	int disconnected() const;

	const InetAddr &getPeerAddr() const;
	const InetAddr &getLocalAddr() const;
	void readDataIntoBuffer();
	Buffer &getBuffer();
	bool isBufferWritable();
	void send(const std::string &data);

private:
	int number_;
	int connFd_;
	Buffer recvBuf_;
	bool isConnected_ = 1;
	InetAddr peerAddr_;
	InetAddr localAddr_;
};

typedef std::shared_ptr<Connection> ConnectionPtr;

#endif	//CONNECTION_H

