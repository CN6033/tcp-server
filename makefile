tcp_server:	tcp_server.o	server.o	epoll.o
	g++ -std=c++0x -Wall -O -o tcp_server	tcp_server.o	server.o	epoll.o	-I /usr/include -lboost_thread
tcp_server.o:	tcp_server.cc	server.h	debug.h
	g++ -std=c++0x -Wall -O -c tcp_server.cc
server.o:	server.cc	exception.h	util.h	epoll.h
	g++	-std=c++0x -Wall -O -c server.cc
epoll.o:	epoll.cc	assert.h	socket_buffer.h	util.h
	g++	-std=c++0x -Wall -O -c epoll.cc
clean:
	rm	tcp_server	*.o
