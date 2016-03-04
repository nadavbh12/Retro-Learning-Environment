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
#include "FinalFight.hpp"

#include "AleSystem.hxx"

using namespace ale;


FinalFightSettings::FinalFightSettings() {
    reset();

    minimalActions = {	JOYPAD_NOOP,
    					JOYPAD_UP,
    					JOYPAD_DOWN,
						JOYPAD_LEFT,
						JOYPAD_RIGHT,
						JOYPAD_UP | JOYPAD_RIGHT,
						JOYPAD_UP | JOYPAD_LEFT,
						JOYPAD_DOWN | JOYPAD_RIGHT,
						JOYPAD_DOWN | JOYPAD_LEFT,
						JOYPAD_B,					// jump
						JOYPAD_Y,					// attack
						JOYPAD_Y | JOYPAD_B,		// special attack
						JOYPAD_RIGHT | JOYPAD_B,	// jump right
						JOYPAD_LEFT | JOYPAD_B,		// jump left
    };
}


/* create a new instance of the rom */
RomSettings* FinalFightSettings::clone() const {

    RomSettings* rval = new FinalFightSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void FinalFightSettings::step(const AleSystem& system) {
//    uint8_t* address = system.getRetroAgent().getRamAddress(RETRO_MEMORY_SYSTEM_RAM);
    int time = getDecimalScoreWord(0xcbc, 0xcbd, &system);


	// update the reward
    reward_t playerScore = getDecimalScoreWords({0xc97, 0xc96, 0xc95, 0xc94, 0xc93, 0xc92, 0xc91, 0xc90}, &system);
//    DEBUG2("Player Score: " << std::dec << playerScore);

    reward_t score = playerScore;
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
    if (m_wins==2){
    	m_score = m_score + 1000*time; //shai: manually adding time bonus to win faster
    	m_terminal = true;
    }
    if(o_wins == 2){
    	m_terminal = true;
    }
}

/* is end of game */
bool FinalFightSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t FinalFightSettings::getReward() const {

    return m_reward;
}


/* is an action part of the minimal set? */
bool FinalFightSettings::isMinimal(const Action &a) const {

	if(minimalActions.find(a) ==  minimalActions.end())
		return false;
	else
		return true;
}


/* reset the state of the game */
void FinalFightSettings::reset() {

    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;

}



/* saves the state of the rom settings */
void FinalFightSettings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void FinalFightSettings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect FinalFightSettings::getStartingActions(){
	int i, num_of_xs(4),num_of_nops(100);
	ActionVect startingActions;
	startingActions.reserve(num_of_xs*num_of_nops);
	// wait for intro to end
	for(i = 0; i<16*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// select tournament
	startingActions.push_back(JOYPAD_X);
	// wait for character select screen
	for(i = 0; i<3*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// choose default character (kano)
	startingActions.push_back(JOYPAD_X);
	startingActions.push_back(JOYPAD_X);
	startingActions.push_back(JOYPAD_X);
	// wait for game to begin
	for(i = 0; i<4*num_of_nops; i++){
		startingActions.push_back(JOYPAD_NOOP);
	}
	// skip tournament overview
	startingActions.push_back(JOYPAD_X);
	// wait for game to start
	for(i = 0; i<3.5*num_of_nops; i++){
			startingActions.push_back(JOYPAD_NOOP);
		}

	return startingActions;
}

