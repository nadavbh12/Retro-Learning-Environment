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
 */
#include <iomanip>
#include "../RomUtils.hpp"
#include "MortalKombat2Players.hpp"
#include "AleSystem.hxx"

using namespace ale;


MortalKombat2PlayersSettings::MortalKombat2PlayersSettings() : m_rewardB(0), MortalKombatSettings() {}


/* create a new instance of the rom */
RomSettings* MortalKombat2PlayersSettings::clone() const {

    RomSettings* rval = new MortalKombat2PlayersSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void MortalKombat2PlayersSettings::step(const AleSystem& system) {
	MortalKombatSettings::step(system);
	m_rewardB = - m_reward;
}


/* get the most recently observed reward */
reward_t MortalKombat2PlayersSettings::getReward() const {

    return m_reward;
}


/* reset the state of the game */
void MortalKombat2PlayersSettings::reset() {
	MortalKombatSettings::reset();
    m_rewardB   = 0;

}



/* saves the state of the rom settings */
void MortalKombat2PlayersSettings::saveState( Serializer & ser ) {
  MortalKombatSettings::saveState(ser);
  ser.putInt(m_rewardB);
}

// loads the state of the rom settings
void MortalKombat2PlayersSettings::loadState( Deserializer & des ) {
  MortalKombatSettings::loadState(des);
  m_rewardB = des.getInt();
}

ActionVect MortalKombat2PlayersSettings::getStartingActions(){
	int num_of_nops(100);
	ActionVect startingActions;

	// wait for intro to end
	INSERT_NOPS(16*num_of_nops)

	// select tournament
	INSERT_ACTION_SINGLE_A(JOYPAD_X)
	INSERT_NOPS(3.5*num_of_nops)

	// add second player
	INSERT_ACTION_SINGLE(JOYPAD_START, B)

	// choose for player a - Raiden
	INSERT_ACTION_SINGLE(JOYPAD_DOWN, A)
	// choose for player b - Sonya
	INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
	INSERT_NOPS(num_of_nops)

	// select character
	INSERT_ACTION_SINGLE(JOYPAD_X, A)
	// select character
	INSERT_ACTION_SINGLE(JOYPAD_X, B)
	INSERT_NOPS(num_of_nops)

	// wait for game to begin
	INSERT_NOPS(4*num_of_nops)
	// skip tournament overview
	INSERT_ACTION_SINGLE_A(JOYPAD_X)
	// wait for game to start
	INSERT_NOPS(3.5*num_of_nops)

	return startingActions;
}

