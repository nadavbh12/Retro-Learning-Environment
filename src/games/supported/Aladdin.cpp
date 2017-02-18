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
#include "Aladdin.hpp"

using namespace rle;


AladdinSettings::AladdinSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_UP, // only relavent when hanging
    					JOYPAD_DOWN, //duck
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_B,	// Jump
						JOYPAD_B | JOYPAD_RIGHT, // jump right
						JOYPAD_B | JOYPAD_LEFT,  // jump left
						JOYPAD_Y | JOYPAD_RIGHT, // Run right
						JOYPAD_Y | JOYPAD_LEFT,  // Run left
						JOYPAD_A, //throw apple
						JOYPAD_A | JOYPAD_B,	// Jump
						JOYPAD_A | JOYPAD_B | JOYPAD_RIGHT, // jump right
						JOYPAD_A | JOYPAD_B | JOYPAD_LEFT,  // jump left, 	// jump & Shoot
    };
}


/* create a new instance of the rom */
RomSettings* AladdinSettings::clone() const {
    RomSettings* rval = new AladdinSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void AladdinSettings::step(const RleSystem& system) {
	// update the reward
	reward_t playerScore = 256*readRam(&system, 0xF411) + readRam(&system, 0xF410); //based solely on moving right
	reward_t healthBonus = m_health - readRam(&system, 0x366); // adding health bonus to discourage getting hit
//	int apples = readRam(&system, 0x368);
//	int diamonds = readRam(&system, 0x36a);
	playerScore += healthBonus;
 	int current_lives = readRam(&system, 0x363)-1;
	m_reward = playerScore - m_score;
    m_score = playerScore;
	if( current_lives != m_lives) { //adding "previous lives" since lives=0 until game starts
		m_prev_lives = m_lives;
	}
    m_lives = current_lives;
	m_health = readRam(&system, 0x366);

//	cout <<"Score is: " << playerScore << " Health is: " << m_health << " lives are: " << m_lives << " m_prev_lives " << m_prev_lives<<endl;

    if ((m_prev_lives == 1) && (m_lives == 0) && (m_health == 0)){ //lives doesn't reach 0, condition is last life and no health
    	m_terminal = true;
    }

}


/* reset the state of the game */
void AladdinSettings::reset() {

    m_reward     = 0;
    m_score      = 0;
    m_health     = 3;
    m_lives	     = 3;
    m_prev_lives = 3;

    m_terminal = false;
}



/* saves the state of the rom settings */
void AladdinSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_lives);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void AladdinSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_lives = des.getInt();
  m_terminal = des.getBool();
}


ActionVect AladdinSettings::getStartingActions(){
	int i, num_of_nops(100);
	ActionVect startingActions;

	// wait for intro to end
	for(i = 0; i<5*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// main Screen
	startingActions.push_back(JOYPAD_START);
	// wait for character select screen
	for(i = 0; i<3.5*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	//start game
	startingActions.push_back(JOYPAD_START);

	for(i = 0; i<3.5*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	//start game
	startingActions.push_back(JOYPAD_START);
	return startingActions;
}

