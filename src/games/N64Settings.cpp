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
#include "N64Settings.hpp"
#include "Constants.h"

using namespace ale;

N64Settings::N64Settings(){

	// since every action is represented as a bit in uInt32, in order to get all possible actions we
	// iterate over all possible combination excluding up-down, left-right, start and select.
	// And action can't be a combination of a c-direction and a d-direction
	for(int iup(0),up(0); iup <= 1 ; iup++, up+=JOYPAD_UP){
		for(int idown(0), down(0); idown <= 1 ; idown++, down+=JOYPAD_DOWN){
			if(down != 0 && up != 0) continue;
			for(int ileft(0), left(0); ileft <= 1 ; ileft++, left+=JOYPAD_LEFT){
				for(int iright(0), right(0); iright <= 1 ; iright++, right+=JOYPAD_RIGHT){
					if(left != 0 && right != 0) continue;
					for(int ia(0), a(0); ia <= 1 ; ia++, a+=JOYPAD_A){
							for(int ia(0), a(0); ia <= 1 ; ia++, a+=JOYPAD_A){
								for(int ib(0), b(0); ib <= 1 ; ib++, b+=JOYPAD_B){
									for(int iz(0), z(0); iz <= 1 ; iz++, z+=JOYPAD_Z){
										for(int ir(0), r(0); ir <= 1 ; ir++, r+=JOYPAD_R){
											for(int il(0), l(0); il <= 1 ; il++, l+=JOYPAD_L){
												for(int idup(0),dup(0); idup <= 1 ; idup++, dup+=JOYPAD_DUP){
													for(int iddown(0), ddown(0); iddown <= 1 ; iddown++, ddown+=JOYPAD_DDOWN){
														if(ddown != 0 && dup != 0) continue;
														for(int idleft(0), dleft(0); idleft <= 1 ; idleft++, dleft+=JOYPAD_DLEFT){
															for(int idright(0), dright(0); idright <= 1 ; idright++, dright+=JOYPAD_DRIGHT){
																if(dleft != 0 && dright != 0) continue;
																	AllActionsVector.push_back(up | down | left | right |
																							   dup | ddown | dleft | dright |
																							   a | b | z | r | l);
															}
														}
													}
												}
												for(int icup(0),cup(0); icup <= 1 ; icup++, cup+=JOYPAD_CUP){
													for(int icdown(0), cdown(0); icdown <= 1 ; icdown++, cdown+=JOYPAD_CDOWN){
														if(cdown != 0 && cup != 0) continue;
														for(int icleft(0), cleft(0); icleft <= 1 ; icleft++, cleft+=JOYPAD_CLEFT){
															for(int icright(0), cright(0); icright <= 1 ; icright++, cright+=JOYPAD_CRIGHT){
																if(cleft != 0 && cright != 0) continue;
																	AllActionsVector.push_back(up | down | left | right |
																							   cup | cdown | cleft | cright |
																							   a | b | z | r | l);
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
					}
				}
			}
		}

//	for (ActionVect::iterator it = AllActionsVector.begin() ; it != AllActionsVector.end(); ++it){
//		cout << action_to_string(*it) << endl;
//	}

}
