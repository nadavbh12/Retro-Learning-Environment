#include "../RomUtils.hpp"
#include <iomanip>

#include "../RomUtils.hpp"
#include "SuperMarioKart.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

using namespace rle;

SuperMarioKartSettings::SuperMarioKartSettings() {
    reset();

    // TODO
    minimalActions = {  JOYPAD_NOOP,
                        JOYPAD_LEFT,
                        JOYPAD_RIGHT,
                        JOYPAD_R,
                        JOYPAD_A,
                        JOYPAD_B,
                        JOYPAD_Y,
                        JOYPAD_X,
                        JOYPAD_L,
                        JOYPAD_LEFT | JOYPAD_A,
                        JOYPAD_RIGHT | JOYPAD_A,
                        JOYPAD_LEFT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_Y,
                        JOYPAD_LEFT | JOYPAD_Y,
                        JOYPAD_RIGHT | JOYPAD_R,
                        JOYPAD_LEFT | JOYPAD_R,
                        JOYPAD_RIGHT | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_B | JOYPAD_L,
                        JOYPAD_RIGHT | JOYPAD_B | JOYPAD_L
    };
}

/* create a new instance of the rom */
RomSettings* SuperMarioKartSettings::clone() const {
    RomSettings* rval = new SuperMarioKartSettings();
    *rval = *this;
    return rval;
}

/* process the latest information from RLE */
void SuperMarioKartSettings::step(const RleSystem& system) {

  // update the player posiiton
  reward_t playerPosition = getDecimalScore(0x1040, &system);

  //Selecting reward score strategy as the diff with the previous position
  reward_t playerScore = playerPosition;

  //Reward is positive if the player wins one position 
  m_reward = m_reward - playerPosition;
  
  m_score = playerScore;

  int current_lives = getDecimalScore(0x0154, &system);

  if (current_lives < m_lives)
    {
      //Reached a terminal state
      m_terminal = true;
    }
}

/* reset the state of the game */
void SuperMarioKartSettings::reset() {
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;

}

/* saves the state of the rom settings */
void SuperMarioKartSettings::saveState( Serializer & ser ) {
    ser.putInt(m_reward);
    ser.putBool(m_terminal);
    ser.putInt(m_lives);
}

// loads the state of the rom settings
void SuperMarioKartSettings::loadState( Deserializer & des ) {
    m_reward = des.getInt();
    m_terminal = des.getBool();
    m_lives = des.getInt();
}

ActionVect SuperMarioKartSettings::selectChar(int character_index){
    ActionVect selectCharActions;
    
    selectCharActions.push_back(JOYPAD_NOOP);
    
    for(int i = 0; i < character_index; i++) {
        selectCharActions.push_back(JOYPAD_RIGHT);
        selectCharActions.push_back(JOYPAD_NOOP);
    }

    selectCharActions.push_back(JOYPAD_B);

    return selectCharActions;
}

int SuperMarioKartSettings::getCharacterIndex(const RleSystem& system){
    int character_index = 0; // Ryu by default
    string player1_character = system.settings()->getString("SUPER_MARIO_KART_player1_character");
    if("mario" == player1_character){
        character_index = 0;
    }else if("peach" == player1_character){
        character_index = 1;
    }else if("bowser" == player1_character){
        character_index = 2;
    }else if("koopa" == player1_character){
        character_index = 3;
    }else if("luigi" == player1_character){
        character_index = 4;
    }else if("yoshi" == player1_character){
        character_index = 5;
    }else if("donkeykongjr" == player1_character){
        character_index = 6;
    }else if("toad" == player1_character){
        character_index = 7;
    }else{
        throw RleException("SUPER_MARIO_KART_player1_character illegal");
    }
    return character_index;
}


ActionVect SuperMarioKartSettings::getStartingActions(const RleSystem& system){
    int i, num_of_nops(100);
    ActionVect startingActions;
    // wait for intro to end
    for(i = 0; i<7 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // second animation
    startingActions.push_back(JOYPAD_B);

    for(i = 0; i<5 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }
    
    // Selecting 1 player mode
    startingActions.push_back(JOYPAD_B);

    for(i = 0; i<7 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }
    
    // Selecting race mode
    startingActions.push_back(JOYPAD_B);

    for(i = 0; i<7 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }
    
    // Selecting velocity class FIXME: add selection capability here
    startingActions.push_back(JOYPAD_B);

    for(i = 0; i<7 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }
    
    // Validating options
    startingActions.push_back(JOYPAD_B);

    for(i = 0; i<7 * num_of_nops; i++){
      startingActions.push_back(JOYPAD_NOOP);
    }

    //Selecting driver
    // Select character by index
    ActionVect selectCharActions = selectChar(getCharacterIndex(system));
    startingActions.insert(startingActions.end(), selectCharActions.begin(), selectCharActions.end());

    // startingActions.push_back(JOYPAD_B);

    for(i = 0; i<7 * num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    //Validate
    startingActions.push_back(JOYPAD_B);

    for(i = 0; i<7*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // Selecting circuit
    startingActions.push_back(JOYPAD_B);

    for(i = 0; i<5*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    
    startingActions.push_back(JOYPAD_B);
    
    return startingActions;
}

