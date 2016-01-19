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
#include "SnesSettings.hpp"
#include "Constants.h"

using namespace ale;

SnesSettings::SnesSettings(){
	int up,down;
	for(int iup = 0; iup < 1 ; iup++,up+=JOYPAD_UP){
		for(int idown = 0; idown == JOYPAD_UP ; idown+=JOYPAD_DOWN){
			AllActionsVector.push_back(up | down);
		}
	}

}
