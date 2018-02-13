#ifndef BUFFER_H
#define BUFFER_H

#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

const int bufsize = BUFSIZ;

class Buffer
{
public:
	Buffer(int fd) 
		: fd_(fd), 
		  inPtr_(buf_), 
		  outPtr_(buf_) { }

	bool isBufferFull();
	int readableSize();
	const char *peek() const;
	const char *findCRLF(const char *start) const;
	const char *findCRLF() const;
	std::string retrieveAllData();
	std::string retrieveData(int len);
	std::string retrieveUntil(const char *end);
	void append(char *data, int len);
	ssize_t readData();

private:
	int fd_;
	char buf_[bufsize];
	char *inPtr_;
	char *outPtr_;
};

#endif	//BUFFER_H
