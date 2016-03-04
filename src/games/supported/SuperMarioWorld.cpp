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

#include "AleSystem.hxx"

using namespace ale;


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
}


/* create a new instance of the rom */
RomSettings* SuperMarioWorldSettings::clone() const {

    RomSettings* rval = new SuperMarioWorldSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void SuperMarioWorldSettings::step(const AleSystem& system) {
    uint8_t* address = system.getRetroAgent().getRamAddress(RETRO_MEMORY_SYSTEM_RAM);
    int time = 100 * readRam(&system, 0xf25) + 10 * readRam(&system, 0xf26) + readRam(&system, 0xf27);
//	DEBUG2("time: " << std::dec << time);

	// update the reward
    reward_t score = 10 * readRam(&system, 0xf34) + 100 * readRam(&system, 0xf34) + 100 * readRam(&system, 0xf345);
//    DEBUG2("Score: " << std::dec << score);

    m_reward = score - m_score;
//    // Deal with score wrapping. In truth this should be done for all games and in a more
//    // uniform fashion.
//    if (m_reward < 0) {
//        const int WRAP_SCORE = 100000;
//        m_reward += WRAP_SCORE;
//    }
    m_score = score;

//    update terminal status
//    int playerLife = readRam(&system, 0x4c1);
//    int npcLife = readRam(&system, 0x4c3);

    if(time == 0x1){ //shai:comparing to 1 not zero to avoid terminal upon first run
		m_terminal=true;
    }
}

/* is end of game */
bool SuperMarioWorldSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t SuperMarioWorldSettings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool SuperMarioWorldSettings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
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
	int i, num_of_xs(4),num_of_nops(100);
	ActionVect startingActions;
//	startingActions.reserve(num_of_xs*num_of_nops);
	// wait for intro to end
	for(i = 0; i<3*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	startingActions.push_back(JOYPAD_B);
	for(i = 0; i<1*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// select first save slot
	startingActions.push_back(JOYPAD_B);
	for(i = 0; i<1*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// select 1 player
	startingActions.push_back(JOYPAD_B);
	for(i = 0; i<1*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// wait for intro to end
	for(i = 0; i<18*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// selec level
	startingActions.push_back(JOYPAD_LEFT);
	for(i = 0; i<1*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// start level
	startingActions.push_back(JOYPAD_B);

	return startingActions;
}

