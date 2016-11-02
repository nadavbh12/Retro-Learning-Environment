/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * ******************************************************************************/

#include "../rle_interface.hpp"

namespace rle {
inline RLERAM::RLERAM(const RLERAM &rhs) {
  // Copy data over
//  memcpy(m_ram, rhs.m_ram, sizeof(m_ram));
}

inline RLERAM& RLERAM::operator=(const RLERAM &rhs) {
  // Copy data over
//  memcpy(m_ram, rhs.m_ram, sizeof(m_ram));
  m_ram = rhs.m_ram;
  return *this;
}

inline bool RLERAM::equals(const RLERAM &rhs) const {
  return (m_ram == rhs.m_ram);
}

// Byte accessors
byte_t RLERAM::get(unsigned int x) const {
  // Wrap RAM around the first 128 bytes
  return m_ram[x & 0x7F];
//  return m_ram.at(x);
}

//inline byte_t* RLERAM::byte(unsigned int x) {
//  return &m_ram[x & 0x7F];
//}

} // rle namespace
