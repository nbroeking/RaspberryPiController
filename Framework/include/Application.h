/* This is the main application. 
Pattern: Singleton
Uses: Should be run from the main.m */


#ifndef __Application__
#define __Application__
#include <mutex>
#include "MainApplication.h"

class Application
{

public:
	static int run ();
	static std::mutex m;
};

extern "C" {
	void sigKillRecieved(int);
}
#endif
