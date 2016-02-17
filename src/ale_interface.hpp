/* *****************************************************************************
 * The line 99 is based on Xitari's code, from Google Inc.
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
 *  ale_interface.hpp
 *
 *  The shared library interface.
 **************************************************************************** */
#ifndef __ALE_INTERFACE_HPP__
#define __ALE_INTERFACE_HPP__

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include "libretro.h"
#include <string>

// SN
#include <assert.h>

namespace ale {

static const std::string Version = "0.5.0";

class AleSystem;
class Settings;

struct RomSettings;
class RetroEnvironment;
class ALEState;
class ScreenExporter;
class RetroAgent;

typedef uint32_t Action;
// actions are represented as in uint where each bit represents whether a "button" is pressed or not.
// Each bit matches the action as defined in libretro.h under RETRO_DEVICE_ID_JOYPAD
// For example: action = 3, means = B + Y
// The msb defines whether the action is player a or player b
// special actions:
#define JOYPAD_B                    (1 << RETRO_DEVICE_ID_JOYPAD_B     )
#define JOYPAD_Y                    (1 << RETRO_DEVICE_ID_JOYPAD_Y     )
#define JOYPAD_SELECT               (1 << RETRO_DEVICE_ID_JOYPAD_SELECT)
#define JOYPAD_START                (1 << RETRO_DEVICE_ID_JOYPAD_START )
#define JOYPAD_UP                   (1 << RETRO_DEVICE_ID_JOYPAD_UP    )
#define JOYPAD_DOWN                 (1 << RETRO_DEVICE_ID_JOYPAD_DOWN  )
#define JOYPAD_LEFT                 (1 << RETRO_DEVICE_ID_JOYPAD_LEFT  )
#define JOYPAD_RIGHT                (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT )
#define JOYPAD_A                    (1 << RETRO_DEVICE_ID_JOYPAD_A     )
#define JOYPAD_X                    (1 << RETRO_DEVICE_ID_JOYPAD_X     )
#define JOYPAD_L                    (1 << RETRO_DEVICE_ID_JOYPAD_L     )
#define JOYPAD_R                    (1 << RETRO_DEVICE_ID_JOYPAD_R     )
#define JOYPAD_L2                   (1 << RETRO_DEVICE_ID_JOYPAD_L2    )
#define JOYPAD_R2                   (1 << RETRO_DEVICE_ID_JOYPAD_R2    )
#define JOYPAD_L3                   (1 << RETRO_DEVICE_ID_JOYPAD_L3    )
#define JOYPAD_R3                   (1 << RETRO_DEVICE_ID_JOYPAD_R3    )
#define JOYPAD_NOOP 				(1 << 16)
#define JOYPAD_RESET 				(1 << 17)
#define JOYPAD_UNDEFINED 			(1 << 18)
#define JOYPAD_RANDOM	 			(1 << 19)
#define JOYPAD_SAVE_STATE 			(1 << 20)
#define JOYPAD_LOAD_STATE 			(1 << 21)
#define JOYPAD_SYSTEM_RESET 		(1 << 22)
#define JOYPAD_LAST_ACTION_INDEX 	(1 << 23)

#define PLAYER_A					0
#define PLAYER_B					(1 << 31)

// Other constant values
#define RAM_LENGTH 128

// Atari actions
#define JOYPAD_FIRE					JOYPAD_B	// for atari

// a list of ALE actions
typedef std::vector<Action> ActionVect;

// type used to represent insantanteous reward
typedef int reward_t;

typedef unsigned char pixel_t;
typedef unsigned char byte_t;

/** A simple wrapper around an Atari screen. */
class ALEScreen {
  public:
    ALEScreen(int h, int w) : m_rows(h), m_columns(w), m_pixels(m_rows * m_columns) {}
    ALEScreen(const ALEScreen &rhs): m_rows(rhs.m_rows), m_columns(rhs.m_columns), m_pixels(rhs.m_pixels) {}

    ALEScreen& operator=(const ALEScreen &rhs);

    /** pixel accessors, (row, column)-ordered */
    pixel_t get(int r, int c) const;
    pixel_t *pixel(int r, int c);

    /** Access a whole row */
    pixel_t *getRow(int r) const;

    /** Access the whole array */
    pixel_t *getArray() const { return const_cast<pixel_t *>(&m_pixels[0]); }

    /** Dimensionality information */
    size_t height() const { return m_rows; }
    size_t width() const { return m_columns; }

    /** Returns the size of the underlying array */
    size_t arraySize() const { return m_rows * m_columns * sizeof(pixel_t); }

    /** Returns whether two screens are equal */
    bool equals(const ALEScreen &rhs) const;

  protected:
    int m_rows;
    int m_columns;

    std::vector<pixel_t> m_pixels;
};

//#define RAM_SIZE (128)

/** A simple wrapper around the Atari RAM. */
class ALERAM {
  public:
    ALERAM(){}
    ALERAM(const ALERAM &rhs);

    ALERAM& operator=(const ALERAM &rhs);

    /** Byte accessors */
    byte_t get(unsigned int x) const;
    byte_t *byte(unsigned int x);

    /** Returns the whole array. */
    std::vector<byte_t> array() { return m_ram;}

    size_t size() const { return sizeof(m_ram); }
    /** Returns whether two copies of the RAM are equal */
    bool equals(const ALERAM &rhs) const;

  protected:
//    byte_t m_ram[RAM_SIZE];
    std::vector<byte_t> m_ram;
};


/**
   This class interfaces ALE with external code for controlling agents.
 */
class ALEInterface {
public:

    /** create an ALEInterface. This routine is not threadsafe! */
  ALEInterface();


  // Legacy constructor
  ALEInterface(bool display_screen);

  /** Unload the emulator. */
  ~ALEInterface();

  // Resets the game, but not the full system.
  void reset_game();

  // Resets the Atari and loads a game. After this call the game
  // should be ready to play. This is necessary after changing a
  // setting for the setting to take effect.
  // ALE2: specify which core to load as well. Default is Atari.
  void loadROM(std::string rom_file, std::string core_file = "");

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

  // Display ALE welcome message
  static std::string welcomeMessage();
  static void disableBufferedIO();
  static void createAleSystem(std::shared_ptr<AleSystem> &theAleSystem,
                            std::shared_ptr<Settings> &theSettings,
                            std::shared_ptr<RetroAgent> &theRetroAgent);
  static void loadSettings(const std::string& romfile, const std::string& corefile,
                           std::shared_ptr<AleSystem> &theSLESystem);
  static void getRGB(
      unsigned char pixel,
      unsigned char &red,
      unsigned char &green,
      unsigned char &blue
  );

 private:

   /** Copying is explicitly disallowed. */
   ALEInterface(const ALEInterface &);

   /** Assignment is explicitly disallowed. */
   ALEInterface &operator=(const ALEInterface &);

   class Impl;
   Impl* m_pimpl;
};

} // namespace ale
#endif // __ALE_INTERFACE_HPP__
