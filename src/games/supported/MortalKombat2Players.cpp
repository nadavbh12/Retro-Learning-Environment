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
#include <iomanip>
#include "../RomUtils.hpp"
#include "MortalKombat2Players.hpp"
#include "RleSystem.hxx"
#include "RleException.h"

using namespace rle;


MortalKombat2PlayersSettings::MortalKombat2PlayersSettings() : MortalKombatSettings(), m_rewardB(0) {}


/* create a new instance of the rom */
RomSettings* MortalKombat2PlayersSettings::clone() const {

    RomSettings* rval = new MortalKombat2PlayersSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void MortalKombat2PlayersSettings::step(const RleSystem& system) {
	MortalKombatSettings::step(system);
	m_rewardB = - m_reward;
}

/* reset the state of the game */
void MortalKombat2PlayersSettings::reset() {
	MortalKombatSettings::reset();
    m_rewardB   = 0;

}



/* saves the state of the rom settings */
void MortalKombat2PlayersSettings::saveState( Serializer & ser ) {
  MortalKombatSettings::saveState(ser);
  ser.putInt(m_rewardB);
}

// loads the state of the rom settings
void MortalKombat2PlayersSettings::loadState( Deserializer & des ) {
  MortalKombatSettings::loadState(des);
  m_rewardB = des.getInt();
}

ActionVect MortalKombat2PlayersSettings::getStartingActions(const RleSystem& system){
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
	string difficulty = system.settings()->getString("MK_difficulty");
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

	// add second player
	INSERT_ACTION_SINGLE(JOYPAD_START, B)

	// choose character from list
	string player1_character = system.settings()->getString("MK_player1_character");
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
	// choose for player b - Sonya
	// choose character from list
	string player2_character = system.settings()->getString("MK_player2_character");
	if("rayden" == player2_character){
		INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
		INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
		INSERT_ACTION_SINGLE(JOYPAD_DOWN, B)
	}else if("sonya" == player2_character){
		INSERT_ACTION_SINGLE(JOYPAD_RIGHT, B)
	}else if("sub-zero" == player2_character){
	}else if("liu-kang" == player2_character){
		INSERT_ACTION_SINGLE(JOYPAD_DOWN, B)
		INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
		INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
	}else if("cage" == player2_character){
		INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
		INSERT_ACTION_SINGLE(JOYPAD_NOOP, B)
		INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
	}else if("kano" == player2_character){
		INSERT_ACTION_SINGLE(JOYPAD_LEFT, B)
	}else if("scorpion" == player2_character){
		INSERT_ACTION_SINGLE(JOYPAD_DOWN, B)
	}else{
		throw RleException("MK_player2_character illegal");
	}
	INSERT_NOPS(num_of_nops)

	// select character
	INSERT_ACTION_SINGLE(JOYPAD_X, A)
	// select character
	INSERT_ACTION_SINGLE(JOYPAD_X, B)
	INSERT_NOPS(num_of_nops)

	// wait for game to begin
	INSERT_NOPS(4*num_of_nops)
	// skip tournament overview
	INSERT_ACTION_SINGLE_A(JOYPAD_X)
	// wait for game to start
	INSERT_NOPS(3.5*num_of_nops)

	return startingActions;
}

