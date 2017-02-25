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
#include <rle_interface.hpp>
#include <DebugMacros.h>
#include <algorithm>

#ifdef __USE_SDL
 #include <SDL.h>
#include "SDL/SDL_rotozoom.h"

#endif

using namespace std;
using namespace rle;

#include <unistd.h>
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " rom_file core_file" << std::endl;
        return 1;
    }
    RLEInterface rle;

    // Get & Set the desired settings
    rle.setInt("random_seed", 123);
    //The default is already 0.25, this is just an example
    rle.setFloat("repeat_action_probability", 0.25);

#ifdef __USE_SDL
    rle.setBool("display_screen", true);
    rle.setBool("sound", true);
#endif
    // Load the ROM file. (Also resets the system for new settings to
    // take effect.)
    rle.loadROM(argv[1], argv[2]);

    // Get the vector of minimal actions
    ActionVect legal_actions = rle.getMinimalActionSet();


//    // Play 10 episodes
    for (int episode=0; episode<10; episode++) {
        float totalReward = 0;
        while (!rle.game_over()) {
            Action a = legal_actions[rand() % legal_actions.size()];
        	// Apply the action and get the resulting reward
            float reward = rle.act(a);
            totalReward += reward;
        }
        cout << "Episode " << episode << " ended with score: " << totalReward << endl;
        rle.reset_game();
    }
    return 0;
}

