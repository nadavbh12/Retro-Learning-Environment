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

using namespace ale;

ActionVect AtariSettings::getMinimalActionSet() {
  ActionVect actions;
  for (unsigned a = 0; a < AtariAllActionsVector.size(); a++) {
    if (isMinimal(AtariAllActionsVector.at(a)) && isLegal(AtariAllActionsVector.at(a))) {
      actions.push_back(AtariAllActionsVector.at(a));
    }
  }
  return actions;
}

ActionVect AtariSettings::getAllActions() {
  ActionVect actions;
  for (unsigned a = 0; a < AtariAllActionsVector.size(); a++) {
    if (isLegal(AtariAllActionsVector.at(a))) {
      actions.push_back(AtariAllActionsVector.at(a));
    }
  }
  return actions;
}
