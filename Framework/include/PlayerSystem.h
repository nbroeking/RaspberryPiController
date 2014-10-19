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
#include "SDL/SDL.h"


class CommManager;

enum State { IDLE, PLAYING, STOPPED };

class Player
{
public:
	Player();
	virtual	~Player();
	virtual void run();
	virtual void pleaseDie();
	virtual void addEvent(Event);	
	void playmusic(void* udata, Uint8 *stream, int len);
	virtual void registerComm(CommManager*);	
protected:
	bool isRunning;
	bool shouldRun;

	CommManager* communications;

	std::mutex queueMutex;
	
	BlockingQueue q;
	std::thread* th;

	std::vector<std::string> songs;	
	//Audio settings
	SDL_AudioSpec audio;
	
	Uint8 *audio_chunk;
	Uint32 audio_len;
	Uint8 *audio_pos;

	
	//Thread Stuff
	//Methods
	virtual void mainLoop();

	State state;	

};

extern "C" {

extern void play_audio(void* udata, Uint8 *stream, int len);
}
#endif
