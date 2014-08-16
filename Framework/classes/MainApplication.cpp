#include "MainApplication.h"
#include "ScopedLock.h"
#include "Event.h"
#include "Log.h"

using namespace std;

//Singleton methods
MainApplication* MainApplication::app = NULL;
std::mutex MainApplication::getMutex;

MainApplication* MainApplication::getApplication()
{
	if (app == NULL )
	{
		app = new MainApplication();
	}
	return app;
}
void MainApplication::cleanUp()
{
	if( app != NULL)
	{
		delete app;
	}
	app = NULL;
}

//Main Running Loop
int MainApplication::run()
{
	//No two threads can try and run the application at a time. It shouldn't
	// happen anyway but you never know what mistakes i will make.
	if( !runMutex.try_lock() )
	{
		return 1;
	}
	m.lock();
	isRunning = true;
	//Initilization
	shouldRun = true;
	m.unlock();
	applicationWillLaunch();
	while (shouldRun)
	{
	//	ScopedLock s(m);
		Event e = q.pop();
		if( e.getType() == QUIT )
		{
			SystemLog("Main Application recieved a quit event");
			shouldRun = false;
			SystemError("Player Application will be asked to closed");
			player->pleaseDie();
			SystemError("Player Application was asked to closed");
		}
		else
		{
			SystemLog("Main Application recieved an event");
		}
	}
	m.lock();
	isRunning = false;
	m.unlock();
	applicationWillTerminate();

	runMutex.unlock();
	return 0;
}

//Constructor
MainApplication::MainApplication():
m(),
q(),
runMutex()
{
//	m.lock();
	isRunning = false;
	shouldRun = false;
	//Create the event queue
	player = new Player();
//	m.unlock();
}


MainApplication::~MainApplication()
{
//	m.lock();

	//Destruct memory
 	delete player;

//	m.unlock();	
}
void MainApplication::applicationWillLaunch()
{
//	m.lock();
	//Do setup for running
	player->run();
//	m.unlock();
	
}
void MainApplication::applicationWillTerminate()
{
//	m.lock();
	//Do Tear down for closing
	
//	m.unlock();
}
void MainApplication::pleaseDie()
{
	SystemLog("Please Die in Main Application");
//	m.lock();
	//Add a close event to the application
	Event quit;
	quit.setType(QUIT);
	q.push(quit);
//	m.unlock();
}
