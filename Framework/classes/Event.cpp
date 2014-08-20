#include "Event.h"
#include "ScopedLock.h"

Event::Event():
m()
{
	ScopedLock s(m);
	type = NOOP;
}
Event::Event(const Event& event):
m()
{
	//Each event must have its own mutex
	this->type = event.type;
	this->fd = event.fd;
}
Event::~Event()
{
}
Event& Event::operator= (const Event& event )
{
	//We dont copy mutexes
	//Hopefully thread safe but probably not
	this->type = event.type;
	this->fd = event.fd;
	return *this;
}
Type Event::getType()
{
	//ScopedLock
	ScopedLock s(m);
	return type;
}
void Event::setType(Type t)
{
	//Scoped Lock
	ScopedLock s(m);
	type = t;
}
int Event::getFD()
{
	ScopedLock s(m);
	return fd;
}
void Event::setFD(int x)
{
	ScopedLock s(m);
	fd = x;
}
