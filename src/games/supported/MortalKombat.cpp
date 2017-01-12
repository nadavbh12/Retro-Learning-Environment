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
#include "MortalKombat.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

using namespace rle;


MortalKombatSettings::MortalKombatSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_UP,	// jump
    					JOYPAD_DOWN,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_R,	// block
						JOYPAD_A,	// low Kick
						JOYPAD_B,	// low/high punch
						JOYPAD_Y,	// low kick
						JOYPAD_X,	// high kick
						JOYPAD_UP | JOYPAD_RIGHT,	// jump right
						JOYPAD_UP | JOYPAD_LEFT,	// jump left
						JOYPAD_DOWN | JOYPAD_Y,		// uppercut
						JOYPAD_LEFT | JOYPAD_A,		// sweep kick
						JOYPAD_RIGHT | JOYPAD_A,	// sweep kick
						JOYPAD_LEFT | JOYPAD_X,		// roundhouse
						JOYPAD_RIGHT | JOYPAD_X,	// sweep kick
						JOYPAD_RIGHT | JOYPAD_Y,	// throw
						JOYPAD_LEFT | JOYPAD_Y,		// throw

						JOYPAD_R | JOYPAD_LEFT,		// added to support kano's combos
						JOYPAD_R | JOYPAD_RIGHT,
						JOYPAD_R | JOYPAD_UP,
						JOYPAD_R | JOYPAD_DOWN,
    };
}


/* create a new instance of the rom */
RomSettings* MortalKombatSettings::clone() const {

    RomSettings* rval = new MortalKombatSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void MortalKombatSettings::step(const RleSystem& system) {
    int time = getDecimalScore(0x122, &system);

    int npcScore = getDecimalScore(0x2a, 0x2b, 0x2c, &system);
    npcScore *= 100;

	// update the reward
    reward_t playerScore = getDecimalScore(0x26, 0x27, 0x28, &system);
    playerScore *= 100;

    reward_t score = playerScore - npcScore;

    m_reward = score - m_score;
    m_score = score;

//    update terminal status
    int playerLife = readRam(&system, 0x4c1);
    int npcLife = readRam(&system, 0x4c3);

    if(time == 0x1){ //shai:comparing to 1 not zero to avoid terminal upon first run
    	if(playerLife == npcLife ){ // game behavior: is time is up
    		m_terminal=true;
    	}
    }

    int totalWins = m_wins + o_wins;
    m_wins = getDecimalScore(0x196e, &system);
    o_wins = getDecimalScore(0x1aca, &system);

    if(system.settings().getBool("MK_random_position")){
		if((m_wins + o_wins) != totalWins){
			match_ended = true;
		}

		bool newMatchStarted = (time == 99) && (match_ended == true);
		if(newMatchStarted){
			match_ended = false;
			// set random positions for players
			startingOperations(const_cast<RleSystem&>(system));
		}
    }

    if (m_wins==2){
    	m_terminal = true;
    }
    if(o_wins == 2){
    	m_terminal = true;
    }
}

/* reset the state of the game */
void MortalKombatSettings::reset() {
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_wins    = 0;
    o_wins    = 0;
    match_ended = false;
}



/* saves the state of the rom settings */
void MortalKombatSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_wins);
  ser.putInt(o_wins);
  ser.putBool(m_terminal);
  ser.putBool(match_ended);
}

// loads the state of the rom settings
void MortalKombatSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_wins = des.getInt();
  o_wins = des.getInt();
  m_terminal = des.getBool();
  match_ended = des.getBool();
}


ActionVect MortalKombatSettings::getStartingActions(const RleSystem& system){
	int num_of_nops(100);
	ActionVect startingActions;

	// wait for intro to end
	INSERT_NOPS(16*num_of_nops)

	// select options
	INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
	INSERT_ACTION_SINGLE_A(JOYPAD_X)
	INSERT_NOPS(3*num_of_nops)

	// select difficulty
	INSERT_ACTION_SINGLE_A(JOYPAD_DOWN)
	INSERT_NOPS(num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_DOWN)
	INSERT_NOPS(num_of_nops)
	string difficulty = system.settings().getString("MK_difficulty");
	if("medium" == difficulty){
	}else if("hard" == difficulty){
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
	}else if("very_hard" == difficulty){
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
		INSERT_NOPS(0.5*num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
	}else if("very_easy" == difficulty){
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
		INSERT_NOPS(0.5*num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
		INSERT_NOPS(0.5*num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
	}else if("easy" == difficulty){
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
		INSERT_NOPS(0.5*num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
		INSERT_NOPS(0.5*num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
		INSERT_NOPS(0.5*num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_X)
	}

	// exit options screen
	INSERT_ACTION_SINGLE_A(JOYPAD_START)
	INSERT_NOPS(3*num_of_nops)

	// select tournament
	INSERT_ACTION_SINGLE_A(JOYPAD_X)

	// wait for character select screen
	INSERT_NOPS(3.5*num_of_nops)

	// choose character from list
	string player1_character = system.settings().getString("MK_player1_character");
	if("rayden" == player1_character){
		INSERT_ACTION_SINGLE(JOYPAD_DOWN, A)
	}else if("sonya" == player1_character){
		INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
		INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
		INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
	}else if("sub-zero" == player1_character){
		INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
	}else if("liu-kang" == player1_character){
		INSERT_ACTION_SINGLE(JOYPAD_DOWN, A)
		INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
		INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
	}else if("cage" == player1_character){
		INSERT_ACTION_SINGLE(JOYPAD_LEFT, A)
	}else if("kano" == player1_character){
	}else if("scorpion" == player1_character){
		INSERT_ACTION_SINGLE(JOYPAD_RIGHT, A)
		INSERT_ACTION_SINGLE(JOYPAD_NOOP, A)
		INSERT_ACTION_SINGLE(JOYPAD_DOWN, A)
	}else{
		throw RleException("MK_player1_character illegal");
	}
	INSERT_NOPS(num_of_nops)

	// Select opponent: random seed for opponent is set by num of NOOPs
	int opponent_character_nops = system.settings().getInt("MK_opponent_character");
	INSERT_NOPS(opponent_character_nops);

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

void MortalKombatSettings::startingOperations(RleSystem& system){
	if(system.settings().getBool("MK_random_position")){
		Random& rng = system.rng();
		writeRam(&system, 0x30d, (rng.next()%0x100));	// set player 1's random init position
		writeRam(&system, 0x30f, (rng.next()%0x100));	// set player 2's random init position
	}
}
