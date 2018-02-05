#ifndef TOPIC_H
#define TOPIC_H

#include <connection.h>

#include <unordered_set>
#include <set>

class Topic
{
public:
	Topic() = default;

	Topic(const std::string &name)
		: name_(name) { }

	void add(const ConnectionPtr &conn);
	void remove(const ConnectionPtr &conn);
	void doPublish(const std::string &content);

private:
	typedef std::weak_ptr<Connection> WeakConnectionPtr;
	struct Entry
	{
		Entry(const ConnectionPtr &conn)
			: weakConn_(conn) { }

		WeakConnectionPtr weakConn_;
	};
	typedef std::shared_ptr<Entry> EntryPtr;
	std::unordered_set<EntryPtr> connections_;
	std::string name_;
	std::string content_;
};

#endif	//TOPIC_H
