#ifndef __SERVERSOCKET__
#define __SERVERSOCKET__

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

class ServerSocket
{
public:
	ServerSocket(int setport = 0, CommManager* manager = NULL);
	~ServerSocket();

	void listen();	
	int  accept();

	void write(std::string);
	
	void pleaseDie();

protected:

	bool getMode();	
	void mainLoop();

	std::mutex loopLock;
	std::thread* th;
	CommManager* owner;

	int socket;
	int port;

	bool shouldRun;
	struct sockaddr_in serverAddress;
	int status; 
};
#endif
