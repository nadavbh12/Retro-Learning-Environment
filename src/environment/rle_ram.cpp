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
#include "../common/RleException.h"

namespace rle {

inline bool RLERAM::equals(const RLERAM &rhs) const {
  return (m_ram == rhs.m_ram);
}

// Byte accessors
byte_t RLERAM::get(unsigned int x) const {
  if(x < m_ram_size){
	return m_ram[x];
  }
  else{
	throw RleException("Invalid Ram Address");
  }
}

inline byte_t* RLERAM::byte(unsigned int x) {
  if(x < m_ram_size){
	return m_ram + x;
  }
  else{
	throw RleException("Invalid Ram Address");
  }
}

} // rle namespace
