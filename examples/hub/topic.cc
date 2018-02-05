#include "topic.h"

void Topic::add(const ConnectionPtr &conn)
{
	for (auto entryPtr : connections_)
	{
		if (conn == entryPtr->weakConn_.lock())
		{
/*			std::cerr << "Topic " << name_ << ": connection#" 
					  << conn->getConnNo() << " exists\n";*/
			conn->send("Subscribed\r\n");
			return;
		}
	}
	EntryPtr entryPtr(new Entry(conn));
	connections_.insert(entryPtr);
	conn->send(name_ + ": " + content_ + "\r\n");
/*	std::cerr << "Topic " << name_ << ": Add connection#" 
			  << conn->getConnNo() << ", now exists " 
			  << connections_.size() << " connections\n";*/
}

void Topic::remove(const ConnectionPtr &conn)
{
	for (auto it = connections_.begin(); it != connections_.end(); ++it)
	{
		if (conn == (*it)->weakConn_.lock())
		{
			connections_.erase(it);
			break;
		}
	}
/*	std::cerr << "Unsub: remove connection#" 
			  << conn->getConnNo() << ", now exists " 
			  << connections_.size() << " connectionis\n";*/
}

void Topic::doPublish(const std::string &content)
{
	content_ = content;
	for (auto it = connections_.begin(); it != connections_.end();)
	{
		auto conn = (*it)->weakConn_.lock();
		if (conn)
		{
			conn->send(name_ + ": " + content_ + "\r\n");
			++it;
		}
		else 
		{
			it = connections_.erase(it);
		}
	}
}

