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
                        JOYPAD_LEFT | JOYPAD_B | JOYPAD_A,
                        JOYPAD_RIGHT | JOYPAD_B | JOYPAD_A,
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
  m_reward = m_prevPosition - playerPosition;
  m_prevPosition = playerPosition;
  
  m_score = playerScore;

  //Possible End Race flags
  //0x012e/0x0144 -> 2 right after race ends/0 during race
  //0x17f6 -> 1 right after race ends/255 otherwise
  //0x17f7 -> 0 right after race ends/255 otherwise
  int endRaceFlag = getDecimalScore(0x012e, &system);

  // Player position is 8 if the player is 5th

  //The agent reaches a terminal state if it finished 5th or worse
  // and a driver has arrived to the goal
  if ((endRaceFlag > 0) && (playerPosition > 6))
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
    // Avoid resetting the current character as it is persisted across resets
}

/* saves the state of the rom settings */
void SuperMarioKartSettings::saveState( Serializer & ser ) {
    ser.putInt(m_reward);
    ser.putBool(m_terminal);
    ser.putInt(m_lives);
    ser.putInt(m_current_character);
}

// loads the state of the rom settings
void SuperMarioKartSettings::loadState( Deserializer & des ) {
    m_reward = des.getInt();
    m_terminal = des.getBool();
    m_lives = des.getInt();
    m_current_character = des.getInt();
}

ActionVect SuperMarioKartSettings::selectChar(int character_index){
    ActionVect selectCharActions;
    
    selectCharActions.push_back(JOYPAD_NOOP);

    int action = JOYPAD_NOOP;
    if (character_index > m_current_character) {
        action = JOYPAD_RIGHT;
    } else {
        action = JOYPAD_LEFT;
    }
    for(int i = 0; i < abs(character_index-m_current_character); i++) {
        selectCharActions.push_back(action);
        selectCharActions.push_back(JOYPAD_NOOP);
    }

    m_current_character = character_index;

    selectCharActions.push_back(JOYPAD_B);
    selectCharActions.push_back(JOYPAD_NOOP);

    return selectCharActions;
}

int SuperMarioKartSettings::getCharacterIndex(const RleSystem& system){
    int character_index = 0;
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

