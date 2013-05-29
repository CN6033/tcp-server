#ifndef _TCP_SERVER_ASSERT_H_
#define _TCP_SERVER_ASSERT_H_

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

namespace TCP_SERVER
{

static inline void ASSERT(int condition, 
						const char* filename, 
						int line, 
						const std::string& msg)
{
	#ifndef NODEBUG
	if(!condition){
		std::cerr << "ASSERT fail@" << filename << ':' << line << ':' << msg << '\n';
		exit(EXIT_FAILURE);
	}
	#endif
}

}

#endif
