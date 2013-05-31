/*
 * File:	server.h
 * Author:	huang shitao (hstaos@gmail.com)
 * Update:	2013-5-30 by huang shitao
 */
 
#ifndef _TCP_SERVER_SERVER_H_
#define _TCP_SERVER_SERVER_H_

#include <boost/noncopyable.hpp>

#include "socket.h"

namespace TCP_SERVER
{
class Server: private boost::noncopyable
{
public:
	Server(int);
	void Start();
	
private:
	int port_;
	int max_buf_;
	int conn_num_;
	Socket serv_socket_;
};

}

#endif
