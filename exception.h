/*
 * File:	exception.h
 * Author:	huang shitao （hstaos@gmail.com）
 * Update:	2013-5-30 by huang shitao
 */
 
#ifndef _TCP_SERVER_EXCEPTION_H_
#define _TCP_SERVER_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace TCP_SERVER
{

class SocketException: public std::exception
{
public:
	SocketException(const std::string &msg = "Socket Error!")
		:message_(msg){}
	
	virtual const char* what() const throw()
	{
    	return message_.c_str();
  	}
  	
  	virtual ~SocketException() throw() {}
  	
private:
	std::string message_;
};

}

#endif
