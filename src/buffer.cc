#include <buffer.h>

#include <algorithm>

ssize_t Buffer::readData()
{
	//XXX:effecient way seen in muduo
	ssize_t nrecv = ::recv(fd_, beginWrite(), writableSize(), 0);
//	std::cerr << "Buffer::readData() - nrecv = " << nrecv << std::endl;
	if (nrecv > 0 )
	{
		writerIndex_ += nrecv;
	}
	else if (nrecv == -1)
	{
		::perror("::recv");
	}
    return nrecv;
}

const char *Buffer::findCRLF(const char *start) const
{
	const char *kCRLF = "\r\n";
	assert(start >= peek() && start <= beginWrite());
	const char *ret = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
	return ret != beginWrite() ? ret : NULL;
}

const char *Buffer::findCRLF() const
{
	const char *ret = std::find_if(peek(), beginWrite(),	//FIXME:std::search() 
								   [] (const char &c) 
			                       { return (c == '\r' && *(&c + 1) == '\n'); });
	return ret != beginWrite() ? ret : NULL;
}

std::string Buffer::retrieveUntil(const char *end)
{
	assert(end >= peek() && end <= beginWrite());
	return retrieveData(end - peek());
}

std::string Buffer::retrieveAllData()
{
	std::string data(peek(), readableSize());
	writerIndex_ = readerIndex_ = kInitIndex;
	return data;
}

std::string Buffer::retrieveData(int len)
{
	//XXX: when arg is size_t, len >= 0, but assert(len <= readableSize())
	if (len <= 0)
	{
		std::string data;
		return data;
	}
	else if (len >= readableSize())
	{
		return retrieveAllData();
	}
	else
	{
		std::string data(peek(), len);
		readerIndex_ += len;
		if (readerIndex_ == buf_.size())
		{
			//int readSize = readableSize();	
			//XXX: std::copy()
			//::memmove(&*buf_.begin(), peek(), readSize); 
			//readerIndex_ = kInitIndex;
			//writerIndex_ = readerIndex_ + readSize;
		}
		return data;
	}
}

void Buffer::append(const char *data, size_t len)
{
	if (len > writableSize())
	{
		makeSpace(len);
	}
	std::copy(data, data + len, beginWrite());
	writerIndex_ += len;
}
 
void Buffer::makeSpace(size_t len)
{
	int readSize = readableSize();	
	//XXX: std::copy()
	::memmove(&*buf_.begin(), peek(), readSize); 
	readerIndex_ = kInitIndex;
	writerIndex_ = readerIndex_ + readSize;
	if (len > writableSize())
	{
		buf_.resize(writerIndex_ + len);
	}
}
