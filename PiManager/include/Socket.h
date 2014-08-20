#ifndef __SOCKET__
#define __SOCKET__

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include "Socket.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <thread>
#include <mutex>

class CommManager;

class Socket 
{
public:
    	Socket( int fd, CommManager* manager = NULL) ;
	~Socket();

	void pleaseDie();
private:
	int socket; //FD

	CommManager* owner;
	bool getMode();
	void mainLoop(); 

	std::mutex loopLock;
	std::thread* th;

	bool shouldRun;	
} ;

#endif
