#include "Application.h"
#include <stdio.h>
#include <signal.h>
#include "Log.h"

using namespace std;

std::mutex Application::m;
int Application::run()
{
	//Only one thing can run the app
	m.lock();

	MainApplication *app = MainApplication::getApplication();	
	//Set up the signal handling
	signal( SIGINT, sigKillRecieved);

	//Expecially the the sigkill handler

	//run the MainApplicationLoop
	int result = app->run();
	MainApplication::cleanUp();
	m.unlock();

	return result;
}

void sigKillRecieved(int a)
{
	SystemLog("SigKill was recieved. Asking the application to close");
	MainApplication::getApplication()->getApplication()->pleaseDie();
}
