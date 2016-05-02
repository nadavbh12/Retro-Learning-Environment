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
#include "AleSystem.hxx"
#include "TetrisAndDrMarioNumBlocks.hpp"

using namespace ale;


TetrisAndDrMarioNumBlocksSettings::TetrisAndDrMarioNumBlocksSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_DOWN,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_A,	// counter clockwise rotation
						JOYPAD_B,	// clockwise rotation
    };
}


/* create a new instance of the rom */
RomSettings* TetrisAndDrMarioNumBlocksSettings::clone() const {
    RomSettings* rval = new TetrisAndDrMarioNumBlocksSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void TetrisAndDrMarioNumBlocksSettings::step(const AleSystem& system) {
//    uint8_t* address = system.getRetroAgent().getRamAddress(RETRO_MEMORY_SYSTEM_RAM);

	// update the reward
//	reward_t playerScore = (256 * 256) * readRam(&system, 0x272) + 256 * readRam(&system, 0x271) + readRam(&system, 0x270);

	reward_t numBlocks = readRam(&system, 0x357) + readRam(&system, 0x359) + readRam(&system, 0x35b) + readRam(&system, 0x35D) + readRam(&system, 0x35F) + readRam(&system, 0x361) + readRam(&system, 0x363);
    m_reward = numBlocks - m_score;
    m_score = numBlocks;
    //shai: temp fix

    DEBUG2("score is:" << m_score);
//    DEBUG2("reward is:" << m_reward);

//    update terminal status
    int isAlive = readRam(&system, 0x125);
    if (isAlive > 0){
    	m_terminal = true;
    }
//    DEBUG2("Is terminal: " << std::dec << m_terminal);
}

/* is end of game */
bool TetrisAndDrMarioNumBlocksSettings::isTerminal() const {
    return m_terminal;
};


/* get the most recently observed reward */
reward_t TetrisAndDrMarioNumBlocksSettings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool TetrisAndDrMarioNumBlocksSettings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
}


/* reset the state of the game */
void TetrisAndDrMarioNumBlocksSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}



/* saves the state of the rom settings */
void TetrisAndDrMarioNumBlocksSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void TetrisAndDrMarioNumBlocksSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect TetrisAndDrMarioNumBlocksSettings::getStartingActions(){
	int num_of_nops(100);
	ActionVect startingActions;
//	startingActions.reserve(num_of_xs*num_of_nops);

	// wait for intro to end
	startingActions.insert(startingActions.end(), 3.5*num_of_nops, JOYPAD_NOOP);
	// select game
	startingActions.push_back(JOYPAD_START);
	// wait for character select screen
	startingActions.insert(startingActions.end(), 0.8*num_of_nops, JOYPAD_NOOP);
	// select 1 player
	startingActions.push_back(JOYPAD_START);
	startingActions.insert(startingActions.end(), 0.6*num_of_nops, JOYPAD_NOOP);
	// select type (?)
	startingActions.push_back(JOYPAD_START);
	startingActions.insert(startingActions.end(), 0.6*num_of_nops, JOYPAD_NOOP);
	// select level
	startingActions.push_back(JOYPAD_START);
	startingActions.insert(startingActions.end(), 0.6*num_of_nops, JOYPAD_NOOP);
	// start the level
	startingActions.push_back(JOYPAD_START);
	startingActions.insert(startingActions.end(), 1*num_of_nops, JOYPAD_NOOP);
	return startingActions;
}

