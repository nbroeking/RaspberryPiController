/*
* File: CommSystem.h
* Written By: Nicolas Broeking
* Written On: August 8, 2014
*
*/

#ifndef __CommSystem__
#define __CommSystem__

#include "EventQueue.h"
#include "Event.h"
#include <mutex>
#include <thread>
//#include "SDL/SDL.h"
#include "PlayerSystem.h"
#include <vector>
#include "Socket.h"

class CommManager 
{
public:
	CommManager();
	virtual	~CommManager();
	virtual void run();
	virtual void pleaseDie();
	virtual void addEvent(Event);

	void socketCreated(Socket*);
	void socketDestroyed(Socket*); 
	
protected:
	bool isRunning;
	bool shouldRun;

	std::mutex queueMutex;
	
	BlockingQueue q;
	std::thread* th;

	std::vector<Socket*> connections;
	std::vector<Socket*> deletion;	
	//Thread Stuff
	//Methods
	virtual void mainLoop();

};

#endif
