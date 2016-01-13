///* *****************************************************************************
// * A.L.E (Arcade Learning Environment)
// * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
// *   the Reinforcement Learning and Artificial Intelligence Laboratory
// * Released under the GNU General Public License; see License.txt for details.
// *
// * Based on: Stella  --  "An Atari 2600 VCS Emulator"
// * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
// *
// * *****************************************************************************
// */
//#include "Boxing.hpp"
//
//#include "../RomUtils.hpp"
//#include <iomanip>
//
//
//AtariCollectionSettings::AtariCollectionSettings() {
//
//    reset();
//}
//
//
///* create a new instance of the rom */
//RomSettings* AtariCollectionSettings::clone() const {
//
//    RomSettings* rval = new BoxingSettings();
//    *rval = *this;
//    return rval;
//}
//
//
///* process the latest information from ALE */
//void AtariCollectionSettings::step(const AleSystem& system) {
////	FUNCTION_NAME
//    // update the reward
//    int my_score   = getDecimalScore(0x92, &system);
//    int oppt_score = getDecimalScore(0x93, &system);
////    DEBUG2("Score is: " <<  std::hex << std::setw(2) << my_score << ", " << oppt_score);
//
//    // handle KO
//    if (readRam(&system, 0x92) == 0xC0) my_score   = 100;
//    if (readRam(&system, 0x93) == 0xC0) oppt_score = 100;
//    reward_t score = my_score - oppt_score;
//    m_reward = score - m_score;
//    m_score = score;
//
//    // update terminal status
//    // if either is KO, the game is over
//    if (my_score == 100 || oppt_score == 100) {
//        m_terminal = true;
//    } else {  // otherwise check to see if out of time
//        int minutes = readRam(&system, 0x90) >> 4;
//        int seconds = (readRam(&system, 0x91) & 0xF) +
//                      (readRam(&system, 0x91) >> 4) * 10;
//        m_terminal = minutes == 0 && seconds == 0;
////        DEBUG2("Time is: minutes " <<  std::dec << std::setw(2) << minutes << ", seconds: " << seconds);
//    }
//}
//
//
///* is end of game */
//bool AtariCollectionSettings::isTerminal() const {
//
//    return m_terminal;
//};
//
//
///* get the most recently observed reward */
//reward_t AtariCollectionSettings::getReward() const {
//
//    return m_reward;
//}
//
//
///* is an action part of the minimal set? */
//bool AtariCollectionSettings::isMinimal(const Action &a) const {
//
//    switch (a) {
//        case PLAYER_A | JOYPAD_NOOP:
//        case PLAYER_A | JOYPAD_FIRE:
//        case PLAYER_A | JOYPAD_UP:
//        case PLAYER_A | JOYPAD_RIGHT:
//        case PLAYER_A | JOYPAD_LEFT:
//        case PLAYER_A | JOYPAD_DOWN:
//        case PLAYER_A | JOYPAD_UP | JOYPAD_RIGHT:
//        case PLAYER_A | JOYPAD_UP | JOYPAD_LEFT:
//        case PLAYER_A | JOYPAD_DOWN | JOYPAD_RIGHT:
//        case PLAYER_A | JOYPAD_DOWN | JOYPAD_LEFT:
//        case PLAYER_A | JOYPAD_UP | JOYPAD_FIRE:
//        case PLAYER_A | JOYPAD_RIGHT | JOYPAD_FIRE:
//        case PLAYER_A | JOYPAD_LEFT | JOYPAD_FIRE:
//        case PLAYER_A | JOYPAD_DOWN | JOYPAD_FIRE:
//        case PLAYER_A | JOYPAD_UP | JOYPAD_RIGHT | JOYPAD_FIRE:
//        case PLAYER_A | JOYPAD_UP | JOYPAD_LEFT | JOYPAD_FIRE:
//        case PLAYER_A | JOYPAD_DOWN | JOYPAD_RIGHT | JOYPAD_FIRE:
//        case PLAYER_A | JOYPAD_DOWN | JOYPAD_LEFT | JOYPAD_FIRE:
//            return true;
//        default:
//            return false;
//    }
//}
//
//
///* reset the state of the game */
//void AtariCollectionSettings::reset() {
//
//    m_reward   = 0;
//    m_score    = 0;
//    m_terminal = false;
//}
//
//
//
///* saves the state of the rom settings */
//void AtariCollectionSettings::saveState(
////		Serializer & ser
//		) {
////  ser.putInt(m_reward);
////  ser.putInt(m_score);
////  ser.putBool(m_terminal);
//}
//
//// loads the state of the rom settings
//void AtariCollectionSettings::loadState(
////		Deserializer & ser
//		) {
////  m_reward = ser.getInt();
////  m_score = ser.getInt();
////  m_terminal = ser.getBool();
//}
//
//ActionVect AtariCollectionSettings::getStartingActions(){
//	ActionVect startingActions = {JOYPAD_START};
//	return startingActions;
//}
//
