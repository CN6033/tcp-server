/*
 * File:	socket.h
 * Author:	huang shitao (hstaos@gmail.com)
 * Update:	2013-5-30 by huang shitao
 */

#ifndef _TCP_SERVER_SOCKET_H_
#define _TCP_SERVER_SOCKET_H_

#include <sys/socket.h>

#include "exception.h"

namespace TCP_SERVER
{

class Socket
{
public:
	Socket(int fd)
		:sockfd_(fd){}
	
	virtual ~Socket() throw()
	{
		close(this->sockfd_);
	}
	
	operator int()const
	{
		return this->sockfd_;
	}
	
	int get()const
	{
		return this->sockfd_;
	}
	
	bool operator==(const Socket& sock)
	{
		return this->sockfd_ == sock.get();
	}
	
	const Socket& operator=(const Socket& sock)
	{
		this->sockfd_ = sock.get();
		return *this;
	}
	
private:
	int sockfd_;
};

}

#endif
