#include "server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <fcntl.h>

#include "exception.h"
#include "util.h"
#include "epoll.h"

namespace TCP_SERVER
{

static void Bind(int fd, int port);
static void Listen(int fd, int num);

Server::Server(int port=8000)
		:port_(port),max_buf_(1024),conn_num_(10),\
		serv_socket_(socket(AF_INET,SOCK_STREAM,0))
{
	if(serv_socket_.get() == -1)
	{
		throw SocketException("Create server socket error!");
	}
}

void Server::Start()
{	
	try{
	Bind(this->serv_socket_, this->port_);
	Listen(this->serv_socket_, this->conn_num_);
	Epoll epoll_serv(this->serv_socket_);
	epoll_serv.Run();
	
	}catch(SocketException& se){
		throw;
	}catch(...){
		throw;
	}
}

static void Bind(int fd, int port)
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	serv_addr.sin_port = htons(port);
	
	const int YES = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &YES, sizeof(int)) == -1) {
        throw SocketException("Set socket option error!");
    }
    
    SetSockNonBlock(fd);
    
    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        throw SocketException("Bind socket error!");
    }
}

static void Listen(int fd, int num)
{
	if (listen(fd, num) == -1) {
		throw SocketException("Listen socket error!");
    }
}

}
