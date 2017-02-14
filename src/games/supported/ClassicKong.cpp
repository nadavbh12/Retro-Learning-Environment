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
#include "ClassicKong.hpp"

using namespace rle;


ClassicKongSettings::ClassicKongSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_UP,
						JOYPAD_DOWN,
						JOYPAD_A,	// Jump
						JOYPAD_A | JOYPAD_LEFT,
						JOYPAD_A | JOYPAD_RIGHT,
    };
}


/* create a new instance of the rom */
RomSettings* ClassicKongSettings::clone() const {
    RomSettings* rval = new ClassicKongSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void ClassicKongSettings::step(const RleSystem& system) {
	// update the reward
	reward_t score = 10*((readRam(&system, 0x245b) << 8) + readRam(&system, 0x245a));
	reward_t bonus_score = (readRam(&system, 0x2460) << 8) + readRam(&system, 0x245f);

	reward_t bonus_delta = m_bonus_score - bonus_score;
	reward_t score_delta = m_score - score;
	m_reward = score_delta + bonus_delta;
	int terminal_success = readRam(&system, 0x2461);
	m_terminal = terminal_success;

	m_bonus_score = bonus_score;
	m_score = score;
	m_lives = readRam(&system, 0x2458);
	if(m_lives == 0 || m_bonus_score == 0){
		m_terminal = true;
	}
}


/* reset the state of the game */
void ClassicKongSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_lives	   = 3;
    m_terminal = false;
    m_bonus_score = 5000;
}



/* saves the state of the rom settings */
void ClassicKongSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_lives);
  ser.putBool(m_terminal);
  ser.putInt(m_bonus_score);
}

// loads the state of the rom settings
void ClassicKongSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_lives = des.getInt();
  m_terminal = des.getBool();
  m_bonus_score = des.getInt();
}


ActionVect ClassicKongSettings::getStartingActions(){
	ActionVect startingActions;
	int num_of_nops(100);

	// wait for intro to end
	INSERT_NOPS(3.5*num_of_nops)
	// main Screen
	INSERT_ACTION_SINGLE_A(JOYPAD_START)
	// wait for cutscene to end
	INSERT_NOPS(11*num_of_nops)

	return startingActions;
}

