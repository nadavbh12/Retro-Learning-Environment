///* *****************************************************************************
// * A.L.E (Arcade Learning Environment)
// * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
// *   the Reinforcement Learning and Artificial Intelligence Laboratory
// * Released under the GNU General Public License; see License.txt for details.
// *
// * Based on: Stella  --  "An Atari 2600 VCS Emulator"
// * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
// *
// * *****************************************************************************
// *
// * RomSettings.cpp
// *
// * The interface to describe games as RL environments. It provides terminal and
// *  reward information.
// * *****************************************************************************
// */
//#include "SnesSettings.hpp"
//#include "Constants.h"
//
//ActionVect SnesSettings::getMinimalActionSet() {
//  ActionVect actions;
//  for (unsigned a = 0; a < AtariAllActionsVector.size(); a++) {
//    if (isMinimal(AtariAllActionsVector.at(a)) && isLegal(AtariAllActionsVector.at(a))) {
//      actions.push_back(AtariAllActionsVector.at(a));
//    }
//  }
//  return actions;
//}
//
//ActionVect SnesSettings::getAllActions() {
//  ActionVect actions;
//  for (unsigned a = 0; a < AtariAllActionsVector.size(); a++) {
//    if (isLegal(AtariAllActionsVector.at(a))) {
//      actions.push_back(AtariAllActionsVector.at(a));
//    }
//  }
//  return actions;
//}
//
//SnesActionsVector::SnesActionsVector(){
////	SnesAllActionsVector
//	SnesAllActionsVector.push_back(JOYPAD_NOOP);
//	for(int up = 0; up == JOYPAD_UP; up += JOYPAD_UP){
//		SnesAllActionsVector.push_back(up);
//	}
//}
