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

#include "AleSystem.hxx"

using namespace ale;


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

//						JOYPAD_R | JOYPAD_LEFT,		// added to support kano's combos
//						JOYPAD_R | JOYPAD_RIGHT,
//						JOYPAD_R | JOYPAD_UP,
//						JOYPAD_R | JOYPAD_DOWN,
    };
}


/* create a new instance of the rom */
RomSettings* MortalKombatSettings::clone() const {

    RomSettings* rval = new MortalKombatSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void MortalKombatSettings::step(const AleSystem& system) {
//    uint8_t* address = system.getRetroAgent().getRamAddress(RETRO_MEMORY_SYSTEM_RAM);
    int time = getDecimalScore(0x122, &system);

    int npcScore = getDecimalScore(0x2a, 0x2b, 0x2c, &system);
    npcScore *= 100;
//    DEBUG2("NPC Score: " << std::dec << npcScore);

	// update the reward
    reward_t playerScore = getDecimalScore(0x26, 0x27, 0x28, &system);
    playerScore *= 100;
//    DEBUG2("Player Score: " << std::dec << playerScore);

    reward_t score = playerScore - npcScore;
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
    int playerLife = readRam(&system, 0x4c1);
    int npcLife = readRam(&system, 0x4c3);

    if(time == 0x1){ //shai:comparing to 1 not zero to avoid terminal upon first run
    	if(playerLife == npcLife ){ // game behavior: is time is up
    		m_terminal=true;
    	}
    }
    m_wins = getDecimalScore(0x196e, &system);
    o_wins = getDecimalScore(0x1aca, &system);
    if (m_wins==2){
//    	m_score = m_score + 1000*time; //shai: manually adding time bonus to win faster
    	m_terminal = true;
    }
    if(o_wins == 2){
    	m_terminal = true;
    }
}

/* is end of game */
bool MortalKombatSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t MortalKombatSettings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool MortalKombatSettings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
}


/* reset the state of the game */
void MortalKombatSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_wins    = 0;
    o_wins    = 0;

}



/* saves the state of the rom settings */
void MortalKombatSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putInt(m_wins);
  ser.putInt(o_wins);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void MortalKombatSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_wins = des.getInt();
  o_wins = des.getInt();
  m_terminal = des.getBool();
}


ActionVect MortalKombatSettings::getStartingActions(){
	int num_of_nops(100);
	ActionVect startingActions;
//	startingActions.reserve(num_of_xs*num_of_nops);

	// wait for intro to end
	startingActions.insert(startingActions.end(), 16*num_of_nops, JOYPAD_NOOP);
	// select tournament
	startingActions.push_back(JOYPAD_X);
	// wait for character select screen
	startingActions.insert(startingActions.end(), 3.5*num_of_nops, JOYPAD_NOOP);
	// choose Raiden
	startingActions.push_back(JOYPAD_DOWN);
	startingActions.push_back(JOYPAD_DOWN);
	startingActions.insert(startingActions.end(), num_of_nops, JOYPAD_NOOP);
	// select character
	startingActions.push_back(JOYPAD_X);
	startingActions.push_back(JOYPAD_X);
	startingActions.insert(startingActions.end(), num_of_nops, JOYPAD_NOOP);
	// wait for game to begin
	startingActions.insert(startingActions.end(), 4*num_of_nops, JOYPAD_NOOP);
	// skip tournament overview
	startingActions.push_back(JOYPAD_X);
	// wait for game to start
	startingActions.insert(startingActions.end(), 3.5*num_of_nops, JOYPAD_NOOP);

	return startingActions;
}

