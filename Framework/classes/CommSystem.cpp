#include "CommSystem.h"
#include "ScopedLock.h"
#include <stdlib.h>
#include "Log.h"

#include "ServerSocket.h"

using namespace std;

CommManager::CommManager():
queueMutex(),
q(),
connections(),
deletion()
{	
	ScopedLock s(queueMutex);

	isRunning = false;
	shouldRun = false;
	th = NULL;

}
CommManager::~CommManager()
{
	ScopedLock s(queueMutex);
	//Destruct Memory

	if( (th != NULL)&&(th->joinable()))
	{
		this->th->join();
		SystemLog( "Joined the comm  manager thread");
	}
	if( th != NULL )
	{
		delete th;
	}

}
void CommManager::run()
{
	ScopedLock s(queueMutex);
	isRunning = true;
	shouldRun = true;
	this->th = new thread(&CommManager::mainLoop, this);	
}
void CommManager::mainLoop()
{
	SystemLog("Started the communication manager thread");
	bool runloop = true;	

	//Socket starts in its own thread and will start 
	//ServerSocket*	sock =
	ServerSocket *sock = new ServerSocket(54321, this);
	while( runloop )
	{
		//ScopedLock s( queueMutex );
		Event e = q.pop();
		if( e.getType() == QUIT )
		{
	//		SystemLog("CommManager Manager is asked to die");
			runloop = shouldRun = false;

			sock->pleaseDie();

			ScopedLock l(queueMutex); 
			//Clean up the sockets

			for( std::vector<Socket*>::iterator it = connections.begin(); it != connections.end(); ++it)
			{
				(*it)->pleaseDie();
			}
			for( std::vector<Socket*>::iterator it = connections.begin(); it != connections.end(); ++it)
			{
				delete *it;
			}

			connections.erase(connections.begin(), connections.end());
			deletion.erase(deletion.begin(), deletion.end());
		}
		else if( e.getType() == NEWSOCKET )
		{
			//Comm Manager is doing the socket shit
			//Socket* n =
			new Socket(e.getFD(), this);
		//	(void) n;
		}
		else if( e.getType() == CLEANUP )
		{
			SystemLog("Clean Up Event");
			Socket* del = deletion.back();
			deletion.pop_back();

			std::vector<Socket*>::iterator it;
			for( it = connections.begin(); it != connections.end(); ++it)
			{
				if( (*it) == del )
				{				
					(*it)->pleaseDie();
					delete *it;
					break;
					SystemLog("Deleting the Socket Thread");
				}
			}
			if( it != connections.end() )
			{
				SystemLog("Deleting the socket thread from the list");
				connections.erase(it);
			}
		}
		else
		{
			SystemLog("CommManager Manager received an event");
		}
	}

	//Clean up he server socket

	delete sock;
}

void CommManager::socketCreated(Socket* sock)
{
	ScopedLock s(queueMutex);
	connections.push_back(sock);
	SystemLog("Socket registered with the Comm Manager");
}
void CommManager::socketDestroyed(Socket* sock)
{
	SystemLog("Socket has disconnected");
	queueMutex.lock();
	deletion.push_back(sock);
	queueMutex.unlock();

	//Push cleanup event

	Event e;
	e.setType(CLEANUP);

	q.push(e);
}
void CommManager::pleaseDie()
{
//	ScopedLock s( queueMutex );

//	SystemLog("Asking the CommManager Manager to quit");
	Event quit;
	quit.setType(QUIT);
	q.push(quit);
}
void CommManager::addEvent(Event e)
{
	q.push(e);
}
