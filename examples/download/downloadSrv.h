#ifndef DOWNLOADSRV_H
#define DOWNLOADSRV_H

#include <server.h>
#include <loop.h>

class DownloadSrv : public Server
{
public:
	DownloadSrv(Loop &lp, const InetAddr &addr)
		: Server(lp, addr) { }

	void onMessage(const ConnectionPtr &conn, Buffer &buf) override;
	void onConnection(const ConnectionPtr &conn) override;

private:

};

#endif	//DOWNLOADSRV_H

	
