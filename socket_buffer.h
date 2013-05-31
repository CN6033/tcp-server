/*
 * File:	socket_buffer.h
 * Author:	huang shitao （hstaos@gmail.com）
 * Update:	2013-5-30 by huang shitao
 */

#ifndef _TCP_SERVER_SOCKET_BUFFER_H_
#define _TCP_SERVER_SOCKET_BUFFER_H_

#include <ctime>
#include <cstdlib>
#include <string>
#include <cstring>

#include "exception.h"
#include "define.h"

namespace TCP_SERVER
{

class SocketBuffer
{
public:
    /*将socket buffer的大小默认设置为recv函数接收数据报的缓冲区大小。
    之所以这么设置，是期望能减小socket buffer调用realloc的次数。*/
	SocketBuffer(int size = SOCKET_BUFFER_SIZE)
		:max_size_(size), time_(time((time_t*)0))
	{
		buf_ = (char*)malloc(sizeof(char)* max_size_);
		buf_[0] = '\0';
	}
	
	void Append(const char* data)
	{
		size_t len = strlen(data);
		
		while(strlen(buf_) + len >= max_size_){
			int new_size = max_size_*2;
			if(new_size > MAX_SOCKET_BUFFER_SIZE){
				new_size = MAX_SOCKET_BUFFER_SIZE;
			}
			char* new_buf = (char*)realloc(buf_, new_size);
			if(new_buf){
				buf_ = new_buf;
				max_size_ = new_size;
			}
			else{
				throw SocketException("bad alloc!");
			}
		}
		int buf_size = strlen(buf_);
		strncat (buf_, data, (len < (max_size_ - buf_size))?len : (max_size_ - buf_size - 1));
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
