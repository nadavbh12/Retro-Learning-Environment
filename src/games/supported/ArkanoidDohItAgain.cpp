#include "../RomUtils.hpp"
#include <iomanip>

#include "../RomUtils.hpp"
#include "ArkanoidDohItAgain.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

using namespace rle;

ArkanoidDohItAgainSettings::ArkanoidDohItAgainSettings() {
    reset();

    // TODO
    minimalActions = {  JOYPAD_NOOP,
                        JOYPAD_LEFT,
                        JOYPAD_RIGHT,
                        JOYPAD_A,
                        JOYPAD_B,
                        JOYPAD_Y,
                        JOYPAD_X,
                        JOYPAD_LEFT | JOYPAD_A,
                        JOYPAD_RIGHT | JOYPAD_A,
                        JOYPAD_LEFT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_Y,
                        JOYPAD_LEFT | JOYPAD_Y,
                        JOYPAD_RIGHT | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_B,
    };
}

/* create a new instance of the rom */
RomSettings* ArkanoidDohItAgainSettings::clone() const {
    RomSettings* rval = new ArkanoidDohItAgainSettings();
    *rval = *this;
    return rval;
}

/* process the latest information from RLE */
void ArkanoidDohItAgainSettings::step(const RleSystem& system) {

  // update the player posiiton
  reward_t playerPosition = getDecimalScore(0x1a32, 0x1a33, &system);

  //Selecting reward score strategy as the diff with the previous position
  reward_t playerScore = playerPosition;

  //Reward is positive if the player wins one position
  m_reward = playerPosition - m_prevPosition;
  m_prevPosition = playerPosition;
  
  m_score = playerScore;

  int current_lives = getDecimalScore(0x0168, &system);

  if (current_lives == 0)
    {
      //Reached a terminal state
      m_terminal = true;
    }
}

/* reset the state of the game */
void ArkanoidDohItAgainSettings::reset() {
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
}

/* saves the state of the rom settings */
void ArkanoidDohItAgainSettings::saveState( Serializer & ser ) {
    ser.putInt(m_reward);
    ser.putBool(m_terminal);
    ser.putInt(m_lives);
}

// loads the state of the rom settings
void ArkanoidDohItAgainSettings::loadState( Deserializer & des ) {
    m_reward = des.getInt();
    m_terminal = des.getBool();
    m_lives = des.getInt();
}

ActionVect ArkanoidDohItAgainSettings::getStartingActions(const RleSystem& system){
    int i, num_of_nops(100);
    ActionVect startingActions;
    // wait for intro to end
    for(i = 0; i<15 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // second animation
    startingActions.push_back(JOYPAD_START);

    for(i = 0; i< 1 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // second animation
    startingActions.push_back(JOYPAD_START);
    
    for(i = 0; i<1 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }
    
    // Selecting 1 player mode
    startingActions.push_back(JOYPAD_START);

    for(i = 0; i<5 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // Jump Intro
    startingActions.push_back(JOYPAD_START);

    for(i = 0; i<1 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }
    
    return startingActions;
}

