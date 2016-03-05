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
#include "NesSettings.hpp"
#include "Constants.h"

using namespace ale;

NesSettings::NesSettings(){

	AllActionsVector = {
				JOYPAD_NOOP,
				JOYPAD_A,
				JOYPAD_B,
				JOYPAD_UP,
				JOYPAD_RIGHT,
				JOYPAD_LEFT,
				JOYPAD_DOWN,
				JOYPAD_UP | JOYPAD_RIGHT,
				JOYPAD_UP | JOYPAD_LEFT,
				JOYPAD_DOWN | JOYPAD_RIGHT,
				JOYPAD_DOWN | JOYPAD_LEFT,
				JOYPAD_UP | JOYPAD_A,
				JOYPAD_RIGHT | JOYPAD_A,
				JOYPAD_LEFT | JOYPAD_A,
				JOYPAD_DOWN | JOYPAD_A,
				JOYPAD_UP | JOYPAD_RIGHT | JOYPAD_A,
				JOYPAD_UP | JOYPAD_LEFT | JOYPAD_A,
				JOYPAD_DOWN | JOYPAD_RIGHT | JOYPAD_A,
				JOYPAD_DOWN | JOYPAD_LEFT | JOYPAD_A
			};

	// since every action is represented as a bit in uInt32, in order to get all possible actions we
	// iterate over all possible combination excluding up-down, left-right, start and select.
	for(int iup(0),up(0); iup <= 1 ; iup++, up+=JOYPAD_UP){
		for(int idown(0), down(0); idown <= 1 ; idown++, down+=JOYPAD_DOWN){
			if(down != 0 && up != 0) continue;
			for(int ileft(0), left(0); ileft <= 1 ; ileft++, left+=JOYPAD_LEFT){
				for(int iright(0), right(0); iright <= 1 ; iright++, right+=JOYPAD_RIGHT){
					if(left != 0 && right != 0) continue;
					for(int ia(0), a(0); ia <= 1 ; ia++, a+=JOYPAD_A){
						for(int ib(0), b(0); ib <= 1 ; ib++, b+=JOYPAD_B){
								AllActionsVector.push_back(up | down | left | right | a | b);
						}
					}
				}
			}
		}
	}

	// remove the first element which is 0
	AllActionsVector.erase(AllActionsVector.begin());

	AllActionsVector.push_back(JOYPAD_NOOP);
	AllActionsVector.push_back(JOYPAD_SELECT);


}
