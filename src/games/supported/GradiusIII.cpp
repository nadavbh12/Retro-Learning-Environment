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
#include "GradiusIII.hpp"

#include "RleSystem.hxx"

using namespace rle;


GradiusIIISettings::GradiusIIISettings() {
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

						JOYPAD_A,	// shoot
    					JOYPAD_UP | JOYPAD_A,
    					JOYPAD_DOWN | JOYPAD_A,
						JOYPAD_LEFT | JOYPAD_A,
						JOYPAD_RIGHT | JOYPAD_A,
						JOYPAD_UP | JOYPAD_RIGHT | JOYPAD_A,
						JOYPAD_UP | JOYPAD_LEFT | JOYPAD_A,
						JOYPAD_DOWN | JOYPAD_RIGHT | JOYPAD_A,
						JOYPAD_DOWN | JOYPAD_LEFT | JOYPAD_A,

						JOYPAD_B,	// use power
						JOYPAD_A | JOYPAD_B,
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
RomSettings* GradiusIIISettings::clone() const {

    RomSettings* rval = new GradiusIIISettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void GradiusIIISettings::step(const RleSystem& system) {

    //	// update the reward
    reward_t score = getDecimalScore(0x1f44, 0x1f45,0x1f46, &system);

    m_reward = score - m_score;
    m_score = score;

//    update terminal status
    m_prev_lives = m_lives;
    m_lives = readRam(&system, 0x7c);
    if((m_lives == 255) && (m_prev_lives == 0)){
    	m_terminal = true;
    }
}

/* reset the state of the game */
void GradiusIIISettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;

}

/* saves the state of the rom settings */
void GradiusIIISettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void GradiusIIISettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect GradiusIIISettings::getStartingActions(){
	int num_of_nops(100);
	ActionVect startingActions;

	// wait for intro to end
	startingActions.insert(startingActions.end(), 2.4*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	// skip intro
	startingActions.insert(startingActions.end(), 0.5*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	// select 1 player
	startingActions.insert(startingActions.end(), 1.5*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_A);
	startingActions.insert(startingActions.end(), num_of_nops, JOYPAD_NOOP);
	// accept
	startingActions.push_back(JOYPAD_A);
	startingActions.insert(startingActions.end(), 3*num_of_nops, JOYPAD_NOOP);
	return startingActions;
}

