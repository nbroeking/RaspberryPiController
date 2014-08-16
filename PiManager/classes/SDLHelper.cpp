#include "SDLHelper.h"
#include <SDL/SDL.h>
#include "Log.h"

SDL::SDL(Uint32 flags) throw(PiException) 
{
    if (SDL_Init(flags) != 0)
    {
        throw PiException();
    }
}

SDL::~SDL() 
{
    	SDL_Quit();
	SystemLog("Shut Down SDL");
}

