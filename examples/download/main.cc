#include "downloadSrv.h"

int main()
{
	Loop loop;
	InetAddr listenAddr(5000);
	DownloadSrv downloadSrv(loop, listenAddr);

	downloadSrv.start();
	loop.loop();
}
