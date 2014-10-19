#include "CommSystem.h"
#include "ScopedLock.h"
#include <stdlib.h>
#include "Log.h"
#include "Pin.h"
#include "ServerSocket.h"

using namespace std;

CommManager::CommManager(Player* play):
queueMutex(),
q(),
connections(),
deletion(),
pin17("17")
{	
	player = play;
	player->registerComm(this);
	ScopedLock s(queueMutex);

	isRunning = false;
	shouldRun = false;
	th = NULL;

	Event e;
	e.setType(START);
	player->addEvent(e);

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
		else if( e.getType() == SEND )
		{
			SystemLog("Sending data to the clients");
			std::string sendstring;

			std::vector<std::string> sendSongs = e.getSongs();
			if (sendSongs.empty() )
			{
				sendstring = "1";
				sendstring += e.getFD() +'0';
			}
			else
			{
				sendstring = "0";
				sendstring += e.getFD();
				for( std::vector<std::string>::iterator it = sendSongs.begin(); it != sendSongs.end(); ++it)
				{
					sendstring = sendstring + ","+ *it;
				}
			}
			sendstring += ",";
			for( std::vector<Socket*>::iterator t = connections.begin(); t != connections.end(); ++t )
			{
				(*t)->write(sendstring);
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
	pin17.on();
	Event e;
	e.setType(SYNC);
	q.push(e);

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
	pin17.off();
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
