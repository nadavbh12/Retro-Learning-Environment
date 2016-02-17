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
 *  ale_interface.cpp
 *
 *  The shared library interface.
 **************************************************************************** */

#include "ale_interface.hpp"
#include "os_dependent/SettingsWin32.hxx"
#include "os_dependent/SettingsUNIX.hxx"
#include "os_dependent/AleSystemUNIX.hxx"
#include "games/Roms.hpp"
#include "common/display_screen.h"
#include "environment/retro_environment.hpp"
#include "os_dependent/SettingsUNIX.hxx"
#include "os_dependent/SettingsWin32.hxx"
#include "environment/FSNode.hxx"
#include "common/ScreenExporter.hpp"
#include "common/Log.hpp"
#include <string>
#include <stdexcept>
#include <ctime>

#include <sstream>

using namespace std;
namespace ale {


class ALEInterface::Impl{
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
	  // ALE2: specify which core to load as well. Default is Atari.
	  void loadROM(std::string rom_file ="", std::string core_file = "");

	  // Applies an action to the game and returns the reward. It is the
	  // user's responsibility to check if the game has ended and reset
	  // when necessary - this method will keep pressing buttons on the
	  // game over screen.
	  reward_t act(Action action);

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
	  const int lives();

	  // Returns the frame number since the start of the current episode
	  int getEpisodeFrameNumber() const;

	  // Returns the current game screen
	  const ALEScreen &getScreen() const;

	  // Returns the current RAM content
	  const ALERAM &getRAM() const;

	  // Saves the state of the system
	  void saveState();

	  // Loads the state of the system
	  void loadState();

	  // This makes a copy of the environment state. This copy does *not* include pseudorandomness,
	  // making it suitable for planning purposes. By contrast, see cloneSystemState.
	  ALEState cloneState();

	  // Reverse operation of cloneState(). This does not restore pseudorandomness, so that repeated
	  // calls to restoreState() in the stochastic controls setting will not lead to the same outcomes.
	  // By contrast, see restoreSystemState.
	  void restoreState(const ALEState& state);

	  // This makes a copy of the system & environment state, suitable for serialization. This includes
	  // pseudorandomness and so is *not* suitable for planning purposes.
	  ALEState cloneSystemState();

	  // Reverse operation of cloneSystemState.
	  void restoreSystemState(const ALEState& state);

	  // Save the current screen as a png file
	  void saveScreenPNG(const std::string& filename);

	  // Creates a ScreenExporter object which can be used to save a sequence of frames. Ownership
	  // said object is passed to the caller. Frames are saved in the directory 'path', which needs
	  // to exists.
	  ScreenExporter *createScreenExporter(const std::string &path) const;

	  // static functions
	  void createAleSystem(std::shared_ptr<AleSystem> &theAleSystem,
	                      std::shared_ptr<Settings> &theSettings,
	                      std::shared_ptr<RetroAgent> &theRetroAgent);
	  void loadSettings(const std::string& romfile, const std::string& corefile,
	                     std::shared_ptr<AleSystem> &theSLESystem);



private:
	  std::shared_ptr<AleSystem> theAleSystem;
	  std::shared_ptr<Settings> theSettings;
	  std::shared_ptr<RetroAgent> theRetroAgent;
	  std::shared_ptr<RomSettings> romSettings;
	  std::shared_ptr<RetroEnvironment> environment;
	  int max_num_frames; // Maximum number of frames for each episode

};

ALEInterface::Impl::Impl(){
  ALEInterface::createAleSystem(theAleSystem, theSettings, theRetroAgent);
}

ALEInterface::Impl::~Impl() {}


// Get the value of a setting.
std::string ALEInterface::Impl::getString(const std::string& key) {
  assert(theSettings);
  return theSettings->getString(key);
}
int ALEInterface::Impl::getInt(const std::string& key) {
  assert(theSettings);
  return theSettings->getInt(key);
}
bool ALEInterface::Impl::getBool(const std::string& key) {
  assert(theSettings);
  return theSettings->getBool(key);
}
float ALEInterface::Impl::getFloat(const std::string& key) {
  assert(theSettings);
  return theSettings->getFloat(key);
}

// Set the value of a setting.
void ALEInterface::Impl::setString(const string& key, const string& value) {
  assert(theSettings);
  assert(theAleSystem);
  theSettings->setString(key, value);
  theSettings->validate();
}
void ALEInterface::Impl::setInt(const string& key, const int value) {
  assert(theSettings);
  assert(theAleSystem);
  theSettings->setInt(key, value);
  theSettings->validate();
}
void ALEInterface::Impl::setBool(const string& key, const bool value) {
  assert(theSettings);
  assert(theAleSystem);
  theSettings->setBool(key, value);
  theSettings->validate();
}
void ALEInterface::Impl::setFloat(const string& key, const float value) {
  assert(theSettings);
  assert(theAleSystem);
  theSettings->setFloat(key, value);
  theSettings->validate();
}

// Get the value of a setting.
std::string ALEInterface::getString(const std::string& key) {
	return m_pimpl->getString(key);
}
int ALEInterface::getInt(const std::string& key) {
	return m_pimpl->getInt(key);
}
bool ALEInterface::getBool(const std::string& key) {
	return m_pimpl->getBool(key);
}
float ALEInterface::getFloat(const std::string& key) {
	return m_pimpl->getFloat(key);
}

// Set the value of a setting.
void ALEInterface::setString(const string& key, const string& value) {
	m_pimpl->setString(key, value);
}
void ALEInterface::setInt(const string& key, const int value) {
	m_pimpl->setInt(key, value);
}
void ALEInterface::setBool(const string& key, const bool value) {
	m_pimpl->setBool(key, value);
}
void ALEInterface::setFloat(const string& key, const float value) {
	m_pimpl->setFloat(key, value);
}


ALEInterface::ALEInterface() : m_pimpl(new ALEInterface::Impl()) {
  disableBufferedIO();
  Logger::Info << welcomeMessage() << std::endl;
}

ALEInterface::ALEInterface(bool display_screen) : m_pimpl(new ALEInterface::Impl())  {
  disableBufferedIO();
  Logger::Info << welcomeMessage() << std::endl;
  m_pimpl->setBool("display_screen", display_screen);
}

ALEInterface::~ALEInterface() {
//	delete m_pimpl;
}

void ALEInterface::reset_game() {
	m_pimpl->reset_game();
}

// Resets the game, but not the full system.
void ALEInterface::Impl::reset_game() {
	environment->reset();
}


reward_t ALEInterface::act(Action action) {
    return m_pimpl->act(action);
}
reward_t ALEInterface::Impl::act(Action action) {
  reward_t reward = environment->act(action, PLAYER_B | JOYPAD_NOOP);
  if (theAleSystem->p_display_screen != NULL) {
    theAleSystem->p_display_screen->display_screen();
    while (theAleSystem->p_display_screen->manual_control_engaged()) {
      Action user_action = theAleSystem->p_display_screen->getUserAction();
      reward += environment->act(user_action, PLAYER_B | JOYPAD_NOOP);
      theAleSystem->p_display_screen->display_screen();
    }
  }
  return reward;
}

// Display ALE welcome message
std::string ALEInterface::welcomeMessage() {
  std::ostringstream oss;
  oss << "A.L.E: Arcade Learning Environment (version "
      << Version << ")\n"
      << "[Powered by Stella]\n"
      << "Use -help for help screen.";
  return oss.str();
}

void ALEInterface::disableBufferedIO() {
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);
  cin.rdbuf()->pubsetbuf(0,0);
  cout.rdbuf()->pubsetbuf(0,0);
  cin.sync_with_stdio();
  cout.sync_with_stdio();
}

void ALEInterface::createAleSystem(std::shared_ptr<AleSystem> &theAleSystem,
                          std::shared_ptr<Settings> &theSettings,
                          std::shared_ptr<RetroAgent> &theRetroAgent) {
#if (defined(WIN32) || defined(__MINGW32__))
	theRetroAgent.reset(new RetroAgent());
	theAleSystem.reset(new OSystemWin32());
	theSettings.reset(new SettingsWin32(theAleSystem.get()));
#else
	theRetroAgent.reset(new RetroAgent());
	theAleSystem.reset(new AleSystemUNIX(theRetroAgent.get()));
	theSettings.reset(new SettingsUNIX(theAleSystem.get()));
#endif

  theAleSystem->settings().loadConfig();
}

void ALEInterface::loadSettings(const string& romfile, const std::string& corefile,
                                std::shared_ptr<AleSystem> &theAleSystem) {
  // Load the configuration from a config file (passed on the command
  //  line), if provided
  string configFile = theAleSystem->settings().getString("config", false);

  if (!configFile.empty())
    theAleSystem->settings().loadConfig(configFile.c_str());

  theAleSystem->settings().validate();
  theAleSystem->create();

  // Attempt to load the ROM
//  if (romfile == "" || !FilesystemNode::fileExists(romfile)) {
//    Logger::Error << "No ROM File specified or the ROM file was not found."
//              << std::endl;
//    exit(1);
//  } else if (theAleSystem->createConsole(romfile))  {
//    Logger::Info << "Running ROM file..." << std::endl;
//    theAleSystem->settings().setString("rom_file", romfile);
//  } else {
//    exit(1);
//  }
  string corePath;
  if (romfile == "" || !FilesystemNode::fileExists(romfile)) {
	Logger::Error << "No ROM File specified or the ROM file was not found."
			<< std::endl;
	exit(1);
  }
  if(corefile == "atari"){
	  corePath = ATARI_PATH;
  }else if(corefile == "snes"){
	  corePath = SNES_PATH;
  }
	theAleSystem->loadCore(corePath);
	theAleSystem->loadRom(romfile);
	Logger::Info << "Running ROM file..." << std::endl;
	theAleSystem->settings().setString("rom_file", romfile);
	theAleSystem->p_display_screen = new DisplayScreen(&theAleSystem->getRetroAgent());

  // Must force the resetting of the OSystem's random seed, which is set before we change
  // choose our random seed.
  Logger::Info << "Random seed is " << theAleSystem->settings().getInt("random_seed") << std::endl;
  theAleSystem->resetRNGSeed();

//  TODO maybe add
//  string currentDisplayFormat = theAleSystem->console().getFormat();
//  theAleSystem->colourPalette().setPalette("standard", currentDisplayFormat);
}

void ALEInterface::getRGB(
    unsigned char pixel,
    unsigned char &red,
    unsigned char &green,
    unsigned char &blue
) {
//
//    static const int ntsc_tbl[] = {
//        0x000000, 0, 0x4a4a4a, 0, 0x6f6f6f, 0, 0x8e8e8e, 0,
//        0xaaaaaa, 0, 0xc0c0c0, 0, 0xd6d6d6, 0, 0xececec, 0,
//        0x484800, 0, 0x69690f, 0, 0x86861d, 0, 0xa2a22a, 0,
//        0xbbbb35, 0, 0xd2d240, 0, 0xe8e84a, 0, 0xfcfc54, 0,
//        0x7c2c00, 0, 0x904811, 0, 0xa26221, 0, 0xb47a30, 0,
//        0xc3903d, 0, 0xd2a44a, 0, 0xdfb755, 0, 0xecc860, 0,
//        0x901c00, 0, 0xa33915, 0, 0xb55328, 0, 0xc66c3a, 0,
//        0xd5824a, 0, 0xe39759, 0, 0xf0aa67, 0, 0xfcbc74, 0,
//        0x940000, 0, 0xa71a1a, 0, 0xb83232, 0, 0xc84848, 0,
//        0xd65c5c, 0, 0xe46f6f, 0, 0xf08080, 0, 0xfc9090, 0,
//        0x840064, 0, 0x97197a, 0, 0xa8308f, 0, 0xb846a2, 0,
//        0xc659b3, 0, 0xd46cc3, 0, 0xe07cd2, 0, 0xec8ce0, 0,
//        0x500084, 0, 0x68199a, 0, 0x7d30ad, 0, 0x9246c0, 0,
//        0xa459d0, 0, 0xb56ce0, 0, 0xc57cee, 0, 0xd48cfc, 0,
//        0x140090, 0, 0x331aa3, 0, 0x4e32b5, 0, 0x6848c6, 0,
//        0x7f5cd5, 0, 0x956fe3, 0, 0xa980f0, 0, 0xbc90fc, 0,
//        0x000094, 0, 0x181aa7, 0, 0x2d32b8, 0, 0x4248c8, 0,
//        0x545cd6, 0, 0x656fe4, 0, 0x7580f0, 0, 0x8490fc, 0,
//        0x001c88, 0, 0x183b9d, 0, 0x2d57b0, 0, 0x4272c2, 0,
//        0x548ad2, 0, 0x65a0e1, 0, 0x75b5ef, 0, 0x84c8fc, 0,
//        0x003064, 0, 0x185080, 0, 0x2d6d98, 0, 0x4288b0, 0,
//        0x54a0c5, 0, 0x65b7d9, 0, 0x75cceb, 0, 0x84e0fc, 0,
//        0x004030, 0, 0x18624e, 0, 0x2d8169, 0, 0x429e82, 0,
//        0x54b899, 0, 0x65d1ae, 0, 0x75e7c2, 0, 0x84fcd4, 0,
//        0x004400, 0, 0x1a661a, 0, 0x328432, 0, 0x48a048, 0,
//        0x5cba5c, 0, 0x6fd26f, 0, 0x80e880, 0, 0x90fc90, 0,
//        0x143c00, 0, 0x355f18, 0, 0x527e2d, 0, 0x6e9c42, 0,
//        0x87b754, 0, 0x9ed065, 0, 0xb4e775, 0, 0xc8fc84, 0,
//        0x303800, 0, 0x505916, 0, 0x6d762b, 0, 0x88923e, 0,
//        0xa0ab4f, 0, 0xb7c25f, 0, 0xccd86e, 0, 0xe0ec7c, 0,
//        0x482c00, 0, 0x694d14, 0, 0x866a26, 0, 0xa28638, 0,
//        0xbb9f47, 0, 0xd2b656, 0, 0xe8cc63, 0, 0xfce070, 0
//    };
//
//    int c = ntsc_tbl[pixel];
//    red   = c >> 16;
//    green = (c >> 8) & 0xFF;
//    blue  = c & 0xFF;
}

void ALEInterface::loadROM(string rom_file, string core_file) {
	m_pimpl->loadROM(rom_file, core_file);
}

// Loads and initializes a game. After this call the game should be
// ready to play. Resets the OSystem/Console/Environment/etc. This is
// necessary after changing a setting. Optionally specify a new rom to
// load.
void ALEInterface::Impl::loadROM(string rom_file, string core_file) {
  assert(theAleSystem.get());
  if (rom_file.empty()) {
    rom_file = theAleSystem->romFile();
  }
  if (core_file.empty()) {
	core_file = theAleSystem->coreFile();
  }
  ALEInterface::loadSettings(rom_file, core_file, theAleSystem);
  romSettings.reset(buildRomRLWrapper(rom_file));
  environment.reset(new RetroEnvironment(theAleSystem.get(), romSettings.get()));
  max_num_frames = theAleSystem->settings().getInt("max_num_frames_per_episode");
  environment->reset();
//#ifndef __USE_SDL
//  if (theAleSystem->p_display_screen != NULL) {
//    Logger::Error << "Screen display requires directive __USE_SDL to be defined." << endl;
//    Logger::Error << "Please recompile this code with flag '-D__USE_SDL'." << endl;
//    Logger::Error << "Also ensure ALE has been compiled with USE_SDL active (see ALE makefile)." << endl;
//    exit(1);
//  }
//#endif
}

bool ALEInterface::game_over() const{
	return m_pimpl->game_over();
}

// Indicates if the game has ended.
bool ALEInterface::Impl::game_over() const{
  return (environment->isTerminal() ||
          (max_num_frames > 0 && getEpisodeFrameNumber() >= max_num_frames));
}

const int ALEInterface::lives() {
	return m_pimpl->lives();
}

// The remaining number of lives.
const int ALEInterface::Impl::lives() {
  if (!romSettings.get()){
    throw std::runtime_error("ROM not set");
  }
  return romSettings->lives();
}

ActionVect ALEInterface::getLegalActionSet() {
	return m_pimpl->getLegalActionSet();
}

// TODO SN : replace function with something else since the vector of all legal actions woul dbe too big
// Returns the vector of legal actions. This should be called only
// after the rom is loaded.
ActionVect ALEInterface::Impl::getLegalActionSet() {
  if (!romSettings.get()){
    throw std::runtime_error("ROM not set");
  }
  return romSettings->getAllActions();
}

ActionVect ALEInterface::getMinimalActionSet() {
	return m_pimpl->getMinimalActionSet();
}

// Returns the vector of the minimal set of actions needed to play
// the game.
ActionVect ALEInterface::Impl::getMinimalActionSet() {
  if (!romSettings.get()){
    throw std::runtime_error("ROM not set");
  }
  return romSettings->getMinimalActionSet();
}

int ALEInterface::getFrameNumber() {
  return m_pimpl->getFrameNumber();
}

// Returns the frame number since the loading of the ROM
int ALEInterface::Impl::getFrameNumber() {
  return environment->getFrameNumber();
}

int ALEInterface::getEpisodeFrameNumber() const{
  return m_pimpl->getEpisodeFrameNumber();
}

// Returns the frame number since the start of the current episode
int ALEInterface::Impl::getEpisodeFrameNumber() const{
  return environment->getEpisodeFrameNumber();
}

const ALEScreen& ALEInterface::getScreen() const {
  return m_pimpl->getScreen();
}

// Returns the current game screen
const ALEScreen& ALEInterface::Impl::getScreen() const{
  return environment->getScreen();
}

const ALERAM& ALEInterface::getRAM() const{
  return m_pimpl->getRAM();
}

// Returns the current RAM content
const ALERAM& ALEInterface::Impl::getRAM() const{
  return environment->getRAM();
}

void ALEInterface::saveState() {
	m_pimpl->saveState();
}

// Saves the state of the system
void ALEInterface::Impl::saveState() {
  environment->save();
}

void ALEInterface::loadState() {
	m_pimpl->loadState();
}

// Loads the state of the system
void ALEInterface::Impl::loadState() {
  environment->load();
}

ALEState ALEInterface::cloneState() {
  return m_pimpl->cloneState();
}

ALEState ALEInterface::Impl::cloneState() {
  return environment->cloneState();
}

void ALEInterface::restoreState(const ALEState& state) {
  return m_pimpl->restoreState(state);
}

void ALEInterface::Impl::restoreState(const ALEState& state) {
  return environment->restoreState(state);
}

ALEState ALEInterface::cloneSystemState() {
  return m_pimpl->cloneSystemState();
}

ALEState ALEInterface::Impl::cloneSystemState() {
  return environment->cloneSystemState();
}

void ALEInterface::restoreSystemState(const ALEState& state) {
  return m_pimpl->restoreSystemState(state);
}

void ALEInterface::Impl::restoreSystemState(const ALEState& state) {
  return environment->restoreSystemState(state);
}

void ALEInterface::saveScreenPNG(const string& filename) {
	m_pimpl->saveScreenPNG(filename);
}

void ALEInterface::Impl::saveScreenPNG(const string& filename) {
  
//  ScreenExporter exporter(theAleSystem->colourPalette());
//  exporter.save(environment->getScreen(), filename);
}

ScreenExporter *ALEInterface::createScreenExporter(const std::string &filename) const {
	return m_pimpl->createScreenExporter(filename);
}

ScreenExporter *ALEInterface::Impl::createScreenExporter(const std::string &filename) const {
//    return new ScreenExporter(theAleSystem->colourPalette(), filename);
}

/* --------------------------------------------------------------------------------------------------*/

// RAM & Screen implementation

ALEScreen& ALEScreen::operator=(const ALEScreen &rhs) {

  m_rows = rhs.m_rows;
  m_columns = rhs.m_columns;

  m_pixels = rhs.m_pixels;

  return *this;
}
bool ALEScreen::equals(const ALEScreen &rhs) const {
  return (m_rows == rhs.m_rows &&
          m_columns == rhs.m_columns &&
          (memcmp(&m_pixels[0], &rhs.m_pixels[0], arraySize()) == 0) );
}

// pixel accessors, (row, column)-ordered
inline pixel_t ALEScreen::get(int r, int c) const {
  // Perform some bounds-checking
  assert (r >= 0 && r < m_rows && c >= 0 && c < m_columns);
  return m_pixels[r * m_columns + c];
}

inline pixel_t* ALEScreen::pixel(int r, int c) {
  // Perform some bounds-checking
  assert (r >= 0 && r < m_rows && c >= 0 && c < m_columns);
  return &m_pixels[r * m_columns + c];
}

// Access a whole row
inline pixel_t* ALEScreen::getRow(int r) const {
  assert (r >= 0 && r < m_rows);
  return const_cast<pixel_t*>(&m_pixels[r * m_columns]);
}


//inline ALERAM::ALERAM() {
//}

inline ALERAM::ALERAM(const ALERAM &rhs) {
  // Copy data over
//  memcpy(m_ram, rhs.m_ram, sizeof(m_ram));
}

inline ALERAM& ALERAM::operator=(const ALERAM &rhs) {
  // Copy data over
//  memcpy(m_ram, rhs.m_ram, sizeof(m_ram));
  m_ram = rhs.m_ram;
  return *this;
}

inline bool ALERAM::equals(const ALERAM &rhs) const {
  return (m_ram == rhs.m_ram);
}

// Byte accessors
inline byte_t ALERAM::get(unsigned int x) const {
  // Wrap RAM around the first 128 bytes
//  return m_ram[x & 0x7F];
  return m_ram.at(x);
}

//inline byte_t* ALERAM::byte(unsigned int x) {
//  return &m_ram[x & 0x7F];
//}

} // namespace ale
