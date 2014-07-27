/* This is the main application. 
Pattern: Singleton
File: MainApplicaion.mm
Written By: Nicolas Broeking */

#ifndef __MainApplication__
#define __MainApplication__

#include "EventQueue.h"
#include <mutex>

class MainApplication
{
public:
	static MainApplication* app;
	static std::mutex getMutex;
	static MainApplication* getApplication();
	static void cleanUp();

	virtual int run();
	virtual void pleaseDie();

protected:
	MainApplication();
	virtual ~MainApplication();
	virtual void applicationWillLaunch();
	virtual void applicationWillTerminate();
	bool isRunning;
	bool shouldRun;

	std::mutex m; //Access Mutex
	BlockingQueue q;
	std::mutex runMutex; // So only one thread can run the application at a time

};
#endif
