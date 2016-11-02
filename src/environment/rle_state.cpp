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
 */
#include "rle_state.hpp"
#include "../common/Constants.h"
#include "Serializer.hxx"
#include "Deserializer.hxx"
using namespace std;
using namespace rle;

#include <stdexcept>

/** Default constructor - loads settings from system */ 
RLEState::RLEState():
  m_frame_number(0),
  m_episode_frame_number(0) {
}

RLEState::RLEState(const RLEState &rhs, std::string serialized):
  m_frame_number(rhs.m_frame_number),
  m_episode_frame_number(rhs.m_episode_frame_number),
  m_serialized_state(serialized) {
}

RLEState::RLEState(const std::string &serialized) {
  Deserializer des(serialized);
  this->m_frame_number = des.getInt();
  this->m_episode_frame_number = des.getInt();
  this->m_serialized_state = des.getString();
}


/** Restores ALE to the given previously saved state. */ 
void RLEState::load(
		RleSystem* rlesystem,
		RomSettings* settings, const RLEState &rhs,
		bool load_system) {
  assert(rhs.m_serialized_state.length() > 0);

  // Deserialize the stored string into the emulator state
  Deserializer deser(rhs.m_serialized_state);

  // A primitive check to produce a meaningful error if this state does not contain osystem info.
  if (deser.getBool() != load_system)
    throw new std::runtime_error("Attempting to load an RLEState which does not contain "
        "system information.");

  rlesystem->getRetroAgent().deserialize(deser);
  // If we have osystem data, load it as well
  if (load_system)
    rlesystem->loadState(deser);
  settings->loadState(deser);

  m_episode_frame_number = rhs.m_episode_frame_number;
  m_frame_number = rhs.m_frame_number;
}

RLEState RLEState::save(
		RleSystem* rlesystem,
		RomSettings* settings,
		bool save_system) {
  // Use the emulator's built-in serialization to save the state
  Serializer ser;

  // We use 'save_system' as a check at load time.
  ser.putBool(save_system);

  rlesystem->getRetroAgent().serialize(ser);
  if (save_system)
	  rlesystem->saveState(ser);
  settings->saveState(ser);

  // Now make a copy of this state, also storing the emulator serialization
  return RLEState(*this, ser.get_str());
}

void RLEState::incrementFrame(int steps /* = 1 */) {
    m_frame_number += steps;
    m_episode_frame_number += steps;
}

void RLEState::resetEpisodeFrameNumber() {
    m_episode_frame_number = 0;
}

std::string RLEState::serialize() {
  Serializer ser;
//
//  ser.putInt(this->m_left_paddle);
//  ser.putInt(this->m_right_paddle);
  ser.putInt(this->m_frame_number);
  ser.putInt(this->m_episode_frame_number);
  ser.putString(this->m_serialized_state);

  return ser.get_str();
}

bool RLEState::equals(RLEState &rhs) {
  return (rhs.m_serialized_state == this->m_serialized_state &&
//    rhs.m_left_paddle == this->m_left_paddle &&
//    rhs.m_right_paddle == this->m_right_paddle &&
    rhs.m_frame_number == this->m_frame_number &&
    rhs.m_episode_frame_number == this->m_episode_frame_number);
}
