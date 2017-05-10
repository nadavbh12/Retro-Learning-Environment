#include "../RomUtils.hpp"
#include <iomanip>

#include "../RomUtils.hpp"
#include "SuperDoubleDragon.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

using namespace rle;

SuperDoubleDragonSettings::SuperDoubleDragonSettings() {
    reset();

    // TODO
    minimalActions = {  JOYPAD_NOOP,
                        JOYPAD_LEFT,
                        JOYPAD_RIGHT,
                        JOYPAD_UP,
                        JOYPAD_DOWN,
                        JOYPAD_A,
                        JOYPAD_B,
                        JOYPAD_Y,
                        JOYPAD_X,
                        JOYPAD_LEFT | JOYPAD_A,
                        JOYPAD_RIGHT | JOYPAD_A,
                        JOYPAD_UP | JOYPAD_A,
                        JOYPAD_DOWN | JOYPAD_A,
                        JOYPAD_LEFT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_X,
                        JOYPAD_UP | JOYPAD_X,
                        JOYPAD_DOWN | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_Y,
                        JOYPAD_LEFT | JOYPAD_Y,
                        JOYPAD_UP | JOYPAD_Y,
                        JOYPAD_DOWN | JOYPAD_Y,
                        JOYPAD_RIGHT | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_B,
                        JOYPAD_UP | JOYPAD_B,
                        JOYPAD_DOWN | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_X | JOYPAD_B,
                        JOYPAD_RIGHT | JOYPAD_X | JOYPAD_B,
                        JOYPAD_UP | JOYPAD_X | JOYPAD_B,
                        JOYPAD_DOWN | JOYPAD_X | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_X | JOYPAD_A,
                        JOYPAD_RIGHT | JOYPAD_X | JOYPAD_A,
                        JOYPAD_UP | JOYPAD_X | JOYPAD_A,
                        JOYPAD_DOWN | JOYPAD_X | JOYPAD_A,


    };
}

/* create a new instance of the rom */
RomSettings* SuperDoubleDragonSettings::clone() const {
    RomSettings* rval = new SuperDoubleDragonSettings();
    *rval = *this;
    return rval;
}

/* process the latest information from RLE */
void SuperDoubleDragonSettings::step(const RleSystem& system) {

  // update the player life
  reward_t playerLife = getDecimalScore(0x0FD6, &system);

  // update the player enemy kills
  reward_t enemyKills = getDecimalScore(0x00DF, 0x00E0, &system);

  //Reward is positive if the player wins one position
  string rewardStrategy = system.settings()->getString("DDRAGON_reward_strategy");

  if (rewardStrategy == "score") {
    m_reward = enemyKills - m_prevEnemyKills;
  }
  else {
    m_reward = playerLife - m_prevLife;
  }

  m_prevLife = playerLife;
  m_prevEnemyKills = enemyKills;
  
  int current_lives = getDecimalScore(0x00dc, &system);

  if (current_lives == 0)
    {
      //Reached a terminal state
      m_terminal = true;
    }
}

/* reset the state of the game */
void SuperDoubleDragonSettings::reset() {
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_prevLife = 0;
    m_prevEnemyKills = 0;
}

/* saves the state of the rom settings */
void SuperDoubleDragonSettings::saveState( Serializer & ser ) {
    ser.putInt(m_reward);
    ser.putBool(m_terminal);
    ser.putInt(m_lives);
}

// loads the state of the rom settings
void SuperDoubleDragonSettings::loadState( Deserializer & des ) {
    m_reward = des.getInt();
    m_terminal = des.getBool();
    m_lives = des.getInt();
}

ActionVect SuperDoubleDragonSettings::getStartingActions(const RleSystem& system){
    int i, num_of_nops(100);
    ActionVect startingActions;
    // wait for intro to end
    for(i = 0; i<20 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // second animation
    startingActions.push_back(JOYPAD_START);

    for(i = 0; i< 5 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // second animation
    startingActions.push_back(JOYPAD_START);
    
    for(i = 0; i<1 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }
    
    return startingActions;
}

