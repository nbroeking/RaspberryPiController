#include "CommSystem.h"
#include "ScopedLock.h"
#include <stdlib.h>
#include "Log.h"

#include "ServerSocket.h"

using namespace std;

CommManager::CommManager(Player* play):
queueMutex(),
q(),
connections(),
deletion()
{	
	player = play;
	player->registerComm(this);
	ScopedLock s(queueMutex);

	isRunning = false;
	shouldRun = false;
	th = NULL;

}
CommManager::~CommManager()
{
//	ScopedLock s(queueMutex);
	//Destruct Memory
	queueMutex.lock();
	if( (th != NULL)&&(th->joinable()))
	{	
		queueMutex.unlock();
		this->th->join();
		SystemLog( "Joined the comm  manager thread");
		queueMutex.lock();
	}
	if( th != NULL )
	{
		delete th;
	}
	queueMutex.unlock();

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

	//We need a player manager to run
	if( player == NULL )
	{
		return;
	}
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
			SystemLog("Server Socket has been asked to die");
			ScopedLock l(queueMutex); 
			//Clean up the sockets
			SystemLog("Before connections");
			for( std::vector<Socket*>::iterator it = connections.begin(); it != connections.end(); ++it)
			{
				(*it)->pleaseDie();
			}
			SystemLog("Middle Connections");
			for( std::vector<Socket*>::iterator it = connections.begin(); it != connections.end(); ++it)
			{
				delete *it;
			}
			SystemLog("Almost there");
			connections.erase(connections.begin(), connections.end());
			deletion.erase(deletion.begin(), deletion.end());
			SystemLog("Communications finished processing quit event");
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
			ScopedLock l(queueMutex);
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
		else //Its a player event
		{
			SystemLog("CommManager Manager received an event");
			player->addEvent(e);
		}
	}

	//Clean up he server socket

	delete sock;
}

void CommManager::socketCreated(Socket* sock)
{
	ScopedLock s(queueMutex);
	if( !shouldRun )
	{
		return;
	}
	connections.push_back(sock);
	SystemLog("Socket registered with the Comm Manager");
}
void CommManager::socketDestroyed(Socket* sock)
{
//	queueMutex.lock();
	if( !shouldRun )
	{
//		queueMutex.unlock();
		return;
	}
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
	queueMutex.lock();
	if( !shouldRun )
	{
		queueMutex.unlock();
		return;
	}
	queueMutex.unlock();
	q.push(e);
}
