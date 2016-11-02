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
#include "NBAGiveNGo.hpp"

using namespace rle;


NBAGiveNGoSettings::NBAGiveNGoSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_UP,
    					JOYPAD_DOWN,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_A,	// Select player to pass to
						JOYPAD_B,	// pass/block
						JOYPAD_X,	// pass to dunk
						JOYPAD_Y,	// Throw to basket
    };
}


/* create a new instance of the rom */
RomSettings* NBAGiveNGoSettings::clone() const {
    RomSettings* rval = new NBAGiveNGoSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void NBAGiveNGoSettings::step(const RleSystem& system) {
//    uint8_t* address = system.getRetroAgent().getRamAddress(RETRO_MEMORY_SYSTEM_RAM);

	uint32_t time = 60 * readRam(&system, 0xAC) + readRam(&system, 0xAE);

	// update the reward
	uint32_t playerScore = readRam(&system, 0xA6);
	uint32_t enemyScore = readRam(&system, 0xA8);
	uint32_t score = playerScore - enemyScore;

	m_reward = score - m_score;
    m_score = score;

    if (1 == time){
    	m_terminal = true;
    }

}

/* is end of game */
bool NBAGiveNGoSettings::isTerminal() const {
    return m_terminal;
};


/* get the most recently observed reward */
reward_t NBAGiveNGoSettings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool NBAGiveNGoSettings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
}


/* reset the state of the game */
void NBAGiveNGoSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_time	  =  400;
}



/* saves the state of the rom settings */
void NBAGiveNGoSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void NBAGiveNGoSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect NBAGiveNGoSettings::getStartingActions(){
	int num_of_nops(100);
	ActionVect startingActions;
//	startingActions.reserve(num_of_xs*num_of_nops);

	// wait for intro to end
	startingActions.insert(startingActions.end(), 9*num_of_nops, JOYPAD_NOOP);
	// press start to begin
	startingActions.push_back(JOYPAD_START);
	// select arcade
	startingActions.insert(startingActions.end(), 2*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	// select 1 vs cpu
	startingActions.insert(startingActions.end(), 1.5*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	// select team
	startingActions.insert(startingActions.end(), 1.5*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	// select team
	startingActions.insert(startingActions.end(), 1.5*num_of_nops, JOYPAD_NOOP);
	startingActions.push_back(JOYPAD_START);
	startingActions.insert(startingActions.end(), 15.1*num_of_nops, JOYPAD_NOOP);

	return startingActions;
}

