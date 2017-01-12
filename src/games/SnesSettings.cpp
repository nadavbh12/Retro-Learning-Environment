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

using namespace rle;

SnesSettings::SnesSettings(){

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
							for(int ix(0), x(0); ix <= 1 ; ix++, x+=JOYPAD_X){
								for(int iy(0), y(0); iy <= 1 ; iy++, y+=JOYPAD_Y){
									for(int ir(0), r(0); ir <= 1 ; ir++, r+=JOYPAD_R){
										for(int il(0), l(0); il <= 1 ; il++, l+=JOYPAD_L){
											AllActionsVector.push_back(up | down | left | right | a | b | x | y | r | l);
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


//	for (ActionVect::iterator it = AllActionsVector.begin() ; it != AllActionsVector.end(); ++it){
//		cout << action_to_string(*it) << endl;
//	}

}

/* is an action part of the minimal set? */
bool SnesSettings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
}
