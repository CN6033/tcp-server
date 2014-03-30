/*
 * File:	uncopyable_class.h
 * Author:	huang shitao (apprentice1989@gmail.com)
 * Update:	2014-3-30 by huang shitao
 */

#ifndef _TCP_SERVER_UNCOPYABLE_CLASS_H_
#define _TCP_SERVER_UNCOPYABLE_CLASS_H_

namespace TCP_SERVER
{
class Uncopyable
{
protected:
	Uncopyable() {}
	virtual ~Uncopyable() {}
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator= (const Uncopyable&);
};	
}

#endif
