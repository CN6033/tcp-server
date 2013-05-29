#include <iostream>
#include "socket_buffer.h"

int main()
{
	using namespace std;
	using namespace TCP_SERVER;
	SocketBuffer buf;
	cout << buf.toString() << '\n';
	buf.Append("hello, world");
	cout << buf.toString() << '\n';
	buf.Append("hello, world");
	cout << buf.toString() << '\n';
	buf.Append("hello, world");
	cout << buf.toString() << '\n';
	buf.Append("hello, world");
	cout << buf.toString() << '\n';
	int i = 10;
	while(i--){
		buf.Append("hello, world");
		cout << strlen(buf.toString()) << '\n';
	}
	buf.Clear();
	cout << strlen(buf.toString()) << '\n';
	//cout << buf.toString() << '\n';
}
