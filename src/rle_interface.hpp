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
 * *****************************************************************************
 * R.L.E (Retro Learning Environment)
 * Copyright (c) 2016-2017 by Shai Rozenberg, Nadav Bhonker and Itay Hubara.
 *
 * Based on: A.L.E (see above)
 * Released under the GNU General Public License; see License.txt for details.
 * *****************************************************************************
 *  rle_interface.hpp
 *
 *  The shared library interface.
 **************************************************************************** */
#ifndef __RLE_INTERFACE_HPP__
#define __RLE_INTERFACE_HPP__

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include "libretro.h"
#include <string>
#include <assert.h>

namespace rle {

static const std::string Version = "1.1.1";

class RleSystem;
class Settings;
struct pixelFormat;
struct RomSettings;
class RetroEnvironment;
class RLEState;
class ScreenExporter;
class RetroAgent;

typedef uint32_t Action;
// actions are represented as uint's where each bit represents whether a "button" is pressed or not.
// Each bit matches the action as defined in libretro.h under RETRO_DEVICE_ID_JOYPAD
// For example: action = 3, means = B + Y
// The msb defines whether the action is player a or player b
// special actions:
#define JOYPAD_NOOP					(unsigned int)0
#define JOYPAD_B                    (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_B     )
#define JOYPAD_Y                    (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_Y     )
#define JOYPAD_SELECT               (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_SELECT)
#define JOYPAD_START                (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_START )
#define JOYPAD_UP                   (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_UP    )
#define JOYPAD_DOWN                 (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_DOWN  )
#define JOYPAD_LEFT                 (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_LEFT  )
#define JOYPAD_RIGHT                (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_RIGHT )
#define JOYPAD_A                    (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_A     )
#define JOYPAD_X                    (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_X     )
#define JOYPAD_L                    (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_L     )
#define JOYPAD_R                    (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_R     )
#define JOYPAD_L2                   (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_L2    )
#define JOYPAD_R2                   (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_R2    )
#define JOYPAD_L3                   (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_L3    )
#define JOYPAD_R3                   (unsigned int)(1 << RETRO_DEVICE_ID_JOYPAD_R3    )
#define JOYPAD_RESET 				(unsigned int)(1 << 16)
#define JOYPAD_UNDEFINED 			(unsigned int)(1 << 17)
#define JOYPAD_RANDOM	 			(unsigned int)(1 << 18)
#define JOYPAD_SAVE_STATE 			(unsigned int)(1 << 19)
#define JOYPAD_LOAD_STATE 			(unsigned int)(1 << 20)
#define JOYPAD_SYSTEM_RESET 		(unsigned int)(1 << 21)
#define JOYPAD_LAST_ACTION_INDEX 	(unsigned int)(1 << 22)

#define PLAYER_A					0
#define PLAYER_B					(unsigned int)(1 << 31)

// Atari actions
#define JOYPAD_FIRE					JOYPAD_B	// for atari

// Genesis actions
#define JOYPAD_GENESIS_A			JOYPAD_Y
#define JOYPAD_GENESIS_B			JOYPAD_B
#define JOYPAD_GENESIS_C			JOYPAD_A
#define JOYPAD_GENESIS_X			JOYPAD_L
#define JOYPAD_GENESIS_Y			JOYPAD_X
#define JOYPAD_GENESIS_Z			JOYPAD_R

// a list of RLE actions
typedef std::vector<Action> ActionVect;

// type used to represent insantanteous reward
typedef int reward_t;

typedef uint32_t pixel_t;

typedef unsigned char byte_t;

/** A simple wrapper around an Atari screen. */
class RLEScreen {
  public:
    RLEScreen(int h, int w);
    RLEScreen(const RLEScreen &rhs);
    ~RLEScreen();
    RLEScreen& operator=(const RLEScreen &rhs);

    /** pixel accessors, (row, column)-ordered */
    pixel_t get(int r, int c) const;
    pixel_t *pixel(int r, int c);

    /** Access a whole row */
    pixel_t *getRow(int r) const;

    /** Access the whole array */
    const pixel_t *getArray() const { return &m_pixels[0]; }
//    std::vector<pixel_t>& getArray() { return m_pixels; }

    /** Dimensionality information - values are in pixels */
    size_t height() const { return m_rows; }
    size_t width() const { return m_columns ; }
    int getBpp() const;

    /** Returns the size of the underlying array */
    size_t arraySize() const { return m_pixels.size(); }

    /** Returns whether two screens are equal */
    bool equals(const RLEScreen &rhs) const;

    /** Returns a 32bit pixel in an R|G|B Format, 8 bits each */
    pixel_t getRGBPixel(const uint32_t &pixel)const;
    void  getRGB(const uint32_t &pixel ,uint8_t &red, uint8_t &green, uint8_t &blue)const;

    struct pixelFormat* m_pixelFormat;

  protected:
    int m_rows;
    int m_columns; // in Pixels
    std::vector<pixel_t> m_pixels;

};


/** A simple wrapper around the Core's RAM. Values are Read/Write!*/
class RLERAM {
  public:
    RLERAM(size_t size, byte_t* bytes) : m_ram(bytes), m_ram_size(size){}

    /** Byte accessors */
    byte_t get(unsigned int x) const;
    byte_t *byte(unsigned int x);

    /** Returns the whole array. */
    byte_t* array() const { return m_ram;}

    size_t size() const { return m_ram_size; }

    /** Returns whether two copies of the RAM are equal */
    bool equals(const RLERAM &rhs) const;

  protected:
    byte_t* m_ram;
    size_t 	m_ram_size;
};

/**
   This class interfaces RLE with external code for controlling agents.
 */
class RLEInterface {
public:

    /** create an RLEInterface. Cannot create two instances within one thread */
  RLEInterface();

  /** create an RLEInterface. Cannot create two instances within one thread */
  RLEInterface(const std::string &rom_file, const std::string &core_file);

  // Legacy constructor
  RLEInterface(bool display_screen);

  /** Unload the emulator. */
  ~RLEInterface();

  /** Copying is explicitly disallowed. */
  RLEInterface(const RLEInterface &) = delete;

  /** Assignment is explicitly disallowed. */
  RLEInterface &operator=(const RLEInterface &) = delete;

  // Resets the game, but not the full system.
  void reset_game();

  // Resets the Atari and loads a game. After this call the game
  // should be ready to play. This is necessary after changing a
  // setting for the setting to take effect.
  // RLE: specify which core to load as well. Default is Atari.
  void loadROM(std::string rom_file, std::string core_file = "");

  // Applies an action to the game and returns the reward. It is the
  // user's responsibility to check if the game has ended and reset
  // when necessary - this method will keep pressing buttons on the
  // game over screen.
  reward_t act(Action actionA, Action actionB = (PLAYER_B | JOYPAD_NOOP));

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
//  ScreenExporter *createScreenExporter(const std::string &path) const;

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

  // Display RLE welcome message
  static std::string welcomeMessage();
  static void disableBufferedIO();
  static void createRleSystem(std::shared_ptr<RleSystem>& theRleSystem,
                            std::shared_ptr<Settings>& theSettings,
                            std::shared_ptr<RetroAgent>& theRetroAgent);
  static void loadSettings(const std::string& romfile, const std::string& corefile,
                           std::shared_ptr<RleSystem> &theRleSystem);
//  TODO: add loadConfigFile function
 private:
   class Impl;
   Impl* m_pimpl;
};

} // namespace rle
#endif // __RLE_INTERFACE_HPP__
