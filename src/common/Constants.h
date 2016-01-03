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
 *  common_constants.h
 *
 *  Defines a set of constants used by various parts of the player agent code
 *
 **************************************************************************** */

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "DebugMacros.h"
#include <cassert>
#include <vector>
#include <valarray>
#include <cstdlib>
//#include "../emucore/m6502/src/bspf/src/bspf.hxx"
// SN:
#include <iostream>
#include "DebugMacros.h"
#include <stdint.h>
#include "../libretro.h"


// Define actions
//enum Action {
//    PLAYER_A_NOOP           = 0,
//    PLAYER_A_FIRE           = 1,
//    PLAYER_A_UP             = 2,
//    PLAYER_A_RIGHT          = 3,
//    PLAYER_A_LEFT           = 4,
//    PLAYER_A_DOWN           = 5,
//    PLAYER_A_UPRIGHT        = 6,
//    PLAYER_A_UPLEFT         = 7,
//    PLAYER_A_DOWNRIGHT      = 8,
//    PLAYER_A_DOWNLEFT       = 9,
//    PLAYER_A_UPFIRE         = 10,
//    PLAYER_A_RIGHTFIRE      = 11,
//    PLAYER_A_LEFTFIRE       = 12,
//    PLAYER_A_DOWNFIRE       = 13,
//    PLAYER_A_UPRIGHTFIRE    = 14,
//    PLAYER_A_UPLEFTFIRE     = 15,
//    PLAYER_A_DOWNRIGHTFIRE  = 16,
//    PLAYER_A_DOWNLEFTFIRE   = 17,
//    PLAYER_B_NOOP           = 18,
//    PLAYER_B_FIRE           = 19,
//    PLAYER_B_UP             = 20,
//    PLAYER_B_RIGHT          = 21,
//    PLAYER_B_LEFT           = 22,
//    PLAYER_B_DOWN           = 23,
//    PLAYER_B_UPRIGHT        = 24,
//    PLAYER_B_UPLEFT         = 25,
//    PLAYER_B_DOWNRIGHT      = 26,
//    PLAYER_B_DOWNLEFT       = 27,
//    PLAYER_B_UPFIRE         = 28,
//    PLAYER_B_RIGHTFIRE      = 29,
//    PLAYER_B_LEFTFIRE       = 30,
//    PLAYER_B_DOWNFIRE       = 31,
//    PLAYER_B_UPRIGHTFIRE    = 32,
//    PLAYER_B_UPLEFTFIRE     = 33,
//    PLAYER_B_DOWNRIGHTFIRE  = 34,
//    PLAYER_B_DOWNLEFTFIRE   = 35,
//    RESET                   = 40, // MGB: Use SYSTEM_RESET to reset the environment.
//    UNDEFINED               = 41,
//    RANDOM                  = 42,
//    SAVE_STATE              = 43,
//    LOAD_STATE              = 44,
//    SYSTEM_RESET            = 45,
//    LAST_ACTION_INDEX       = 50
//};

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

//#define PLAYER_A_MAX (18)
//#define PLAYER_B_MAX (36)

// TODO SN : implement
//std::string action_to_string(Action a){EMPTY_FUNCTION}

//  Define datatypes
typedef std::vector<Action> ActionVect;

// reward type for RL interface
typedef int reward_t;

// Other constant values
#define RAM_LENGTH 128

// Atari actions
#define JOYPAD_FIRE					JOYPAD_B	// for atari

static const ActionVect AtariAllActionsVector{
	JOYPAD_NOOP,
	JOYPAD_FIRE,
	JOYPAD_UP,
	JOYPAD_RIGHT,
	JOYPAD_LEFT,
	JOYPAD_DOWN,
	JOYPAD_UP | JOYPAD_RIGHT,
	JOYPAD_UP | JOYPAD_LEFT,
	JOYPAD_DOWN | JOYPAD_RIGHT,
	JOYPAD_DOWN | JOYPAD_LEFT,
	JOYPAD_UP | JOYPAD_FIRE,
	JOYPAD_RIGHT | JOYPAD_FIRE,
	JOYPAD_LEFT | JOYPAD_FIRE,
	JOYPAD_DOWN | JOYPAD_FIRE,
	JOYPAD_UP | JOYPAD_RIGHT | JOYPAD_FIRE,
	JOYPAD_UP | JOYPAD_LEFT | JOYPAD_FIRE,
	JOYPAD_DOWN | JOYPAD_RIGHT | JOYPAD_FIRE,
	JOYPAD_DOWN | JOYPAD_LEFT | JOYPAD_FIRE
};

#endif // __CONSTANTS_H__

