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
#include "StreetFighterII.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

using namespace rle;


StreetFighterIISettings::StreetFighterIISettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_UP,	// jump
    					JOYPAD_DOWN,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_R,	// upper-cut
						JOYPAD_A,	// right Kick
						JOYPAD_B,	// leftkick
						JOYPAD_Y,	// left punch
						JOYPAD_X,	// right punch
						JOYPAD_L,   //spin-kick
						JOYPAD_UP | JOYPAD_RIGHT,	// jump right
						JOYPAD_UP | JOYPAD_LEFT,	// jump left
						JOYPAD_DOWN | JOYPAD_Y,
						JOYPAD_DOWN | JOYPAD_R,
						JOYPAD_DOWN | JOYPAD_L,
						JOYPAD_DOWN | JOYPAD_A,
						JOYPAD_DOWN | JOYPAD_B,
						JOYPAD_DOWN | JOYPAD_X,

						JOYPAD_LEFT | JOYPAD_A,
						JOYPAD_RIGHT | JOYPAD_A,
						JOYPAD_LEFT | JOYPAD_X,
						JOYPAD_RIGHT | JOYPAD_X,
						JOYPAD_RIGHT | JOYPAD_Y,
						JOYPAD_LEFT | JOYPAD_Y,
						JOYPAD_RIGHT | JOYPAD_L,
						JOYPAD_LEFT | JOYPAD_L,
						JOYPAD_RIGHT | JOYPAD_R,
						JOYPAD_LEFT | JOYPAD_R,
						JOYPAD_RIGHT | JOYPAD_B,
						JOYPAD_LEFT | JOYPAD_B,

    };
}


/* create a new instance of the rom */
RomSettings* StreetFighterIISettings::clone() const {

    RomSettings* rval = new StreetFighterIISettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void StreetFighterIISettings::step(const RleSystem& system) {
    int time = getDecimalScore(0x1ac8, &system);

	// update the reward
    reward_t playerScore = getDecimalScore(0xdc3,0xdc4, &system);
    playerScore *= 100;

    reward_t score = playerScore;

    m_reward = score - m_score;
    m_score = score;

//    update terminal status
//    int playerLife = readRam(&system, 0x4c1);
//    int npcLife = readRam(&system, 0x4c3);
    if(time == 0x1){ //shai:comparing to 1 not zero to avoid terminal upon first run
    	m_terminal=true;

    }

//    int totalWins = m_wins + o_wins;
    m_wins = getDecimalScore(0xcd0, &system);
    o_wins = getDecimalScore(0xed0, &system);
//    cout << "player score: " << playerScore  << " Time: "
//    		<< time << " p_wins: " << m_wins << " op wins: " <<o_wins<<endl;

    if (m_wins==2){
    	m_terminal = true;
    }
    if(o_wins == 2){
    	m_terminal = true;
    }
}

/* reset the state of the game */
void StreetFighterIISettings::reset() {
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_wins    = 0;
    o_wins    = 0;
    match_ended = false;
}



/* saves the state of the rom settings */
void StreetFighterIISettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_wins);
  ser.putInt(o_wins);
  ser.putBool(m_terminal);
  ser.putBool(match_ended);
}

// loads the state of the rom settings
void StreetFighterIISettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_wins = des.getInt();
  o_wins = des.getInt();
  m_terminal = des.getBool();
  match_ended = des.getBool();
}


ActionVect StreetFighterIISettings::getStartingActions(const RleSystem& system){
	int i, num_of_nops(100);
	ActionVect startingActions;

	// wait for intro to end
	for(i = 0; i<5*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// main Screen
	startingActions.push_back(JOYPAD_START);
//	 wait for character select screen
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
	for(i = 0; i<3*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	//start game
	startingActions.push_back(JOYPAD_START);

	return startingActions;
}



