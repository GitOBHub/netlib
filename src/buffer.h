#ifndef BUFFER_H
#define BUFFER_H

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>

#include <string>
#include <vector>
#include <iostream>
#include <cassert>


const int kInitBufSize = BUFSIZ;
const int kInitIndex = 0;

class Buffer
{
public:
	Buffer(int fd) 
		: fd_(fd), buf_(kInitBufSize) { }

	int readableSize() const { return writerIndex_ - readerIndex_; }
	int writableSize() const { return buf_.size() - writerIndex_; }
	int prependableSize() const { return readerIndex_ - kInitIndex; }
	const char *peek() const { return &buf_[readerIndex_]; }  //buf_ + readerIndex_; 
	char *beginWrite() { return &*(buf_.begin() + writerIndex_); }
	const char *beginWrite() const { return &buf_[writerIndex_]; }
	const char *findCRLF(const char *start) const;
	const char *findCRLF() const;
	std::string retrieveAllData();
	std::string retrieveData(int len);
	std::string retrieveUntil(const char *end);
	void append(const char *data, size_t len);
	ssize_t readData();

private:
	void makeSpace(size_t len);

	int fd_;
	std::vector<char> buf_;
	size_t writerIndex_ = kInitIndex;
	size_t readerIndex_ = kInitIndex;
};

#endif	//BUFFER_H
