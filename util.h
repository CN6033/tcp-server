/*
 * File:	util.h
 * Author:	huang shitao
 * Email:	hstaos@gmail.com
 * Update:	2013-5-30 by huang shitao
 */

#ifndef _TCP_SERVER_UTIL_H_
#define _TCP_SERVER_UTIL_H_

#include <cstring>
#include <cassert>
#include <unistd.h>
#include <fcntl.h>

namespace TCP_SERVER
{
static void SetSockNonBlock(int fd) {
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        throw SocketException("Set socket non block error!");
    }
    
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        throw SocketException("fcntl(F_SETFL) failed!");
    }
}

}

#endif
