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
#include "AtariCollection.hpp"

using namespace rle;


AtariCollectionSettings::AtariCollectionSettings() {
    reset();
    minimalActions = {
    	PLAYER_A | JOYPAD_NOOP,
    	PLAYER_A | JOYPAD_FIRE,
    	PLAYER_A | JOYPAD_UP,
    	PLAYER_A | JOYPAD_RIGHT,
    	PLAYER_A | JOYPAD_LEFT,
    	PLAYER_A | JOYPAD_DOWN,
    	PLAYER_A | JOYPAD_UP | JOYPAD_RIGHT,
    	PLAYER_A | JOYPAD_UP | JOYPAD_LEFT,
    	PLAYER_A | JOYPAD_DOWN | JOYPAD_RIGHT,
    	PLAYER_A | JOYPAD_DOWN | JOYPAD_LEFT,
    	PLAYER_A | JOYPAD_UP | JOYPAD_FIRE,
    	PLAYER_A | JOYPAD_RIGHT | JOYPAD_FIRE,
    	PLAYER_A | JOYPAD_LEFT | JOYPAD_FIRE,
    	PLAYER_A | JOYPAD_DOWN | JOYPAD_FIRE,
    	PLAYER_A | JOYPAD_UP | JOYPAD_RIGHT | JOYPAD_FIRE,
    	PLAYER_A | JOYPAD_UP | JOYPAD_LEFT | JOYPAD_FIRE,
    	PLAYER_A | JOYPAD_DOWN | JOYPAD_RIGHT | JOYPAD_FIRE,
    	PLAYER_A | JOYPAD_DOWN | JOYPAD_LEFT | JOYPAD_FIRE,
    };
}


/* create a new instance of the rom */
RomSettings* AtariCollectionSettings::clone() const {

    RomSettings* rval = new AtariCollectionSettings();
    *rval = *this;
    return rval;
}

void AtariCollectionSettings::step(const RleSystem& system) {
    // update the reward
    reward_t score = getDecimalScore(0x52, 0x53, &system);
    score *= 10;
//    DEBUG2("Score: " << std::dec << score);
    m_reward = score - m_score;
    // Deal with score wrapping. In truth this should be done for all games and in a more
    // uniform fashion.
    if (m_reward < 0) {
        const int WRAP_SCORE = 100000;
        m_reward += WRAP_SCORE;
    }
    m_score = score;

    // update terminal status
    if((readRam(&system, 0x33)  & 0xff) == 0xff){
		m_lives = readRam(&system, 0x57);
		m_terminal = (m_lives == 0);
    }
}


/* reset the state of the game */
void AtariCollectionSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_lives    = 4;
}



/* saves the state of the rom settings */
void AtariCollectionSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_lives);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void AtariCollectionSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_lives = des.getInt();
  m_terminal = des.getBool();
}


ActionVect AtariCollectionSettings::getStartingActions(){
	int j, num_of_xs(4),num_of_nops(80);
	ActionVect startingActions;
	for(j = 0; j<num_of_xs; j++){
		startingActions.push_back(JOYPAD_X);
		startingActions.insert(startingActions.end(), num_of_nops, JOYPAD_NOOP);
	}
	startingActions.insert(startingActions.end(), 2*num_of_nops, JOYPAD_NOOP);

	return startingActions;
}

