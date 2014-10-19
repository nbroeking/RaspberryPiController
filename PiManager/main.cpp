//This is the main application
// It returns the result of the application
// Created by Nicolas Broeking
// Created on: July 23, 2014

/* SDL Initilization here just to be sure it gets done promptly 

*/

//The includes
#include "Application.h"
#include <iostream>
#include "SDLHelper.h"
int main()
{

	try 
	{
        	SDL sdl(SDL_INIT_AUDIO);
		return Application::run();
	}
	catch (const PiException& err) 
	{
		(void) err;
		std::cerr << err.what() << std::endl;
		return 1;
	}
	
	//If we could start SDL then we can start running the app
	//Return the result of the running app
	return 1;
}
