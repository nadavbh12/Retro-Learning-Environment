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
#include "SuperMarioAllStarsNoRight.hpp"

using namespace rle;


SuperMarioAllStarsNoRightSettings::SuperMarioAllStarsNoRightSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_DOWN, //duck
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_A,	// Jump
						JOYPAD_X,	// Accelerate or Shoot
						JOYPAD_A | JOYPAD_RIGHT, // jump right
						JOYPAD_A | JOYPAD_LEFT,  // jump left
						JOYPAD_X | JOYPAD_RIGHT, // Run right
						JOYPAD_X | JOYPAD_LEFT,  // Run left
						JOYPAD_X | JOYPAD_A, 	// jump & Shoot
    };
}


/* create a new instance of the rom */
RomSettings* SuperMarioAllStarsNoRightSettings::clone() const {
    RomSettings* rval = new SuperMarioAllStarsNoRightSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void SuperMarioAllStarsNoRightSettings::step(const RleSystem& system) {
//    uint8_t* address = system.getRetroAgent().getRamAddress(RETRO_MEMORY_SYSTEM_RAM);

	// update the reward
//    reward_t playerScore = getDecimalScore(0x270, 0x271, 0x272, &system);
	reward_t playerScore = 10*readRam(&system, 0x7D3) + 100*readRam(&system, 0x7D2) + 1000 * readRam(&system, 0x7D1) + 10000*readRam(&system, 0x7D0);
//	reward_t playerLocationBounus = 256*readRam(&system, 0x43)+readRam(&system, 0x42) ; //shai: adding bounus for location to make mario go right, adjust in order to prefer winning over score

	m_reward = playerScore - m_score;
    m_score = playerScore ;
//	DEBUG2("Score: " << std::dec << m_score);
    m_time=100*readRam(&system, 0x7E9) + 10 * readRam(&system, 0x7EA) + readRam(&system, 0x7EB);
//    DEBUG2("time is: " << std::dec << m_time);
	m_lives = readRam(&system, 0x172A);
//    update terminal status
//    int isAlive = readRam(&system, 0x125);
    if ((m_lives == 1) || (m_time == 1)){ // shai : set to 1, doesn't get to 0
    	m_terminal = true;
    }

}


/* reset the state of the game */
void SuperMarioAllStarsNoRightSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_lives	   = 6; // shai: since stopping at one, begin in 6
    m_terminal = false;
    m_time	  =  400;
}



/* saves the state of the rom settings */
void SuperMarioAllStarsNoRightSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_lives);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void SuperMarioAllStarsNoRightSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_lives = des.getInt();
  m_terminal = des.getBool();
}


ActionVect SuperMarioAllStarsNoRightSettings::getStartingActions(){
	int i, num_of_nops(100);
	ActionVect startingActions;

	// wait for intro to end
	for(i = 0; i<2*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// main Screen
	startingActions.push_back(JOYPAD_START);
	// wait for character select screen
	for(i = 0; i<3.5*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// select Mario 1985
	startingActions.push_back(JOYPAD_START);
	for(i = 0; i<0.8*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// select File (?)
	startingActions.push_back(JOYPAD_START);
	for(i = 0; i<0.5*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// select Player
	startingActions.push_back(JOYPAD_START);
	for(i = 0; i<0.7*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// Begin Level
	startingActions.push_back(JOYPAD_START);
	for(i = 0; i<0.4*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	return startingActions;
}

