#include "exception.h"

#include <stdexcept>
#include <iostream>
#include <cassert>

int main()
{
	using namespace TCP_SERVER;
	using namespace std;
	try{
		throw SocketException();
	}
	catch(std::exception &e){
		cout << e.what() << '\n';
	}
	
	assert(true);
}
