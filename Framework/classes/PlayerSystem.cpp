#include "PlayerSystem.h"
#include "ScopedLock.h"
#include <stdlib.h>
#include "Log.h"
#include <SDL/SDL.h>
#include "CommSystem.h"
#include "dirent.h"

//#include <SDL/SDL_mixer.h>

//#include "QtMultimedia/QtMultimedia"
using namespace std;

static Player* p = NULL;

Player::Player():
queueMutex(),
q(),
songs()//,
//audio()
{	
	ScopedLock s(queueMutex);
	state = IDLE;

	p = this;
	isRunning = false;
	shouldRun = false;
	th = NULL;
	
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("/home/nbroeking/Documents/PiManager/Media")) != NULL) 
	{
  		/* print all the files and directories within directory */
  		while ((ent = readdir (dir)) != NULL) 
		{
			if( strcmp(ent->d_name,".") && strcmp(ent->d_name,".."))
			{
    				printf ("Optional Song: %s\n", ent->d_name);
				songs.push_back(ent->d_name);
			}
			else
			{
				printf( "Can not play: %s\n" , ent->d_name);
			}
  		}
  		closedir (dir);
	} 
	else 
	{
  		/* could not open directory */
  		perror ("PiError: Can not open music directory ");
  		return;
	}	

/*	if( SDL_LoadWAV("/home/nbroeking/Documents/PiManager/Media/onemorenight.wav", &audio, &audio_chunk, &audio_len ) == NULL)
	{

	}	
	//Set up audio
	audio.callback = play_audio;

	if( SDL_OpenAudio(&audio, NULL) < 0 )
	{
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	}			
*/	
}
Player::~Player()
{
//	ScopedLock s(queueMutex);
	//Destruct Memory

	queueMutex.lock();
	if( state != IDLE )
	{
	//		SDL_CloseAudio();
	//	SDL_FreeWAV(audio_chunk);
	}
	p = NULL;
	if( (th != NULL)&&(th->joinable()))
	{
		queueMutex.unlock();
	//	SystemLog("Joining the player manager thread");
		this->th->join();
		SystemLog( "Joined the player manager thread");
		queueMutex.lock();
	}
	if( th != NULL )
	{
		delete th;
	}
	queueMutex.unlock();
}
void Player::run()
{
	ScopedLock s(queueMutex);
	isRunning = true;
	shouldRun = true;
	this->th = new thread(&Player::mainLoop, this);	
}
void Player::registerComm(CommManager* c)
{
	communications = c;
}
void Player::mainLoop()
{
	SystemLog("Started the player manager thread");
	bool runloop = true;	

	//We need communications
	if( communications == NULL )
	{
		return;
	}
	while( runloop )
	{
		//ScopedLock s( queueMutex );
		Event e = q.pop();
		if( e.getType() == QUIT )
		{
			ScopedLock l(queueMutex);
//			SystemLog("Player Manager is asked to die");
//			SDL_PauseAudio(1);
			runloop = shouldRun = false;
		}
		else if( e.getType() == NOOP )
		{
			SystemLog("Player: NOOP");
		}
		else if( e.getType() == STOP )
		{
			ScopedLock l(queueMutex);
//			SDL_PauseAudio(1);
			state = STOPPED;

			SystemLog("Player Manager Stoped Song Event");
		}
		else if( e.getType() == PLAY )
		{
			ScopedLock l(queueMutex);
//			SDL_PauseAudio(0);
			state = PLAYING;
			SystemLog("Player Manager Played Song Event");
		}
		else if( e.getType() ==SYNC )
		{
			ScopedLock l(queueMutex);
			Event ne;
			ne.setType(SEND);
			
			if( state == PLAYING)
			{
				ne.setFD(0);
			}
			else if( state == STOPPED )
			{
				ne.setFD(1);
			}
			else
			{
				ne.setFD(2);
			}
			ne.setSongs(songs);
			
			communications->addEvent(ne);		
		}
		else
		{
			SystemLog("Player Manager Started Song Event");
			ScopedLock l(queueMutex);
/*			if( state == PLAYING )
			{
				SystemError("Song already playing: can't play two songs at once");
				continue;
			}

			state = PLAYING;

			SystemLog("Trying to play a song with SDL");
			
			audio_pos = audio_chunk;
*/	
			//Start Playing
			/* Let the callback function play the audio chunk */
//			SDL_PauseAudio(0);

		}
	}
}

void Player::pleaseDie()
{
//	SystemLog("Asking the Player Manager to quit");
	Event quit;
	quit.setType(QUIT);
	q.push(quit);
//	SystemLog("Player Manager is queued to quit");
}

void Player::addEvent(Event e)
{
	queueMutex.lock();
	if( !shouldRun)
	{
		queueMutex.unlock();
		return;
	}
	queueMutex.unlock();
	q.push(e);
}
//void Player::playmusic(void* udata, Uint8 *stream, int len)
//{
	/* Only play if we have data left */
/*	if ( audio_len == 0 )
	{
		return;
	}
*/
	/* Mix as much data as possible */
/*	len = ( len > (int)audio_len ? audio_len : len );
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
*/
//}
/* The audio function callback takes the following parameters:
  stream:  A pointer to the audio buffer to be filled
  len:     The length (in bytes) of the audio buffer
*/
/*void play_audio(void *udata, Uint8 *stream, int len)
{
	if( p == NULL)
	{
		SystemError("The player object is dead");
		return;
	}
	p->playmusic(udata, stream, len);
}*/
