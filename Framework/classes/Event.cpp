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
}
Event::~Event()
{
}
Event& Event::operator= (const Event& event )
{
	//We dont copy mutexes
	//Hopefully thread safe but probably not
	this->type = event.type;
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
