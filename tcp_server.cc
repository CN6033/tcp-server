#include "server.h"

int main()
{
	using namespace TCP_SERVER;
	Server server(8000);
	server.Start();
}

//ccc *.cc -I /usr/include -lboost_thread
