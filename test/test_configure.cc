#include "configure.h"
#include <iostream>

int main()
{
	using namespace std;
	using namespace TCP_SERVER;
	Configure conf;
	cout << conf.GetMaxEpollEvents() << endl;
	cout << conf.GetMaxThreads() << endl;
}
