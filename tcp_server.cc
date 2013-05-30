/*
 * File:	tcp_server.cc
 * Author:	huang shitao
 * Email:	hstaos@gmail.com
 * Update:	2013-5-30 by huang shitao
 */

#include "server.h"

int main()
{
	using namespace TCP_SERVER;
	Server server(8000);
	server.Start();
}

//ccc *.cc -I /usr/include -lboost_thread
