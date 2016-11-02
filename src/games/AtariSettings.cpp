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
 *
 * RomSettings.cpp
 *
 * The interface to describe games as RL environments. It provides terminal and
 *  reward information.
 * *****************************************************************************
 */
#include "AtariSettings.hpp"
#include "Constants.h"

using namespace rle;
AtariSettings::AtariSettings(){
	AllActionsVector = {
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
}

int AtariSettings::readRam(const RleSystem* system, int offset){
	RomSettings::readRam(system, offset & 0x7F );
}
