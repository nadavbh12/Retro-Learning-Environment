/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <memory>

#if (defined(WIN32) || defined(__MINGW32__))
#   include "os_dependent/SettingsWin32.hxx"
#   include "os_dependent/OSystemWin32.hxx"
#else
#   include "os_dependent/SettingsUNIX.hxx"
#   include "os_dependent/AleSystemUNIX.hxx"
#endif

#include "controllers/ale_controller.hpp"
#include "controllers/fifo_controller.hpp"
#include "controllers/rlglue_controller.hpp"
#include "common/Constants.h"
#include "ale_interface.hpp"

static std::auto_ptr<AleSystem> theAleSystem(NULL);
static std::auto_ptr<Settings> theSettings(NULL);
static std::auto_ptr<RetroAgent> theRetroAgent;

//static ALEController* createController(OSystem* osystem, std::string type) {
//  if(type.empty()){
//    std::cerr << "You must specify a controller type (via -game_controller)." << std::endl;
//    exit(1);
//  }
//  else if (type == "fifo") {
//    std::cerr << "Game will be controlled through FIFO pipes." << std::endl;
//    return new FIFOController(osystem, false);
//  }
//  else if (type == "fifo_named") {
//    std::cerr << "Game will be controlled through named FIFO pipes." << std::endl;
//    return new FIFOController(osystem, true);
//  }
//  else if (type == "rlglue") {
//    std::cerr << "Game will be controlled through RL-Glue." << std::endl;
//    return new RLGlueController(osystem);
//  }
//  else {
//    std::cerr << "Invalid controller type: " << type << " " << std::endl;
//    exit(1);
//  }
//}

/* application entry point */
int main(int argc, char* argv[]) {

  ALEInterface::disableBufferedIO();

  std::cerr << ALEInterface::welcomeMessage() << std::endl;

  ALEInterface::createAleSystem(theAleSystem, theSettings, theRetroAgent);
  // Process commandline arguments, which over-ride all possible
  // config file settings
  std::string romfile = theAleSystem->settings().loadCommandLine(argc, argv);
  ALEInterface::loadSettings(romfile, theAleSystem);

  // TODO SN: implement after we have some settings
  // Create the game controller
  std::string controller_type = theAleSystem->settings().getString("game_controller");
//  std::auto_ptr<ALEController> controller(createController(theAleSystem.get(), controller_type));

//  controller->run();

  // MUST delete theOSystem to avoid a segfault (theOSystem relies on Settings
  //  still being a valid construct)
  theAleSystem.reset(NULL);

  return 0;
}
