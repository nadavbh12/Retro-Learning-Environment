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
#include "SuperMarioBros.hpp"

#include "AleSystem.hxx"

using namespace ale;


SuperMarioBrosSettings::SuperMarioBrosSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_UP,	// look up
    					JOYPAD_DOWN,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_A,	// jump
						JOYPAD_B,	// run/shoot fireball
						JOYPAD_A | JOYPAD_RIGHT,	// jump right
						JOYPAD_A | JOYPAD_LEFT,		// jump left

						// same as above while running
						JOYPAD_Y | JOYPAD_LEFT,
						JOYPAD_Y | JOYPAD_RIGHT,
						JOYPAD_Y | JOYPAD_A | JOYPAD_RIGHT,
						JOYPAD_Y | JOYPAD_A | JOYPAD_LEFT,
    };
    m_lastTime = {-1, -1, -1};
}


/* create a new instance of the rom */
RomSettings* SuperMarioBrosSettings::clone() const {

    RomSettings* rval = new SuperMarioBrosSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void SuperMarioBrosSettings::step(const AleSystem& system) {
	DEBUG2("ram size: " << system.getRetroAgent().getRamSize(0));
	DEBUG2("ram size: " << system.getRetroAgent().getRamSize(1));
	DEBUG2("ram size: " << system.getRetroAgent().getRamSize(2));
	DEBUG2("ram size: " << system.getRetroAgent().getRamSize(3));
//    uint8_t* address = system.getRetroAgent().getRamAddress(RETRO_MEMORY_VIDEO_RAM);
//    int time = getDecimalScoreWords({0x7fa, 0x7f9, 0x7f8}, &system);
//	DEBUG2("time: " << std::dec << time);
//
//    m_lastTime.erase(m_lastTime.begin());
//    m_lastTime.push_back(readRam(&system, 0xf30));
//
//	// update the reward
//    reward_t score = 10 * getDecimalScoreWords({0xf34, 0xf35, 0xf36}, &system);
////    DEBUG2("Score: " << std::dec << score);
//
//    m_reward = score - m_score;
//    m_score = score;
//
////    update terminal status
////    int playerLife = readRam(&system, 0x4c1);
//
//    if(time == 0x1){ //shai:comparing to 1 not zero to avoid terminal upon first run
//		m_terminal=true;
//    }
//    else{ // check if the timer has stopped
//    	if ( (0 < time && time < 300) && (std::adjacent_find( m_lastTime.begin(), m_lastTime.end(), std::not_equal_to<int>() ) == m_lastTime.end()) ){
//    	    m_terminal=true;
//    	}
//    }
}

/* is end of game */
bool SuperMarioBrosSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t SuperMarioBrosSettings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool SuperMarioBrosSettings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
}


/* reset the state of the game */
void SuperMarioBrosSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;

}



/* saves the state of the rom settings */
void SuperMarioBrosSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void SuperMarioBrosSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect SuperMarioBrosSettings::getStartingActions(){
	int i, num_of_nops(100);
	ActionVect startingActions;
//	startingActions.reserve(num_of_xs*num_of_nops);
	// wait for intro to end
	for(i = 0; i<5; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// select 1 player
	startingActions.push_back(JOYPAD_START);
	for(i = 0; i<1.65*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	return startingActions;
}

