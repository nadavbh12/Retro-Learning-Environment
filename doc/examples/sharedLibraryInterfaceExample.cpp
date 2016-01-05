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
#endif

#include "../../src/environment/RetroAgent.h"

using namespace std;

//int main(int argc, char *argv[]) {
//	if (argc < 3)
//		cout << "usage: " << argv[0] << " <core> <game>" << endl;
//
//	RetroAgent rAgent;
//
//	if (!rAgent.initWindow())
//		cout << "Failed to initialize glfw" << endl;
//
//	rAgent.loadCore(argv[1]);
//	rAgent.loadRom(argv[2]);
//
//	while (!rAgent.windowShouldClose()) {
//		rAgent.pollEvents();
//
//		rAgent.run();
//
//		rAgent.clearWindows();
//
//		rAgent.videoRender();
//
//		rAgent.swapBuffers();
//	}
//
//	rAgent.unloadCore();
//	rAgent.audioDeinit();
//	rAgent.videoDeinit();
//
//	rAgent.terminateWindow();
//	return 0;
//}



/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////								Below is ALE 											/////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*#include <unistd.h>
int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " rom_file" << std::endl;
        return 1;
    }

//    char* cwd;
//    char buff[512 + 1];
//
//    cout << argv[1] << endl;
//
//    cwd = getcwd( buff, 512+ 1 );
//    if( cwd != NULL ) {
//        printf( "My working directory is %s.\n", cwd );
//    }


    ALEInterface ale;

//    TODO SN: implement once we have configuration alternative
    // Get & Set the desired settings
    ale.setInt("random_seed", 123);
    //The default is already 0.25, this is just an example
    ale.setFloat("repeat_action_probability", 0.25);

//#ifdef __USE_OpenGL
    ale.setBool("display_screen", true);
    ale.setBool("sound", true);
//#endif

    // Load the ROM file. (Also resets the system for new settings to
    // take effect.)
    ale.loadROM(argv[1]);

    // Get the vector of legal actions
    ActionVect legal_actions = ale.getLegalActionSet();
    // SN: replaced legal with minimal since legal is too big
//    ActionVect legal_actions = ale.getMinimalActionSet();

//    // Play 10 episodes
//    for (int episode=0; episode<10; episode++) {
//        float totalReward = 0;
//        while (!ale.game_over()) {
//            Action a = legal_actions[rand() % legal_actions.size()];
//            // Apply the action and get the resulting reward
//            float reward = ale.act(a);
//            totalReward += reward;
//        }
//        cout << "Episode " << episode << " ended with score: " << totalReward << endl;
//        ale.reset_game();
//    }
//
    return 0;
}
*/

// testing code
#include <unistd.h>
int main(int argc, char** argv) {
    ALEInterface ale;
    ale.loadROM(argv[1]);

    cout << ale.theRetroAgent->readRam(RETRO_MEMORY_SAVE_RAM, std::stoi(argv[2])) << endl;

    return 0;
}
