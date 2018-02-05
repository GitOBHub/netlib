#ifndef CODEC_H
#define CODEC_H

#include <buffer.h>
#include <connection.h>

class Codec
{
public:
	typedef std::function<void (const ConnectionPtr &,
								Buffer &)> MessageCallback;
	Codec(const MessageCallback &cb)
		: messageCallback_(cb) { }

	void parseMsg(std::string &cmd,
				  std::string &topic,
				  std::string &content);
	void onMessage(const ConnectionPtr &conn, Buffer &buf);

private:

	MessageCallback  messageCallback_;
	std::string cmd_;
	std::string topic_;
	std::string content_;
};

#endif	//CODEC_H
