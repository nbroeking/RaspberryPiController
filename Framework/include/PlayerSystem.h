/*
* File: Player.h
* Written By: Nicolas Broeking
* Written On: Jul 26, 2014
*
*/

#ifndef __Player__
#define __Player__

#include "EventQueue.h"
#include <mutex>
#include <thread>

class Player
{
public:
	Player();
	virtual	~Player();
	virtual void run();
	virtual void pleaseDie();

protected:
	bool isRunning;
	bool shouldRun;
	
	//Thread Stuff
	std::mutex queueMutex;
	
	BlockingQueue q;
	std::thread* th;
	//Methods
	virtual void mainLoop();

};
#endif
