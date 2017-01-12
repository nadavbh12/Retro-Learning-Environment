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
#include "SuperMarioWorld.hpp"

#include "RleSystem.hxx"

using namespace rle;


SuperMarioWorldSettings::SuperMarioWorldSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_UP,	// look up
    					JOYPAD_DOWN,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_R,	// move slightly right
						JOYPAD_L,	// move slightly left
						JOYPAD_A,	// spin jump
						JOYPAD_B,	// standard jump
						JOYPAD_Y,	// run/pick-up shell/open door...
						JOYPAD_B | JOYPAD_RIGHT,	// jump right
						JOYPAD_B | JOYPAD_LEFT,		// jump left
						JOYPAD_A | JOYPAD_RIGHT,	// jump right
						JOYPAD_A | JOYPAD_LEFT,		// jump left
						JOYPAD_SELECT,	// drop stock item

						// samve as above while running
						JOYPAD_Y | JOYPAD_LEFT,
						JOYPAD_Y | JOYPAD_RIGHT,
						JOYPAD_Y | JOYPAD_B | JOYPAD_RIGHT,
						JOYPAD_Y | JOYPAD_B | JOYPAD_LEFT,
						JOYPAD_Y | JOYPAD_A | JOYPAD_RIGHT,
						JOYPAD_Y | JOYPAD_A | JOYPAD_LEFT,
    };
    m_lastTime = {-1, -1, -1};
}


/* create a new instance of the rom */
RomSettings* SuperMarioWorldSettings::clone() const {

    RomSettings* rval = new SuperMarioWorldSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void SuperMarioWorldSettings::step(const RleSystem& system) {
    int time = getDecimalScoreWords({0xf33, 0xf32, 0xf31}, &system);
//	DEBUG2("time: " << std::dec << time);

    m_lastTime.erase(m_lastTime.begin());
    m_lastTime.push_back(readRam(&system, 0xf30));

	// update the reward
    reward_t score = 10 * getDecimalScoreWords({0xf34, 0xf35, 0xf36}, &system);

    m_reward = score - m_score;
    m_score = score;

    if(time == 0x1){ //shai:comparing to 1 not zero to avoid terminal upon first run
		m_terminal=true;
    }
    else{ // check if the timer has stopped
    	if ( (0 < time && time < 300) && (std::adjacent_find( m_lastTime.begin(), m_lastTime.end(), std::not_equal_to<int>() ) == m_lastTime.end()) ){
    	    m_terminal=true;
    	}
    }
}

/* reset the state of the game */
void SuperMarioWorldSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;

}


/* saves the state of the rom settings */
void SuperMarioWorldSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void SuperMarioWorldSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect SuperMarioWorldSettings::getStartingActions(){
	int i, num_of_nops(100);
	ActionVect startingActions;
//	startingActions.reserve(num_of_xs*num_of_nops);
	// wait for intro to end
	for(i = 0; i<3*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	startingActions.push_back(JOYPAD_B);
	for(i = 0; i<0.3*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
//	// select first save slot
	startingActions.push_back(JOYPAD_B);
	for(i = 0; i<0.3*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// select 1 player
	startingActions.push_back(JOYPAD_B);
	for(i = 0; i<0.3*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// wait for text to end
	for(i = 0; i<10*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	startingActions.push_back(JOYPAD_B);
	// wait for text to end
	for(i = 0; i<2*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// select level
	startingActions.push_back(JOYPAD_LEFT);
	for(i = 0; i<1.5*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// start level
	startingActions.push_back(JOYPAD_B);
	// wait for level to load
	startingActions.push_back(JOYPAD_LEFT);
	for(i = 0; i<2*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}

	return startingActions;
}

