/*
* File: Player.h
* Written By: Nicolas Broeking
* Written On: Jul 26, 2014
*
*/

#ifndef __Player__
#define __Player__

#include "EventQueue.h"
#include <mutex>
#include <thread>
//#include "SDL/SDL.h"


enum State { IDLE, PLAYING, STOPPED };

class Player
{
public:
	Player();
	virtual	~Player();
	virtual void run();
	virtual void pleaseDie();
	
//	void playmusic(void* udata, Uint8 *stream, int len);	
protected:
	bool isRunning;
	bool shouldRun;

	std::mutex queueMutex;
	
	BlockingQueue q;
	std::thread* th;
	
	//Audio settings
/*	SDL_AudioSpec audio;
	
	Uint8 *audio_chunk;
	Uint32 audio_len;
	Uint8 *audio_pos;
*/
	
	//Thread Stuff
	//Methods
	virtual void mainLoop();

	State state;	

};

/*extern "C" {

extern void play_audio(void* udata, Uint8 *stream, int len);
}*/
#endif
