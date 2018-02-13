#include "codec.h"

#include <algorithm>

void Codec::onMessage(const ConnectionPtr &conn, Buffer &buf)
{
//	std::cerr << "Codec::onMessage()\n";
	const char *crlf = buf.findCRLF();
	if (crlf)
	{
//		std::cerr << "crlf found\n";
		const char *space = std::find(buf.peek(), crlf, ' ');
		if (space != crlf)
		{
			cmd_.assign(buf.peek(), space);			
			topic_.assign(space + 1, crlf);
			if (cmd_ == "pub")
			{
				const char *start = crlf + 2;
				crlf = buf.findCRLF(start);
				if (crlf != NULL)
				{
					content_.assign(start, crlf);
				}
				else 
				{
					return;
				}
			}
			else
			{
				content_ = std::string();
			}
			buf.retrieveUntil(crlf + 2);
			messageCallback_(conn);
		}
		else 
		{
			conn->send(std::string("Invalid command\r\n"));
			conn->shutdown();
		}
	}
//	else  std::cerr << "crlf unfound\n";
}

void Codec::parseMsg(std::string &cmd,
				std::string &topic,
				std::string &content)
{
	cmd = cmd_;
	topic = topic_;
	content = content_;
}

