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
#ifndef __SNES_SETTINGS_HPP__
#define __SNES_SETTINGS_HPP__

#include "RomSettings.hpp"
#include <unordered_set>

namespace rle {

// rom support interface for snes
struct SnesSettings : public RomSettings{
    SnesSettings();

	virtual ~SnesSettings() {}

    // reset
    virtual void reset() = 0;

    // is end of game
    bool isTerminal() const{ return m_terminal;}

    // get the most recently observed reward
    reward_t getReward() const{ return m_reward;}

    // the rom-name
    virtual const char *rom() const = 0;

    // create a new instance of the rom
    virtual RomSettings *clone() const = 0;

    // is an action part of the minimal set?
    bool isMinimal(const Action &a) const;

    // process the latest information from ALE
    virtual void step(const RleSystem &system) = 0;
protected:
    std::unordered_set<Action> minimalActions;
    bool m_terminal;
    reward_t m_reward;
    reward_t m_score;
};


} // namespace rle


#endif // __SNES_SETTINGS_HPP__
