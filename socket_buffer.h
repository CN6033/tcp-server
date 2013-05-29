#ifndef _TCP_SERVER_SOCKET_BUFFER_H_
#define _TCP_SERVER_SOCKET_BUFFER_H_

#include <ctime>
#include <cstdlib>
#include <string>
#include <cstring>

#include "exception.h"

namespace TCP_SERVER
{
class SocketBuffer
{
public:
	SocketBuffer(int size = 1024)
		:max_size_(size), time_(time((time_t*)0))
	{
		buf_ = (char*)malloc(sizeof(char)* max_size_);
		buf_[0] = '\0';
	}
	
	void Append(const char* data)
	{
		time_ = time((time_t*)0);
		
		size_t len = strlen(data);
		if(strlen(buf_) + len >= max_size_){
			char * new_buf = (char*)realloc (buf_, max_size_*2);
			if(new_buf){
				buf_ = new_buf;
				max_size_ = max_size_*2;
			}
			else{
				throw SocketException("bad alloc!");
			}
		}
		strncat (buf_, data, len);
	}
	
	void Clear()
	{
		buf_[0] = '\0';
	}
	
	virtual ~SocketBuffer()
	{
		free(buf_);
	}
	
	const char* toString()
	{
		return buf_;
	}
	
private:
	char* buf_;
	size_t max_size_;
	time_t time_;
};

}
#endif
