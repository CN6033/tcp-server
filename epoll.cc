/*
 * File:	epoll.cc
 * Author:	huang shitao （hstaos@gmail.com）
 * Update:	2013-5-30 by huang shitao
 */

#include "epoll.h"

#include <string>
#include <cstring>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <utility>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "assert.h"
#include "socket_buffer.h"
#include "util.h"
#include "define.h"

namespace TCP_SERVER
{
void Proc(int, boost::shared_ptr<SocketBuffer>, boost::shared_ptr<SocketBuffer>, boost::function<void()>);
void EpollCtl(int, int, int, struct epoll_event *);

void Epoll::Run()
{
	int res = -1;
	for(;;){
		res = epoll_wait(this->epfd_, this->events_, this->max_events_, -1);
		
		if (res < 0){
            throw SocketException("epoll_wait failed!");
        } else if (res == 0) {
            continue;
        }
        
        int i = 0;
        for (; i < res; ++i){
        	if ( (this->events_[i].events & EPOLLERR) || 
                 (this->events_[i].events & EPOLLHUP) ||
                 (!this->events_[i].events & EPOLLIN) ||
                 (!this->events_[i].events & EPOLLOUT)){
                //fprintf (stderr, "epoll error\n");
                close(this->events_[i].data.fd);
                continue;
            }
            
            if(this->events_[i].data.fd == this->serv_sock_){
            	/*Connection arrived*/
            	int conn_socket;
        		struct sockaddr_in client_addr;
				socklen_t client_addr_len;
				
				for(;;){
					client_addr_len = sizeof(client_addr);
					conn_socket = accept(this->serv_sock_, (struct sockaddr *) &client_addr, &client_addr_len);
				
					if (conn_socket == -1) {
						if ( (errno == EAGAIN) || (errno == EWOULDBLOCK) ) {
						    /*non-blocking模式下无新connection请求，跳出for(;;)*/
						    break;
						} else {
						    ASSERT(false, __FILE__ , __LINE__, "");
						}
					}
					
					SetSockNonBlock(conn_socket);
					
					struct epoll_event event;
					event.events = EPOLLIN | EPOLLET;
                    event.data.fd = conn_socket;
                    
                    EpollCtl(this->epfd_, EPOLL_CTL_ADD, conn_socket, &event);
                   
                    SocketBuffer *recv_ptr_buf = new SocketBuffer();
                    this->recv_buffer_[conn_socket] = boost::shared_ptr<SocketBuffer>(recv_ptr_buf);
                    
                    SocketBuffer *send_ptr_buf = new SocketBuffer();
                   	this->send_buffer_[conn_socket] = boost::shared_ptr<SocketBuffer>(send_ptr_buf);
                }
            }
            else if((this->events_[i].events & EPOLLIN)){
            	/*Recveive event*/
            	int recv_socket;
            	int recv_size;
            	recv_socket = this->events_[i].data.fd;
            	//const static int BUF_SIZE = 10240;
            	char buffer[SOCKET_BUFFER_SIZE];
                memset(buffer, 0, sizeof(buffer));
                
                if ((recv_size = recv(recv_socket, buffer, SOCKET_BUFFER_SIZE, 0)) == -1  
                		&& ((errno != EAGAIN)&&(errno != EWOULDBLOCK))) {
                    /*recv在non-blocking模式下，返回-1且errno为EAGAIN或EWOULDBLOCK表示当前无可读数据，并不表示错误*/
                    Socket sock(recv_socket);
                	if(0 == this->recv_buffer_.erase(sock)){
                		ASSERT(false, __FILE__ , __LINE__, "");
                	}
                	EpollCtl(this->epfd_, EPOLL_CTL_DEL, sock, NULL);
                }
                else if(recv_size == 0){
                	/*Connection closed.*/
                	Socket sock(recv_socket);
                	if(0 == this->recv_buffer_.erase(sock)){
                		ASSERT(false, __FILE__ , __LINE__, "");
                	}
                	EpollCtl(this->epfd_, EPOLL_CTL_DEL, sock, NULL);
                }
                else if(recv_size < SOCKET_BUFFER_SIZE){
                	/*Recv completed*/
                	std::unordered_map<int, boost::shared_ptr<SocketBuffer> >::const_iterator iter 
                			= this->recv_buffer_.find(recv_socket);
                	
                	if(iter != this->recv_buffer_.end()){
                		iter->second->Append(buffer);
                	}else{
                		ASSERT(false, __FILE__ , __LINE__, "");
                	}
                	
                	struct epoll_event event;
					event.events = EPOLLOUT | EPOLLET;
                    event.data.fd = recv_socket;
                    
                	boost::function<void()> callback = boost::bind(
                							&EpollCtl, 
                							this->epfd_,
                							EPOLL_CTL_MOD,
                							recv_socket,
                							&event);

                	boost::function<void()> proc = boost::bind(
                							&Proc,
                							recv_socket,
                							this->recv_buffer_[recv_socket],
                							this->send_buffer_[recv_socket],
                							callback);
                							
                	this->thread_pool_ptr_->schedule(proc);
                }
                else{
                	std::unordered_map<int, boost::shared_ptr<SocketBuffer> >::const_iterator \
                			iter = this->recv_buffer_.find(recv_socket);
                	
                	if(iter != this->recv_buffer_.end()){
                		iter->second->Append(buffer);
                	}else{
                		ASSERT(false, __FILE__ , __LINE__, "");
                	}
                }
            }
            else if((this->events_[i].events & EPOLLOUT)){
            	/*Send event*/
            	int send_socket = this->events_[i].data.fd;
            	int send_size;
            	int buf_size = 0;
            	const char* buffer = 0;
                std::unordered_map<int, boost::shared_ptr<SocketBuffer> >::const_iterator \
                	iter = this->send_buffer_.find(send_socket);
            	if(iter != this->send_buffer_.end()){
            		buffer = iter->second->toString();
            		buf_size = strlen(buffer);
            	}else{
            		ASSERT(false, __FILE__ , __LINE__, "");
            	}
            	
            	if ((send_size = send(send_socket, buffer, buf_size, 0)) == -1 && 
            		(errno != EAGAIN) && 
            		(errno != EWOULDBLOCK)){
            		/*send在non-blocking模式下，返回-1且errno为EAGAIN或EWOULDBLOCK表示当前无可写数据，并不表示错误*/
                }
                else if(send_size != buf_size){
                	ASSERT(false, __FILE__ , __LINE__, "");
                }

                Socket sock(send_socket);
            	if(0 == this->send_buffer_.erase(sock)){
            		ASSERT(false, __FILE__ , __LINE__, "");
            	}
            	if(0 == this->recv_buffer_.erase(sock)){
            		ASSERT(false, __FILE__ , __LINE__, "");
            	}
            	EpollCtl(this->epfd_, EPOLL_CTL_DEL, sock, NULL);
            }
            else{
            	ASSERT(false, __FILE__ , __LINE__, "");
			}
        }
	}
}

void Proc(	int fd,
			boost::shared_ptr<SocketBuffer> recv_buf_ptr,
			boost::shared_ptr<SocketBuffer> send_buf_ptr,
			boost::function<void()> callback)
{
	//send_buf_ptr = recv_buf_ptr;
	send_buf_ptr->Append(recv_buf_ptr->toString());
	callback();
}

void EpollCtl(int epfd, int op, int fd, struct epoll_event *event)
{
	if(-1 == epoll_ctl(epfd, op, fd, event)){
		ASSERT(false, __FILE__ , __LINE__, "");
		//TODO:throw exception.
	}
}

}
