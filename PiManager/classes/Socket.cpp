#include "Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include "Log.h"
#include "ScopedLock.h"
#include "CommSystem.h"
using namespace std;

Socket::Socket( int fd, CommManager* manager):
loopLock()
{
	ScopedLock l(loopLock);	
	socket = fd;
	owner = manager;
	SystemLog("A new socket has been created");
	shouldRun = false;
	th = new thread(&Socket::mainLoop, this);
}
Socket::~Socket()
{
	if( (th != NULL)&&(th->joinable() ))
	{
		this->th->join();
		SystemLog("Joined with a client socket");
	}
	if( th != NULL )
	{
		delete th;
	}

	//close file descriptor

	close(socket);
}
void Socket::mainLoop()
{
	loopLock.lock();
	shouldRun = true;
	owner->socketCreated(this);

	loopLock.unlock();
	
	while( getMode() )
	{
		//Blocking Read
		char* buffer = new char[1024];
		size_t length = 1024;
		int bytesRead = read(socket, buffer, length);
		if( bytesRead < 0 )
		{

			SystemError("Error Reading Socket");
		}		
		else if( bytesRead == 0 )
		{

			SystemLog("Socket Disconnected closing socket");
			pleaseDie();
		}
		else
		{
			//If there is stuff to read 
			std::string command(buffer);
			command = command.substr(0, 4);
			if( command == "PLAY" )
			{
				SystemLog("Play");
			}
			else if( command == "STOP")
			{
				SystemLog("Stop");
			}	
			else if( command == "PAUS" )
			{
				SystemLog("Pause");
			}
			else
			{
				SystemError("Error unknown command arrived");
			}
			fprintf(stdout, "%s the socket read %d bytes with length %lu\n", command.c_str(), bytesRead, length);
		}
		delete [] buffer;	
	}

	owner->socketDestroyed(this);
}
void Socket::pleaseDie()
{
	ScopedLock s(loopLock);
	shouldRun = false;

	//We need to wake up the blocking socket
}
bool Socket::getMode()
{
	ScopedLock s(loopLock);
	return shouldRun;
}
