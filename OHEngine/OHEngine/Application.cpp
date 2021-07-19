/*
 * This is a demo application using the entity component game engine. 
 * Made by: Omar Salem Hadhoud for learning purposes.
 * GitHub: https://github.com/OmarHadhoud
 */

#include <iostream>

#include "src/game/Game.h"

int main()
{	
	Game *g = new Game();
	while (!g->GameEnded())
	{
		if (g->Run() == -1)
		{
			delete g;
			std::cout << "ERROR: GAME COULDN'T RUsN, APPLICATON IS TERMINATING" << std::endl;
			return -1;
		}
	}
	delete g;

	return 0;
}

