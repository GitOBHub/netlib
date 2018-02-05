#include "hub.h"

PubSubServer::PubSubServer(Loop &loop, const InetAddr &listenAddr)
	: Server(loop, listenAddr), 
	  codec_(std::bind(&PubSubServer::onMessage, this, _1, _2))
{
	loop.setMessageCallback(
		std::bind(&Codec::onMessage, &codec_, _1, _2));
}

/*void onConnection(const ConnectionPtr &conn)
{

}*/

void PubSubServer::onMessage(const ConnectionPtr &conn, Buffer &buf)
{
	std::string cmd;
	std::string topic;
	std::string content;
	codec_.parseMsg(cmd, topic, content);
/*	std::cerr << "Hub: cmd is " << cmd
			  << ", topic is " << topic
			  << ", content is " << content << "\n";*/
	if (cmd == "pub")
	{
		auto it = topics_.find(topic);
		if (it != topics_.end())
		{
//			std::cerr << "Pub: topic exists\n";
			it->second.doPublish(content);
		}
		else 
		{
//			std::cerr << "Pub: no topic exists, and insert\n";
			topics_[topic] = Topic(topic);
			topics_[topic].doPublish(content);
		}
	}
	else if (cmd == "sub")
	{
		auto it = topics_.find(topic);
		if (it != topics_.end())
		{
			it->second.add(conn);
		}
		else 
		{
			conn->send("No such topic " + topic + "\r\n");
		}
	}
	else if (cmd == "unsub")
	{
		auto it = topics_.find(topic);
		if (it != topics_.end())
		{
			it->second.remove(conn);
		}
		else 
		{
			conn->send("No such topic " + topic + "\r\n");
		}
	}
	else 
	{
		conn->send("Unknown command\r\n");
	}
}
