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
#include "SonicTheHedgehog.hpp"

using namespace rle;


SonicTheHedgehogSettings::SonicTheHedgehogSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_DOWN,
    					JOYPAD_UP,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_GENESIS_A,	// Jump
						JOYPAD_GENESIS_A | JOYPAD_DOWN,
						JOYPAD_GENESIS_A | JOYPAD_UP,
						JOYPAD_GENESIS_A | JOYPAD_LEFT,
						JOYPAD_GENESIS_A | JOYPAD_RIGHT
    };
}


/* create a new instance of the rom */
RomSettings* SonicTheHedgehogSettings::clone() const {
    RomSettings* rval = new SonicTheHedgehogSettings();
    *rval = *this;
    return rval;
}


void SonicTheHedgehogSettings::step(const RleSystem& system) {
//	uint8_t* address = system.getRetroAgent().getRamAddress();

    reward_t score = (readRam(&system, 0xFE26) << 16) | (readRam(&system, 0xFE29) << 8) | (readRam(&system, 0xFE28));// + readRam(&system, 0xFE29);
    score *= 10;
	int rings = readRam(&system, 0xFE22) + (readRam(&system, 0xFE21) << 8);

    // update the reward
	m_reward = score - m_score;
    if(system.settings().getBool("sonic1_ring_reward")){
    	m_reward += rings - m_rings;
    }

    m_score = score;
    m_rings = rings;
	m_lives = readRam(&system, 0xFE13);

	int terminal_status = readRam(&system, 0xFE1F);
	// update terminal status
    if (m_lives == 0 || !terminal_status){
    	m_terminal = true;
    }
}

/* reset the state of the game */
void SonicTheHedgehogSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_rings    = 0;
    m_lives	   = 3;
    m_terminal = false;
    m_time	  =  400;
}



/* saves the state of the rom settings */
void SonicTheHedgehogSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_rings);
  ser.putInt(m_lives);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void SonicTheHedgehogSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_rings = des.getInt();
  m_lives = des.getInt();
  m_terminal = des.getBool();
}


ActionVect SonicTheHedgehogSettings::getStartingActions(){
	int num_of_nops(100);
	ActionVect startingActions;

//	 wait for intro to end
	INSERT_NOPS(6*num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_START)
	// wait for level to begin
	INSERT_NOPS(3.3*num_of_nops)

	return startingActions;
}
