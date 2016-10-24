  /* *****************************************************************************
 * The line 78 is based on Xitari's code, from Google Inc.
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
 *
 * *****************************************************************************
 *
 * RomSettings.hpp
 *
 * The interface to describe games as RL environments. It provides terminal and
 *  reward information.
 * *****************************************************************************
 */
#ifndef __ROMSETTINGS_HPP__
#define __ROMSETTINGS_HPP__

#include "../common/Constants.h"
#include "Serializer.hxx"
#include "Deserializer.hxx"

namespace ale {

class AleSystem;


// rom support interface
struct RomSettings {
    virtual ~RomSettings() {}

    // reset
    virtual void reset() = 0;

    // is end of game
    virtual bool isTerminal() const = 0;

    // get the most recently observed reward
    virtual ale::reward_t getReward() const = 0;

    // get the most recently observed reward for player 2
    virtual ale::reward_t getRewardPlayer2() const{return 0;};

    // the rom-name
    virtual const char *rom() const = 0;

    // create a new instance of the rom
    virtual ale::RomSettings *clone() const = 0;

    // is an action part of the minimal set?
    virtual bool isMinimal(const ale::Action &a) const = 0;

    // process the latest information from ALE
    virtual void step(const ale::AleSystem &system) = 0;

    // saves the state of the rom settings
    virtual void saveState( Serializer & ser ) = 0;

    // loads the state of the rom settings
    virtual void loadState( Deserializer & ser ) = 0;

    // is an action legal (default: yes)
    virtual bool isLegal(const ale::Action &a) const;

    // Remaining lives.
    virtual const int lives() { return isTerminal() ? 0 : 1; }

    // Returns a restricted (minimal) set of actions. If not overriden, this is all actions.
    ActionVect getMinimalActionSet();

    // Returns the set of all legal actions
    // Can change according to emulation system
    ActionVect getAllActions();

    // Returns a list of actions that are required to start the game.
    // By default this is an empty list.
    virtual ActionVect getStartingActions();

    // Returns a list of actions that are required to start the game.
    // Some games require the AleSystem for game-specific settings
    virtual ActionVect getStartingActions(const AleSystem& system){ return getStartingActions();}

    // Optionally perform starting operations such as modifying the game's RAM
    virtual void startingOperations(AleSystem& system){};

    // below functions used to belong to RomUtils
    // reads a byte at a memory location
    virtual int readRam(const AleSystem* system, int offset);

    // write a byte at a memory location
    virtual void writeRam(const AleSystem* system, int offset, uint8_t data);

    // extracts a decimal value from 1, 2, and 3 bytes respectively
    int getDecimalScore(int idx, const AleSystem* system);
    int getDecimalScore(int lo, int hi, const AleSystem* system);
    int getDecimalScore(int lo, int mid, int hi, const AleSystem* system);
    int getDecimalScoreWord(int idx, const AleSystem* system);
	int getDecimalScoreWord(int lo, int hi, const AleSystem* system);
	int getDecimalScoreWord(int lo, int mid, int hi, const AleSystem* system);
	int getDecimalScoreWords(std::vector<int> indexes, const AleSystem* system);


protected:
    ActionVect AllActionsVector;
};

} // namespace ale

#endif // __ROMSETTINGS_HPP__
