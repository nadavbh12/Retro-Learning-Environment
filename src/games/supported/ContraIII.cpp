#include "../RomUtils.hpp"
#include <iomanip>

#include "../RomUtils.hpp"
#include "ContraIII.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

using namespace rle;

ContraIIISettings::ContraIIISettings() {
    reset();

    // TODO
    minimalActions = {  JOYPAD_NOOP,
                        JOYPAD_LEFT,
                        JOYPAD_RIGHT,
                        JOYPAD_UP,
                        JOYPAD_DOWN,
                        JOYPAD_LEFT | JOYPAD_DOWN,
                        JOYPAD_LEFT | JOYPAD_UP,
                        JOYPAD_RIGHT | JOYPAD_DOWN,
                        JOYPAD_RIGHT | JOYPAD_UP,
                        JOYPAD_A,
                        JOYPAD_B,
                        JOYPAD_Y,
                        JOYPAD_X,
                        JOYPAD_LEFT | JOYPAD_B,
                        JOYPAD_RIGHT | JOYPAD_B,
                        JOYPAD_UP |JOYPAD_B,
                        JOYPAD_DOWN | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_DOWN | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_UP | JOYPAD_B,
                        JOYPAD_RIGHT | JOYPAD_DOWN | JOYPAD_B,
                        JOYPAD_RIGHT | JOYPAD_UP | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_Y,
                        JOYPAD_RIGHT | JOYPAD_Y,
                        JOYPAD_UP |JOYPAD_Y,
                        JOYPAD_DOWN | JOYPAD_Y,
                        JOYPAD_LEFT | JOYPAD_DOWN | JOYPAD_Y,
                        JOYPAD_LEFT | JOYPAD_UP | JOYPAD_Y,
                        JOYPAD_RIGHT | JOYPAD_DOWN | JOYPAD_Y,
                        JOYPAD_RIGHT | JOYPAD_UP | JOYPAD_Y,
                        JOYPAD_LEFT | JOYPAD_A,
                        JOYPAD_RIGHT | JOYPAD_A,
                        JOYPAD_UP |JOYPAD_A,
                        JOYPAD_DOWN | JOYPAD_A,
                        JOYPAD_LEFT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_X,
                        JOYPAD_UP |JOYPAD_X,
                        JOYPAD_DOWN | JOYPAD_X,


    };
}

/* create a new instance of the rom */
RomSettings* ContraIIISettings::clone() const {
    RomSettings* rval = new ContraIIISettings();
    *rval = *this;
    return rval;
}

/* process the latest information from RLE */
void ContraIIISettings::step(const RleSystem& system) {

  // update the player posiiton FIXME: this is wrong!
  reward_t playerScore = getDecimalScore(0x1fa1, 0x1fa2, &system);
  //Reward is positive if the player wins one position
  m_reward = playerScore - m_prevScore;
  m_prevScore = playerScore;
  
  m_score = playerScore;

  int current_lives = getDecimalScore(0x1F8A, &system);

  if (current_lives == 0)
    {
      //Reached a terminal state
      m_terminal = true;
    }
}

/* reset the state of the game */
void ContraIIISettings::reset() {
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_prevScore = 0;
}

/* saves the state of the rom settings */
void ContraIIISettings::saveState( Serializer & ser ) {
    ser.putInt(m_reward);
    ser.putBool(m_terminal);
    ser.putInt(m_lives);
}

// loads the state of the rom settings
void ContraIIISettings::loadState( Deserializer & des ) {
    m_reward = des.getInt();
    m_terminal = des.getBool();
    m_lives = des.getInt();
}

ActionVect ContraIIISettings::getStartingActions(const RleSystem& system){
    int i, num_of_nops(100);
    ActionVect startingActions;
    // wait for intro to end
    for(i = 0; i<10 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // second animation
    startingActions.push_back(JOYPAD_START);

    for(i = 0; i<5 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }
    
    // Selecting 1 player mode
    startingActions.push_back(JOYPAD_START);

    for(i = 0; i<5 * num_of_nops; i++){
      startingActions.push_back(JOYPAD_NOOP);
    }
    
    return startingActions;
}

