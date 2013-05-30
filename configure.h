/*
 * File:	configure.h
 * Author:	huang shitao
 * Email:	hstaos@gmail.com
 * Update:	2013-5-30 by huang shitao
 */

#ifndef _TCP_SERVER_CONFIGURE_H_
#define _TCP_SERVER_CONFIGURE_H_

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>

namespace TCP_SERVER
{
class Configure: private boost::noncopyable
{
public:
	Configure(const std::string& filename = "./conf/tcp_server.conf")
	{
		using boost::property_tree::ptree;
    	read_xml(filename, this->pt_);
	}
	
	int GetMaxEpollEvents()const
	{
		return this->pt_.get<int>("conf.connection.epoll.max_events_num");
	}
	
	int GetMaxThreads()const
	{
		return this->pt_.get<int>("conf.thread.num");
	}
	
private:
	boost::property_tree::ptree pt_;
};

}
#endif
