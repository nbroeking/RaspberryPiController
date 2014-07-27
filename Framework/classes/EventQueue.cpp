#include "EventQueue.h"
#include "ScopedLock.h"

using namespace std;
BlockingQueue::BlockingQueue():
m(),
cond()
{
	ScopedLock s(m);
	q = new queue<Event>();
}
BlockingQueue::~BlockingQueue()
{
	ScopedLock s(m);
	delete q;
}
void BlockingQueue::push(Event e)
{
	ScopedLock s(m);
	q->push(e);
	cond.notify_one();
	//Signal Free
}
Event BlockingQueue::pop()
{
	std::unique_lock<std::mutex> lock(m);
	
	while( q->empty() )
	{
		cond.wait(lock);
	}
	Event e = q->front();
	q->pop();
	return e; //Check that this is valid
}

