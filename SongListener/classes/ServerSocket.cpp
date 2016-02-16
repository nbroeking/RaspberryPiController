#include "ServerSocket.h"

#include "Log.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include <thread>
#include "ScopedLock.h"
#include "Event.h"

#include <mutex>

#include "CommSystem.h"
using namespace std;
ServerSocket::ServerSocket(int setport, CommManager* manager ):
loopLock(),
serverAddress()
{
	ScopedLock l(loopLock);
	port = setport;

	status = 0;
	shouldRun = false;

	owner = manager;
	//We are set up to listen
	th = new thread(&ServerSocket::mainLoop, this);
 
}
ServerSocket::~ServerSocket()
{
	SystemLog("Started destructing the server socket");	
	if( (th != NULL)&&(th->joinable()))
	{
		this->th->join();
		SystemLog("Joined with the server socket");
	}
	if( th != NULL)
	{
		delete th;
	}
	//Clean up sockets

	close(socket);

	SystemLog("Server Socket has been destructed");
}

void ServerSocket::mainLoop()
{
	loopLock.lock();
	shouldRun = true;
	socket = -1;
	socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if( socket < 0 )
	{
		perror("Pi Error: Could not open socket");
		return;
	}	
	memset( &serverAddress, 0 , sizeof(serverAddress));
		
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);

	if( bind( socket, (struct sockaddr *) &serverAddress, sizeof(serverAddress) ) < 0 )
	{
		perror("Pi Error: Error on binding");
	}

	this->listen();

	loopLock.unlock();

	//Loop

	while( getMode() )
	{
		SystemLog("Preparing for accept");
		if( this->accept() < 0)
		{
			return;
		}
	}
}

void ServerSocket::pleaseDie()
{
	
	shutdown(socket, SHUT_RDWR);
	ScopedLock s(loopLock);
	shouldRun = false;
	//If needed we wake up the blocking socket
}

bool ServerSocket::getMode()
{
	ScopedLock s(loopLock);
	return shouldRun;
}
void ServerSocket::listen()
{
	if( ::listen(socket, 5) < 0)
	{
		SystemLog("There was an error listening with the server socket");
	}	
}
int ServerSocket::accept()
{
	int newsocket = ::accept(socket, NULL, NULL);
	ScopedLock s(loopLock);
	if( newsocket < 0 )
	{
		perror("Pi Error: Error accepting socket");
		return -1;
	}
	//Create event with file descriptor

	Event e;
	e.setType(NEWSOCKET);
	e.setFD(newsocket);

	owner->addEvent(e);
	
	return newsocket;
}
