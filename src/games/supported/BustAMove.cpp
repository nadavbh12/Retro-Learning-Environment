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
#include "RleSystem.hxx"
#include "BustAMove.hpp"

using namespace rle;


BustAMoveSettings::BustAMoveSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_A,	//shoot
    };
}


/* create a new instance of the rom */
RomSettings* BustAMoveSettings::clone() const {
    RomSettings* rval = new BustAMoveSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void BustAMoveSettings::step(const RleSystem& system) {
	// update the reward
	reward_t playerScore = 	getDecimalScore(0x954,0x955,0x956, &system);

    m_reward = playerScore - m_score;
    m_score = playerScore;
//    cout << "Score: " << playerScore<<endl;
//    update terminal status
    int isAlive = readRam(&system, 0x906);
    if (isAlive > 0){
    	m_terminal = true;
    }
}


/* reset the state of the game */
void BustAMoveSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}



/* saves the state of the rom settings */
void BustAMoveSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void BustAMoveSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect BustAMoveSettings::getStartingActions(){
	int i, num_of_nops(100);
	ActionVect startingActions;

	// wait for intro to end
	for(i = 0; i<2*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// main Screen
	startingActions.push_back(JOYPAD_START);
	// wait for character select screen
	for(i = 0; i<2*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	//start game
	startingActions.push_back(JOYPAD_START);

	for(i = 0; i<2*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	//start game
	startingActions.push_back(JOYPAD_START);
	for(i = 0; i<2*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	//start game
	startingActions.push_back(JOYPAD_START);

	return startingActions;
}


