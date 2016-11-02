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
 *  rle_ram.hpp
 *
 *  A class that encapsulates the Atari 2600 RAM. Code is provided inline for
 *   efficiency reasonss.
 *  
 **************************************************************************** */

#ifndef __RLE_RAM_HPP__
#define __RLE_RAM_HPP__

#include <string.h>

//typedef unsigned char byte_t;
//
//#define RAM_SIZE (128)
//
///** A simple wrapper around the Atari RAM. */
//class RLERAM {
//  public:
//    RLERAM();
//    RLERAM(const RLERAM &rhs);
//
//    RLERAM& operator=(const RLERAM &rhs);
//
//    /** Byte accessors */
//    byte_t get(unsigned int x) const;
//    byte_t *byte(unsigned int x);
//
//    /** Returns the whole array (equivrlent to byte(0)). */
//    byte_t *array() const { return (byte_t*)(m_ram); }
//
//    size_t size() const { return sizeof(m_ram); }
//    /** Returns whether two copies of the RAM are equal */
//    bool equals(const RLERAM &rhs) const;
//
//  protected:
//    byte_t m_ram[RAM_SIZE];
//};
//
//inline RLERAM::RLERAM() {
//}
//
//inline RLERAM::RLERAM(const RLERAM &rhs) {
//  // Copy data over
//  memcpy(m_ram, rhs.m_ram, sizeof(m_ram));
//}
//
//inline RLERAM& RLERAM::operator=(const RLERAM &rhs) {
//  // Copy data over
//  memcpy(m_ram, rhs.m_ram, sizeof(m_ram));
//
//  return *this;
//}
//
//inline bool RLERAM::equals(const RLERAM &rhs) const {
//  return (memcmp(m_ram, rhs.m_ram, size()) == 0);
//}
//
//// Byte accessors
//inline byte_t RLERAM::get(unsigned int x) const {
//  // Wrap RAM around the first 128 bytes
//  return m_ram[x & 0x7F];
//}
//
//inline byte_t* RLERAM::byte(unsigned int x) {
//  return &m_ram[x & 0x7F];
//}

#endif // __RLE_RAM_HPP__

