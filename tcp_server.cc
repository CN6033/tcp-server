#include "server.h"
#include "debug.h"

int main()
{
	using namespace TCP_SERVER;
	Server server(8000);
	DEBUG(__FILE__, __LINE__);
	server.Start();
	DEBUG(__FILE__, __LINE__);
}

//ccc *.cc -I /usr/include -lboost_thread
