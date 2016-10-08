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
#include "MarioKart64.hpp"

#include "AleSystem.hxx"

using namespace ale;


MarioKart64Settings::MarioKart64Settings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_A | JOYPAD_LEFT,	// a to accelerate
						JOYPAD_A | JOYPAD_RIGHT,
						JOYPAD_B | JOYPAD_LEFT,	// b to break
						JOYPAD_B | JOYPAD_RIGHT,
						JOYPAD_R | JOYPAD_A
						JOYPAD_R | JOYPAD_A | JOYPAD_LEFT,	// a to accelerate
						JOYPAD_R | JOYPAD_A | JOYPAD_RIGHT,
						JOYPAD_R | JOYPAD_B | JOYPAD_LEFT,	// b to break
						JOYPAD_R | JOYPAD_B | JOYPAD_RIGHT,
						JOYPAD_Z | JOYPAD_A					// z to use powerup
						JOYPAD_Z | JOYPAD_A | JOYPAD_LEFT,
						JOYPAD_Z | JOYPAD_A | JOYPAD_RIGHT,
						JOYPAD_Z | JOYPAD_B | JOYPAD_LEFT,
						JOYPAD_Z | JOYPAD_B | JOYPAD_RIGHT,
						JOYPAD_R,	// jump
    };
}


/* create a new instance of the rom */
RomSettings* MarioKart64Settings::clone() const {

    RomSettings* rval = new MarioKart64Settings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void MarioKart64Settings::step(const AleSystem& system) {
//    uint8_t* address = system.getRetroAgent().getRamAddress(RETRO_MEMORY_SYSTEM_RAM);
}

/* is end of game */
bool MarioKart64Settings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t MarioKart64Settings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool MarioKart64Settings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
}


/* reset the state of the game */
void MarioKart64Settings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;

}



/* saves the state of the rom settings */
void MarioKart64Settings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void MarioKart64Settings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect MarioKart64Settings::getStartingActions(){
	int num_of_nops(100);
	ActionVect startingActions;
//	startingActions.reserve(num_of_xs*num_of_nops);

	// wait for intro to end
	INSERT_NOPS(16*num_of_nops)

	// select tournament
	INSERT_ACTION_SINGLE_A(JOYPAD_X)

	// wait for character select screen
	INSERT_NOPS(3.5*num_of_nops)

	// choose Raiden
	INSERT_ACTION_SINGLE(JOYPAD_DOWN, A)
	INSERT_NOPS(num_of_nops)

	// Select opponent: random seed for opponent is set by num of NOOPs
//	INSERT_NOPS(0) // Johnny Cage
//	INSERT_NOPS(5) // Sonya
//	INSERT_NOPS(10) // sub_zero
//	INSERT_NOPS(40) // scorpion


	// select character
	INSERT_ACTION_SINGLE(JOYPAD_X, A)
	INSERT_NOPS(num_of_nops)

	// wait for game to begin
	INSERT_NOPS(4*num_of_nops)

	// skip tournament overview
	INSERT_ACTION_SINGLE_A(JOYPAD_X)

	// wait for game to start
	INSERT_NOPS(3.5*num_of_nops)

	return startingActions;
}

