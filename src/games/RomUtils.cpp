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
// *
// * RomUtils.hpp
// *
// * Additional utilities to operate on RAM.
// * *****************************************************************************
// */
//#include "RomUtils.hpp"
//
//#include "RleSystem.hxx"
//
//namespace rle {
//
///* reads a byte at a memory location between 0 and 128 */
//
//#include "../libretro.h"
//
//int readRam(const RleSystem* system, int offset) {
//
////	return system->getRetroAgent().readRam(RETRO_MEMORY_SYSTEM_RAM, (offset & 0x7F) + 0x80);
//	return system->getRetroAgent().readRam(RETRO_MEMORY_SYSTEM_RAM, (offset & 0x7F) );
////	return system->getRetroAgent().readRam(RETRO_MEMORY_SYSTEM_RAM, (offset) );
//}
//
//
///* extracts a decimal value from a byte */
//int getDecimalScore(int index, const RleSystem* system) {
//
//    int score = 0;
//    int digits_val = readRam(system, index);
//    digits_val&= 0xff;				// SN: added since values may be large. TODO : handle negative numbers
//    int right_digit = digits_val & 15;
//    int left_digit = digits_val >> 4;
//    score += ((10 * left_digit) + right_digit);
//
//    return score;
//}
//
///* extracts a decimal value from 2 bytes */
//int getDecimalScore(int lower_index, int higher_index, const RleSystem* system) {
//
//    int score = 0;
//    int lower_digits_val = readRam(system, lower_index);
//    lower_digits_val &= 0xff;	// SN: added since values may be large. TODO : handle negative numbers
//    int lower_right_digit = lower_digits_val & 0xf;
//    int lower_left_digit = (lower_digits_val - lower_right_digit) >> 4;
//    score += ((10 * lower_left_digit) + lower_right_digit);
//    if (higher_index < 0) {
//        return score;
//    }
//    int higher_digits_val = readRam(system, higher_index);
//    higher_digits_val &= 0xff;	// SN: added since values may be large. TODO : handle negative numbers
//    int higher_right_digit = higher_digits_val & 0xf;
//    int higher_left_digit = (higher_digits_val - higher_right_digit) >> 4;
//    score += ((1000 * higher_left_digit) + 100 * higher_right_digit);
//    return score;
//}
//
//
///* extracts a decimal value from 3 bytes */
//int getDecimalScore(int lower_index, int middle_index, int higher_index, const RleSystem* system) {
//
//    int score = getDecimalScore(lower_index, middle_index, system);
//    int higher_digits_val = readRam(system, higher_index);
//    higher_digits_val &= 0xff;	// SN: added since values may be large. TODO : handle negative numbers
//    int higher_right_digit = higher_digits_val & 15;
//    int higher_left_digit = (higher_digits_val - higher_right_digit) >> 4;
//    score += ((100000 * higher_left_digit) + 10000 * higher_right_digit);
//    return score;
//}
//
//} // namespace rle
