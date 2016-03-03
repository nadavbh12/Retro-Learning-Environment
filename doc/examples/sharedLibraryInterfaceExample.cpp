/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare,
 *  Matthew Hausknecht, and the Reinforcement Learning and Artificial Intelligence 
 *  Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  sharedLibraryInterfaceExample.cpp 
 *
 *  Sample code for running an agent with the shared library interface. 
 **************************************************************************** */

#include <iostream>
#include <ale_interface.hpp>
#include <DebugMacros.h>

#ifdef __USE_SDL
 #include <SDL.h>
#include "SDL/SDL_rotozoom.h"

#endif

#include "../../src/environment/RetroAgent.h"

using namespace std;
using namespace ale;
void dispalyExample (ALEInterface *ale);

#include <unistd.h>
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " rom_file core_file" << std::endl;
        return 1;
    }
    ALEInterface ale;

    // Get & Set the desired settings
    ale.setInt("random_seed", 123);
    //The default is already 0.25, this is just an example
    ale.setFloat("repeat_action_probability", 0.25);

#ifdef __USE_SDL
    ale.setBool("display_screen", true);
    ale.setBool("sound", true);
#endif

    // Load the ROM file. (Also resets the system for new settings to
    // take effect.)
    ale.loadROM(argv[1], argv[2]);

    // Get the vector of minimal actions
    ActionVect legal_actions = ale.getMinimalActionSet();

//    // Play 10 episodes
    for (int episode=0; episode<10; episode++) {
        float totalReward = 0;
        while (!ale.game_over()) {
            Action a = legal_actions[rand() % legal_actions.size()];
//            dispalyExample(&ale);
        	// Apply the action and get the resulting reward
            float reward = ale.act(a);
            totalReward += reward;
        }
        cout << "Episode " << episode << " ended with score: " << totalReward << endl;
        ale.reset_game();
    }
    return 0;
}



void dispalyExample (ALEInterface *ale){
#ifdef __USE_SDL
	ALEScreen aleScreen = ale->getScreen();
	size_t screen_height = (size_t)aleScreen.height();
	size_t screen_width = (size_t)aleScreen.width();
	int window_width = 428;
	int window_height = 321;
	void* data = (void*)aleScreen.getArray();
	uint32_t pitch = 2*screen_width;
	uint32_t rmask, gmask, bmask, amask;
	bmask = 0x001f;
	gmask = 0x07e0;
	rmask = 0xf800;
	amask = 0x0000;

	SDL_Surface * tempScreen = SDL_CreateRGBSurfaceFrom(data, screen_width, screen_height,
	                            16, pitch, rmask, gmask, bmask, 0);
	SDL_Surface* zoomed = zoomSurface(tempScreen, window_width/(double)screen_width, window_height/(double)screen_height, 0);

	SDL_Surface* screen = SDL_SetVideoMode(window_width, window_height, 16, SDL_SWSURFACE);
	SDL_BlitSurface(zoomed, NULL, screen, NULL);
	SDL_UpdateRect(screen, 0, 0, screen_width, screen_height);
	SDL_Flip(screen);
	SDL_FreeSurface(tempScreen);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(zoomed);

	//	FILE *a = fopen("/home/administrator/DQN/DeepMind-Atari-Deep-Q-Learner/cWrite1.txt", "w");
		FILE *b = fopen("/home/administrator/DQN/DeepMind-Atari-Deep-Q-Learner/cWrite2.txt", "w");
//	    uint8_t* originBuffer = data;
	    uint8_t* destBuffer =(uint8_t*) data;
	    for( int i = 0; i < screen_height * screen_width * 2 ; i++){
	//    	if(i % m_alesystem->getRetroAgent().getPitch() == 0){
	//    		fprintf(a,"\n");
	//    	}
	    	if(i % (screen_width * 2) == 0){
				fprintf(b,"\n");
			}
	//    	fprintf(a,"%x, ", originBuffer[i]);
	    	fprintf(b,"%x, ", destBuffer[i]);
	//		fprintf(a,"%d, ", i);

	    }
	//    fclose(a);
	    fclose(b);
#endif
}

