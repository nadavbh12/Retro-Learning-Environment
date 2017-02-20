/* *****************************************************************************
 * The lines 201 - 204 are based on Xitari's code, from Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  rle_interface.cpp
 *
 *  The shared library interface.
 **************************************************************************** */

#include "rle_interface.hpp"
#include "RleSystem.hxx"
#include "games/Roms.hpp"
#include "common/display_screen.h"
#include "environment/retro_environment.hpp"
#include "environment/FSNode.hxx"
//#include "common/ScreenExporter.hpp"
#include "common/Log.hpp"
#include "common/RleException.h"
#include <string>
#include <stdexcept>
#include <ctime>

#include <sstream>

using namespace std;
namespace rle {

class RLEInterface::Impl{
public:
	Impl();
	~Impl();

	// Get the value of a setting.
	std::string getString(const std::string& key);
	int getInt(const std::string& key);
	bool getBool(const std::string& key);
	float getFloat(const std::string& key);

	// Set the value of a setting. loadRom() must be called before the
	// setting will take effect.
	void setString(const std::string& key, const std::string& value);
	void setInt(const std::string& key, const int value);
	void setBool(const std::string& key, const bool value);
	void setFloat(const std::string& key, const float value);

	  // Resets the game, but not the full system.
	  void reset_game();

	  // Resets the Atari and loads a game. After this call the game
	  // should be ready to play. This is necessary after changing a
	  // setting for the setting to take effect.
	  // RLE: specify which core to load as well. Default is Atari.
	  void loadROM(std::string rom_file ="", std::string core_file = "");

	  // Applies an action to the game and returns the reward. It is the
	  // user's responsibility to check if the game has ended and reset
	  // when necessary - this method will keep pressing buttons on the
	  // game over screen.
	  reward_t act(Action actionA, Action actionB);

	  // Indicates if the game has ended.
	  bool game_over() const;

	  // Returns the vector of legal actions. This should be called only
	  // after the rom is loaded.
	  ActionVect getLegalActionSet();

	  // Returns the vector of the minimal set of actions needed to play
	  // the game.
	  ActionVect getMinimalActionSet();

	  // Returns the frame number since the loading of the ROM
	  int getFrameNumber();

	  // The remaining number of lives.
	  const int lives() const;

	  // Returns the frame number since the start of the current episode
	  int getEpisodeFrameNumber() const;

	  // Returns the current game screen
	  const RLEScreen &getScreen() const;

	  // Returns the current RAM content
	  const RLERAM &getRAM() const;

	  // Saves the state of the system
	  void saveState();

	  // Loads the state of the system
	  void loadState();

	  // This makes a copy of the environment state. This copy does *not* include pseudorandomness,
	  // making it suitable for planning purposes. By contrast, see cloneSystemState.
	  RLEState cloneState();

	  // Reverse operation of cloneState(). This does not restore pseudorandomness, so that repeated
	  // calls to restoreState() in the stochastic controls setting will not lead to the same outcomes.
	  // By contrast, see restoreSystemState.
	  void restoreState(const RLEState& state);

	  // This makes a copy of the system & environment state, suitable for serialization. This includes
	  // pseudorandomness and so is *not* suitable for planning purposes.
	  RLEState cloneSystemState();

	  // Reverse operation of cloneSystemState.
	  void restoreSystemState(const RLEState& state);

	  // Save the current screen as a png file
	  void saveScreenPNG(const std::string& filename);

	  // Creates a ScreenExporter object which can be used to save a sequence of frames. Ownership
	  // said object is passed to the caller. Frames are saved in the directory 'path', which needs
	  // to exists.
	  ScreenExporter *createScreenExporter(const std::string &path) const;

	  // static functions
	  void createRleSystem(pRleSystem &theRleSystem,
	                      pSettings &theSettings,
	                      pRetroAgent &theRetroAgent);
	  void loadSettings(const std::string& romfile, const std::string& corefile,
	                     pRleSystem &theSLESystem);

private:
	  pRleSystem theRleSystem;
	  pSettings theSettings;
	  pRetroAgent theRetroAgent;
	  pRomSettings romSettings;
	  pRetroEnvironment environment;
	  int max_num_frames; // Maximum number of frames for each episode
	  bool gameLoaded;
};

//thread_local bool RLEInterface::Impl::initialized = false;

RLEInterface::Impl::Impl() : max_num_frames(0), gameLoaded(false){
  RLEInterface::createRleSystem(theRleSystem, theSettings, theRetroAgent);
}

RLEInterface::Impl::~Impl() {
}


// Get the value of a setting.
std::string RLEInterface::Impl::getString(const std::string& key) {
  assert(theSettings);
  return theSettings->getString(key);
}
int RLEInterface::Impl::getInt(const std::string& key) {
  assert(theSettings);
  return theSettings->getInt(key);
}
bool RLEInterface::Impl::getBool(const std::string& key) {
  assert(theSettings);
  return theSettings->getBool(key);
}
float RLEInterface::Impl::getFloat(const std::string& key) {
  assert(theSettings);
  return theSettings->getFloat(key);
}

// Set the value of a setting.
void RLEInterface::Impl::setString(const string& key, const string& value) {
  assert(theSettings);
  assert(theRleSystem);
  theSettings->setString(key, value);
}
void RLEInterface::Impl::setInt(const string& key, const int value) {
  assert(theSettings);
  assert(theRleSystem);
  theSettings->setInt(key, value);
}
void RLEInterface::Impl::setBool(const string& key, const bool value) {
  assert(theSettings);
  assert(theRleSystem);
  theSettings->setBool(key, value);
}
void RLEInterface::Impl::setFloat(const string& key, const float value) {
  assert(theSettings);
  assert(theRleSystem);
  theSettings->setFloat(key, value);
  if(key == "random_seed"){
	  theRleSystem->resetRNGSeed();
  }
}

// Get the value of a setting.
std::string RLEInterface::getString(const std::string& key) {
	return m_pimpl->getString(key);
}
int RLEInterface::getInt(const std::string& key) {
	return m_pimpl->getInt(key);
}
bool RLEInterface::getBool(const std::string& key) {
	return m_pimpl->getBool(key);
}
float RLEInterface::getFloat(const std::string& key) {
	return m_pimpl->getFloat(key);
}

// Set the value of a setting.
void RLEInterface::setString(const string& key, const string& value) {
	m_pimpl->setString(key, value);
}
void RLEInterface::setInt(const string& key, const int value) {
	m_pimpl->setInt(key, value);
}
void RLEInterface::setBool(const string& key, const bool value) {
	m_pimpl->setBool(key, value);
}
void RLEInterface::setFloat(const string& key, const float value) {
	m_pimpl->setFloat(key, value);
}


RLEInterface::RLEInterface() : m_pimpl(new RLEInterface::Impl()) {
  disableBufferedIO();
  Logger::Info << welcomeMessage() << std::endl;
}

RLEInterface::RLEInterface(const std::string &rom_file, const std::string &core_file) :
		RLEInterface(){
	loadROM(rom_file, core_file);
}

RLEInterface::RLEInterface(bool display_screen) : RLEInterface()  {
  m_pimpl->setBool("display_screen", display_screen);
}

RLEInterface::~RLEInterface() {
	delete m_pimpl;
}

void RLEInterface::reset_game() {
	m_pimpl->reset_game();
}

// Resets the game, but not the full system.
void RLEInterface::Impl::reset_game() {
	environment->reset();
}


reward_t RLEInterface::act(Action actionA, Action actionB) {
	return m_pimpl->act(actionA, actionB);
}
reward_t RLEInterface::Impl::act(Action actionA, Action actionB) {

  reward_t reward = environment->act(actionA, actionB);
  if (theRleSystem->p_display_screen != NULL) {
    theRleSystem->p_display_screen->display_screen();
    while (theRleSystem->p_display_screen->manual_control_engaged() && !game_over()) {
      Action user_action = theRleSystem->p_display_screen->getUserAction();
	  // Used to reset game manually pressing 'r' implemneted in display screen
      if (user_action == JOYPAD_SYSTEM_RESET){
		  environment->reset();
	  }
      reward += environment->act(user_action, PLAYER_B | JOYPAD_NOOP);
      theRleSystem->p_display_screen->display_screen();
    }
  }
  return reward;
}


// Display RLE welcome message
std::string RLEInterface::welcomeMessage() {
  std::ostringstream oss;
  oss << "R.L.E: Retro Learning Environment (version "
      << Version << ")\n"
	  << "[Based upon the Arcade Learning Environment (A.L.E)]\n"
      << "[Powered by LibRetro]\n"
      << "Use -help for help screen.";
  return oss.str();
}

void RLEInterface::disableBufferedIO() {
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);
  cin.rdbuf()->pubsetbuf(0,0);
  cout.rdbuf()->pubsetbuf(0,0);
  cin.sync_with_stdio();
  cout.sync_with_stdio();
}

void RLEInterface::createRleSystem(pRleSystem& theRleSystem,
                          pSettings& theSettings,
                          pRetroAgent& theRetroAgent) {
	theRetroAgent.reset(new RetroAgent());
	theSettings.reset(new Settings());
	theRleSystem.reset(new RleSystem(theRetroAgent, theSettings));
}

void RLEInterface::loadSettings(const string& romfile, const std::string& corefile,
                                pRleSystem &theRleSystem) {
//  // Load the configuration from a config file (passed on the command
//  //  line), if provided
//  string configFile = theRleSystem->settings().getString("config", false);
//
//  if (!configFile.empty())
//    theRleSystem->settings().loadConfig(configFile.c_str());

	if (romfile == "" || !FilesystemNode::fileExists(romfile)) {
		Logger::Error << "No ROM File specified or the ROM file was not found."
			<< std::endl;
		exit(1);
	}
	theRleSystem->loadCore(corefile);
	theRleSystem->loadRom(romfile);
	Logger::Info << "Running ROM file..." << std::endl;
	theRleSystem->settings()->setString("rom_file", romfile);
#ifdef __USE_SDL
	if(theRleSystem->settings()->getBool("display_screen")){
		theRleSystem->p_display_screen = make_shared<DisplayScreen>(theRleSystem->getRetroAgent());
	}
#endif
	Logger::Info << "Random seed is " << theRleSystem->settings()->getInt("random_seed") << std::endl;
	theRleSystem->resetRNGSeed();
}

void RLEInterface::loadROM(string rom_file, string core_file) {
	m_pimpl->loadROM(rom_file, core_file);
}

// Loads and initializes a game. After this call the game should be
// ready to play. Resets the OSystem/Console/Environment/etc. This is
// necessary after changing a setting. Optionally specify a new rom to
// load.
void RLEInterface::Impl::loadROM(string rom_file, string core_file) {
  assert(theRleSystem);
  if (rom_file.empty()) {
    rom_file = theRleSystem->romFile();
  }
  if (core_file.empty()) {
	core_file = theRleSystem->coreFile();
  }
  RLEInterface::loadSettings(rom_file, core_file, theRleSystem);

  bool twoPlayers = getBool("two_players");
  romSettings.reset(buildRomRLWrapper(rom_file, twoPlayers));
  environment = make_shared<RetroEnvironment>(theRleSystem, romSettings);
  max_num_frames = theRleSystem->settings()->getInt("max_num_frames_per_episode");
  environment->reset();

#ifndef __USE_SDL
  if (theRleSystem->p_display_screen) {
    Logger::Error << "Screen display requires directive __USE_SDL to be defined." << endl;
    Logger::Error << "Please recompile this code with flag '-D__USE_SDL'." << endl;
    Logger::Error << "Also ensure RLE has been compiled with USE_SDL active (see RLE makefile)." << endl;
    exit(1);
  }
#endif
  gameLoaded = true;
}

bool RLEInterface::game_over() const{
	return m_pimpl->game_over();
}

// Indicates if the game has ended.
bool RLEInterface::Impl::game_over() const{
  return (environment->isTerminal() ||
          (max_num_frames > 0 && getEpisodeFrameNumber() >= max_num_frames));
}

const int RLEInterface::lives() const {
	return m_pimpl->lives();
}

// The remaining number of lives.
const int RLEInterface::Impl::lives() const{
  if (!romSettings.get()){
    throw std::runtime_error("ROM not set");
  }
  return romSettings->lives();
}

ActionVect RLEInterface::getLegalActionSet() {
	return m_pimpl->getLegalActionSet();
}

// TODO SN : replace function with something else since the vector of all legal actions woul dbe too big
// Returns the vector of legal actions. This should be called only
// after the rom is loaded.
ActionVect RLEInterface::Impl::getLegalActionSet() {
  if (!romSettings.get()){
    throw std::runtime_error("ROM not set");
  }
  return romSettings->getAllActions();
}

ActionVect RLEInterface::getMinimalActionSet() {
	return m_pimpl->getMinimalActionSet();
}

// Returns the vector of the minimal set of actions needed to play
// the game.
ActionVect RLEInterface::Impl::getMinimalActionSet() {
  if (!romSettings.get()){
    throw std::runtime_error("ROM not set");
  }
  return romSettings->getMinimalActionSet();
}

int RLEInterface::getFrameNumber() {
  return m_pimpl->getFrameNumber();
}

// Returns the frame number since the loading of the ROM
int RLEInterface::Impl::getFrameNumber() {
  return environment->getFrameNumber();
}

int RLEInterface::getEpisodeFrameNumber() const{
  return m_pimpl->getEpisodeFrameNumber();
}

// Returns the frame number since the start of the current episode
int RLEInterface::Impl::getEpisodeFrameNumber() const{
  return environment->getEpisodeFrameNumber();
}

const RLEScreen& RLEInterface::getScreen() const {
  return m_pimpl->getScreen();
}

// Returns the current game screen
const RLEScreen& RLEInterface::Impl::getScreen() const{
  return environment->getScreen();
}

const RLERAM& RLEInterface::getRAM() const{
  return m_pimpl->getRAM();
}

// Returns the current RAM content
const RLERAM& RLEInterface::Impl::getRAM() const{
  if(!gameLoaded){
	  throw RleException("Need to load a game before you can read the RAM");
  }
  return environment->getRAM();
}

void RLEInterface::saveState() {
	m_pimpl->saveState();
}

// Saves the state of the system
void RLEInterface::Impl::saveState() {
  environment->save();
}

void RLEInterface::loadState() {
	m_pimpl->loadState();
}

// Loads the state of the system
void RLEInterface::Impl::loadState() {
  environment->load();
}

RLEState RLEInterface::cloneState() {
  return m_pimpl->cloneState();
}

RLEState RLEInterface::Impl::cloneState() {
  return environment->cloneState();
}

void RLEInterface::restoreState(const RLEState& state) {
  return m_pimpl->restoreState(state);
}

void RLEInterface::Impl::restoreState(const RLEState& state) {
  return environment->restoreState(state);
}

RLEState RLEInterface::cloneSystemState() {
  return m_pimpl->cloneSystemState();
}

RLEState RLEInterface::Impl::cloneSystemState() {
  return environment->cloneSystemState();
}

void RLEInterface::restoreSystemState(const RLEState& state) {
  return m_pimpl->restoreSystemState(state);
}

void RLEInterface::Impl::restoreSystemState(const RLEState& state) {
  return environment->restoreSystemState(state);
}

void RLEInterface::saveScreenPNG(const string& filename) {
	m_pimpl->saveScreenPNG(filename);
}

void RLEInterface::Impl::saveScreenPNG(const string& filename) {
  
//  ScreenExporter exporter(theRleSystem->colourPrlette());
//  exporter.save(environment->getScreen(), filename);
}

//ScreenExporter *RLEInterface::createScreenExporter(const std::string &filename) const {
//	return m_pimpl->createScreenExporter(filename);
//}

//ScreenExporter *RLEInterface::Impl::createScreenExporter(const std::string &filename) const {
////    return new ScreenExporter(theRleSystem->colourPrlette(), filename);
//}

} // namespace rle
