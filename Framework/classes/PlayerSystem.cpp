#include "PlayerSystem.h"
#include "ScopedLock.h"
#include <stdlib.h>
#include "Log.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

//#include "QtMultimedia/QtMultimedia"

using namespace std;
Player::Player():
queueMutex(),
q()
{
	ScopedLock s(queueMutex);
	isRunning = false;
	shouldRun = false;
	th = NULL;		
}
Player::~Player()
{
	ScopedLock s(queueMutex);
	//Destruct Memory
	if( (th != NULL)&&(th->joinable()))
	{
		SystemLog("Joining the player manager thread");
		this->th->join();
		SystemLog( "Joined the player manager thread");
	}
	if( th != NULL )
	{
		delete th;
	}

}
void Player::run()
{
	ScopedLock s(queueMutex);
	isRunning = true;
	shouldRun = true;
	this->th = new thread(&Player::mainLoop, this);	
}
void Player::mainLoop()
{
	SystemLog("Started the player manager thread");
	bool runloop = true;	
	
	Event e;
	
	q.push(e);

	while( runloop )
	{
		//ScopedLock s( queueMutex );
		Event e = q.pop();
		if( e.getType() == QUIT )
		{
			SystemLog("Player Manager is asked to die");
			runloop = shouldRun = false;
		}
		else
		{
//			QMediaPlayer *player = new QMediaPlayer();
//			player->setMedia(QUrl::fromLocalFile("/home/nbroeking/Documents/PiManager/Media/onemorenight.wav"));	
//			player->setVolume(50);
//			player->play();
//			Mix_Music *music = NULL;
			SystemLog( "Player Manager recieved anoter event");

		}
	}
}
void Player::pleaseDie()
{
//	ScopedLock s( queueMutex );

	SystemLog("Asking the Player Manager to quit");
	Event quit;
	quit.setType(QUIT);
	q.push(quit);
	SystemLog("Player Manager is queued to quit");
}


