/*
* File: Event.h
* Written By: Nicolas Broeking
* Written On: July 25, 2014
*
* This is the list of events that need to be processed.
*
*/

#ifndef __NicEvent__
#define __NicEvent__

enum Type { NOOP, QUIT, STOP, PLAY, NUM};

#include <mutex>
#include <string>

class Event
{
public:
	Event();
	~Event();
	Event(const Event&);
	Event & operator= (const Event& event);
	Type getType();
	void setType(Type);
protected:
private:
	std::mutex m;
	Type type;
	
};
#endif //End Declaration
