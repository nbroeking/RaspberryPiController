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
				Event e;
				e.setType(PLAY);
				owner->addEvent(e);
			}
			else if( command == "STOP")
			{
				SystemLog("Stop");
				Event e;
				e.setType(STOP);
				owner->addEvent(e);
			}	
			else if( command == "PAUS" )
			{
				SystemLog("Pause");
				Event e;
				e.setType(PAUSE);
				owner->addEvent(e);
			}
			else if( command == "SYNC" )
			{
				SystemLog("Sync");
				Event e;
				e.setType(SYNC);
				owner->addEvent(e);
			}
			else
			{
				SystemError("Error unknown command arrived");
			}
		//	fprintf(stdout, "%s the socket read %d bytes with length %lu\n", command.c_str(), bytesRead, length);
		}
		delete [] buffer;	
	}
	SystemLog("Done reading");
	owner->socketDestroyed(this);
}
void Socket::write(std::string send)
{
	size_t len = send.length();
	char* tosend = (char*)send.c_str();
	int ret = ::write(socket, tosend, len);
	if( ret < 0)
	{
		fprintf(stderr, "PiError: Error writing to socket\n");
	}
	else
	{
		SystemLog("Wrote to the sockets");
	}
}
void Socket::pleaseDie()
{
	shutdown(socket, SHUT_RDWR);
	ScopedLock s(loopLock);
	shouldRun = false;

	//We need to wake up the blocking socket
}
bool Socket::getMode()
{
	ScopedLock s(loopLock);
	return shouldRun;
}
