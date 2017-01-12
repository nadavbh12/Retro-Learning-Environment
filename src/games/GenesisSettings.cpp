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
#include "GenesisSettings.hpp"
#include "Constants.h"

using namespace rle;

GenesisSettings::GenesisSettings(){

	// since every action is represented as a bit in uInt32, in order to get all possible actions we
	// iterate over all possible combination excluding up-down, left-right, start and select.
	for(int iup(0),up(0); iup <= 1 ; iup++, up+=JOYPAD_UP){
		for(int idown(0), down(0); idown <= 1 ; idown++, down+=JOYPAD_DOWN){
			if(down != 0 && up != 0) continue;
			for(int ileft(0), left(0); ileft <= 1 ; ileft++, left+=JOYPAD_LEFT){
				for(int iright(0), right(0); iright <= 1 ; iright++, right+=JOYPAD_RIGHT){
					if(left != 0 && right != 0) continue;
					for(int ia(0), a(0); ia <= 1 ; ia++, a+=JOYPAD_GENESIS_A){
						for(int ib(0), b(0); ib <= 1 ; ib++, b+=JOYPAD_GENESIS_B){
							for(int ic(0), c(0); ic <= 1 ; ic++, c+=JOYPAD_GENESIS_C){
								for(int ix(0), x(0); ix <= 1 ; ix++, x+=JOYPAD_GENESIS_X){
									for(int iy(0), y(0); iy <= 1 ; iy++, y+=JOYPAD_GENESIS_Y){
										for(int iz(0), z(0); iz <= 1 ; iz++, z+=JOYPAD_GENESIS_Z){
											AllActionsVector.push_back(up | down | left | right | a | b | c | x | y | z);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// remove the first element which is 0
	AllActionsVector.erase(AllActionsVector.begin());

	AllActionsVector.push_back(JOYPAD_NOOP);
}

/* is an action part of the minimal set? */
bool GenesisSettings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
}
