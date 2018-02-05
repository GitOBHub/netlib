#include <buffer.h>

#include <algorithm>

int Buffer::readData()
{
	ssize_t nrecv;
	if ( (nrecv = ::recv(connFd_, inPtr_, 
						 &buf_[bufsize] - inPtr_, 0)) > 0 )
	{
		inPtr_ += nrecv;
	}
	else if (nrecv == -1)
	{
		std::cerr << "recv: " << strerror(errno) 
				  << std::endl;
	}
	return nrecv;
}

int Buffer::readableSize()
{
	return inPtr_ - outPtr_;
}

const char *Buffer::peek() const
{
	return outPtr_;
}

const char *Buffer::findCRLF(const char *start) const
{
	for (const char *cp = start; cp >= outPtr_ && cp < inPtr_ - 1; ++cp)
	{
		if ( *cp == '\r' && *(cp + 1) == '\n')
		{
			return cp;
		}
	}
	return NULL;
}

const char *Buffer::findCRLF() const
{
	const char *ret = std::find_if(outPtr_, inPtr_, 
			[] (char &c) 
			{ return (c == '\r' && *(&c + 1) == '\n'); });
	return (ret != inPtr_ ? ret : NULL);

	/*for (auto &c : buf_)
	{
		if (c == '\r' && *(&c + 1) == '\n')
		{
			return &c;
		}
	}
	return NULL;*/
}

std::string Buffer::retrieveUntil(const char *end)
{
	return retrieveData(end - buf_);
}

std::string Buffer::retrieveAllData()
{
	std::string data(outPtr_, inPtr_);
	inPtr_ = outPtr_ = buf_;
	return data;
}

std::string Buffer::retrieveData(int len)
{
	if (len <= 0)
	{
		std::string data;
		return data;
	}
	else if (len >= inPtr_ - outPtr_)
	{
		std::string data(outPtr_, inPtr_);
		inPtr_ = outPtr_ = buf_;
		return data;
	}
	else
	{
		std::string data(outPtr_, len);
		outPtr_ += len;
		if (inPtr_ == &buf_[bufsize])
		{
			char *temp = outPtr_;
			int offset = inPtr_ - outPtr_;
			inPtr_ -= offset;;
			outPtr_ = buf_;
			for (int i = 0; i < (inPtr_ - outPtr_); i++)
			{		
				*(outPtr_ + i) = *(temp + i);
			}
		}
		return data;
	}
}

bool Buffer::isBufferFull()
{
	return inPtr_ == &buf_[bufsize];
}
