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
#include "../RomUtils.hpp"
#include <iomanip>

#include "../RomUtils.hpp"
#include "Wolfenstein.hpp"

#include "RleSystem.hxx"

using namespace rle;


WolfensteinSettings::WolfensteinSettings() {
	reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_UP,
    					JOYPAD_DOWN,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_UP | JOYPAD_RIGHT,
						JOYPAD_UP | JOYPAD_LEFT,
						JOYPAD_DOWN | JOYPAD_RIGHT,
						JOYPAD_DOWN | JOYPAD_LEFT,

						JOYPAD_B,	// shoot
    					JOYPAD_UP | JOYPAD_B,
    					JOYPAD_DOWN | JOYPAD_B,
						JOYPAD_LEFT | JOYPAD_B,
						JOYPAD_RIGHT | JOYPAD_B,
						JOYPAD_UP | JOYPAD_RIGHT | JOYPAD_B,
						JOYPAD_UP | JOYPAD_LEFT | JOYPAD_B,
						JOYPAD_DOWN | JOYPAD_RIGHT | JOYPAD_B,
						JOYPAD_DOWN | JOYPAD_LEFT | JOYPAD_B,

						JOYPAD_A	// open door
//						JOYPAD_A | JOYPAD_B,
//    					JOYPAD_UP | JOYPAD_A | JOYPAD_B,
//    					JOYPAD_DOWN | JOYPAD_A | JOYPAD_B,
//						JOYPAD_LEFT | JOYPAD_A | JOYPAD_B,
//						JOYPAD_RIGHT | JOYPAD_A | JOYPAD_B,
//						JOYPAD_UP | JOYPAD_RIGHT | JOYPAD_A | JOYPAD_B,
//						JOYPAD_UP | JOYPAD_LEFT | JOYPAD_A | JOYPAD_B,
//						JOYPAD_DOWN | JOYPAD_RIGHT | JOYPAD_A | JOYPAD_B,
//						JOYPAD_DOWN | JOYPAD_LEFT | JOYPAD_A | JOYPAD_B,
    };
}


/* create a new instance of the rom */
RomSettings* WolfensteinSettings::clone() const {

    RomSettings* rval = new WolfensteinSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void WolfensteinSettings::step(const RleSystem& system) {

    //	// update the reward
    reward_t score = readRam(&system, 0x8949) + 256*readRam(&system, 0x894a) +256*256*readRam(&system, 0x894b);
//    DEBUG2("Score: " << std::dec << score)

    m_reward = score - m_score;
    m_score = score;
    //Mapped for future reward shaping
//  m_hp = readRam(&system, 0x8955);
//	m_ammo=readRam(&system, 0x8957);

//    update terminal status
    m_prev_lives = m_lives;
    m_lives = readRam(&system, 0x8953);
    if((m_lives == 0) && (m_prev_lives == 1)){
    	m_terminal = true;
    }

}


/* reset the state of the game */
void WolfensteinSettings::reset() {
	m_lives	   = 3;
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;

}



/* saves the state of the rom settings */
void WolfensteinSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void WolfensteinSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect WolfensteinSettings::getStartingActions(){
	int num_of_xs(4),num_of_nops(100);
	ActionVect startingActions;
	startingActions.reserve(num_of_xs*num_of_nops);
	// wait for intro to end
	startingActions.insert(startingActions.end(), 8*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_SELECT);
	// skip intro
	startingActions.insert(startingActions.end(), 0.8*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_SELECT);
	// select 1 player
	startingActions.insert(startingActions.end(), 1.2*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_SELECT);
	startingActions.insert(startingActions.end(), 0.5*num_of_nops, JOYPAD_NOOP);

	startingActions.push_back(JOYPAD_SELECT);
	startingActions.insert(startingActions.end(), 0.7*num_of_nops, JOYPAD_NOOP);
	return startingActions;
}

