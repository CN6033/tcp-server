#ifndef _TCP_SERVER_EPOLL_H_
#define _TCP_SERVER_EPOLL_H_

#include <unordered_map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "threadpool.hpp"

#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#include "exception.h"
#include "socket_buffer.h"
#include "util.h"
#include "socket.h"

namespace TCP_SERVER
{
class Epoll: private boost::noncopyable
{
public:
	Epoll(int sockfd)
		:serv_sock_(sockfd), max_events_(64)
	{
		if ((epfd_ = epoll_create(1)) == -1 ) {
            throw SocketException("epoll_create() error!");
        }
       
       	struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = sockfd;
        
        if(epoll_ctl(epfd_, EPOLL_CTL_ADD, sockfd, &event) == -1 ) {
            throw SocketException("epoll_ctl() error!");
        }
        
        memset(events_, 0, sizeof(events_));
        
        thread_pool_ptr_ = boost::shared_ptr<boost::threadpool::pool>(new boost::threadpool::pool(64));
	}
	
	void Run();
	
	virtual ~Epoll(){}
	
private:
	int epfd_;
	int serv_sock_;
	int max_events_;
	std::unordered_map<int, boost::shared_ptr<SocketBuffer> > recv_buffer_;
	std::unordered_map<int, boost::shared_ptr<SocketBuffer> > send_buffer_;
	//struct epoll_event event_;
	struct epoll_event events_[64];
	boost::shared_ptr<boost::threadpool::pool> thread_pool_ptr_;
};

}
#endif
