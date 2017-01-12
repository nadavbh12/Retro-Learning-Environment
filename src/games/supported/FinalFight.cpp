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
#include "FinalFight.hpp"

#include "RleSystem.hxx"

using namespace rle;


FinalFightSettings::FinalFightSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_UP,
    					JOYPAD_DOWN,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_UP | JOYPAD_RIGHT,
						JOYPAD_UP | JOYPAD_LEFT,
						JOYPAD_B | JOYPAD_RIGHT,
						JOYPAD_B | JOYPAD_LEFT,
						JOYPAD_B | JOYPAD_DOWN,		// knee attack when in air
						JOYPAD_Y | JOYPAD_RIGHT,	// toss enemy
						JOYPAD_Y | JOYPAD_LEFT,		// as above
						JOYPAD_DOWN | JOYPAD_RIGHT,
						JOYPAD_DOWN | JOYPAD_LEFT,
						JOYPAD_Y,					// punch
						JOYPAD_B,					// jump
						JOYPAD_Y | JOYPAD_B,		// special attack
    };
}


/* create a new instance of the rom */
RomSettings* FinalFightSettings::clone() const {

    RomSettings* rval = new FinalFightSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void FinalFightSettings::step(const RleSystem& system) {

	// int time = getDecimalScoreWords({0xcbc, 0xcbd}, &system);

	// update the reward
    reward_t playerScore = 100 * getDecimalScoreWords({0xc95,0xc94,0xc93}, &system);

    reward_t score = playerScore;
    m_reward = score - m_score;
    m_score = score;
//    update terminal status
    int lives = readRam(&system, 0x2456);
    if ((lives == 0) && (m_prev_lives == 1)){
    	m_terminal = true;
    } else{
    	m_terminal = false;
    }
    m_prev_lives = lives;
}

/* reset the state of the game */
void FinalFightSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_prev_lives = 5;
    m_terminal = false;

}



/* saves the state of the rom settings */
void FinalFightSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_prev_lives);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void FinalFightSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_prev_lives = des.getInt();
  m_terminal = des.getBool();
}


ActionVect FinalFightSettings::getStartingActions(){
	int num_of_nops(100);
	ActionVect startingActions;
	// wait for intro to end
	startingActions.insert(startingActions.end(), 2.5*num_of_nops, JOYPAD_NOOP);
	// press start to skip intro
	startingActions.push_back(JOYPAD_START);
	startingActions.insert(startingActions.end(), 2*num_of_nops, JOYPAD_NOOP);
	// press start to select character
	startingActions.push_back(JOYPAD_START);
	startingActions.push_back(JOYPAD_START);
	startingActions.insert(startingActions.end(), 2.2*num_of_nops, JOYPAD_NOOP);
	// select character
	startingActions.push_back(JOYPAD_START);
	startingActions.insert(startingActions.end(), 4*num_of_nops, JOYPAD_NOOP);

	return startingActions;
}

