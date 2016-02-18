#include "ScopedLock.h"
#include "Event.h"
#include "Log.h"
#include "Application.h"

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
			shouldRun = false;
			break;
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
	isRunning = false;
	shouldRun = false;
}


MainApplication::~MainApplication()
{
	pleaseDie();
}
void MainApplication::applicationWillLaunch()
{
	
}
void MainApplication::applicationWillTerminate()
{
}
void MainApplication::pleaseDie()
{
//	SystemLog("Please Die in Main Application");
	//Add a close event to the application
	Event quit;
	quit.setType(QUIT);
	q.push(quit);
}
