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
#include "ale_state.hpp"
#include "../common/Constants.h"
#include "Serializer.hxx"
#include "Deserializer.hxx"
using namespace std;
using namespace ale;

#include <stdexcept>

/** Default constructor - loads settings from system */ 
ALEState::ALEState():
  m_frame_number(0),
  m_episode_frame_number(0) {
}

ALEState::ALEState(const ALEState &rhs, std::string serialized):
  m_frame_number(rhs.m_frame_number),
  m_episode_frame_number(rhs.m_episode_frame_number),
  m_serialized_state(serialized) {
}

ALEState::ALEState(const std::string &serialized) {
  Deserializer des(serialized);
  this->m_frame_number = des.getInt();
  this->m_episode_frame_number = des.getInt();
  this->m_serialized_state = des.getString();
}


/** Restores ALE to the given previously saved state. */ 
void ALEState::load(
		AleSystem* alesystem,
		RomSettings* settings, const ALEState &rhs,
		bool load_system) {
  assert(rhs.m_serialized_state.length() > 0);

  // Deserialize the stored string into the emulator state
  Deserializer deser(rhs.m_serialized_state);

  // A primitive check to produce a meaningful error if this state does not contain osystem info.
  if (deser.getBool() != load_system)
    throw new std::runtime_error("Attempting to load an ALEState which does not contain "
        "system information.");

  alesystem->getRetroAgent().deserialize(deser);
  // If we have osystem data, load it as well
  if (load_system)
    alesystem->loadState(deser);
  settings->loadState(deser);

  m_episode_frame_number = rhs.m_episode_frame_number;
  m_frame_number = rhs.m_frame_number;
}

ALEState ALEState::save(
		AleSystem* alesystem,
		RomSettings* settings,
		bool save_system) {
  // Use the emulator's built-in serialization to save the state
  Serializer ser;

  // We use 'save_system' as a check at load time.
  ser.putBool(save_system);

  alesystem->getRetroAgent().serialize(ser);
  if (save_system)
	  alesystem->saveState(ser);
  settings->saveState(ser);

  // Now make a copy of this state, also storing the emulator serialization
  return ALEState(*this, ser.get_str());
}

void ALEState::incrementFrame(int steps /* = 1 */) {
    m_frame_number += steps;
    m_episode_frame_number += steps;
}

void ALEState::resetEpisodeFrameNumber() {
    m_episode_frame_number = 0;
}

std::string ALEState::serialize() {
  Serializer ser;
//
//  ser.putInt(this->m_left_paddle);
//  ser.putInt(this->m_right_paddle);
  ser.putInt(this->m_frame_number);
  ser.putInt(this->m_episode_frame_number);
  ser.putString(this->m_serialized_state);

  return ser.get_str();
}

bool ALEState::equals(ALEState &rhs) {
  return (rhs.m_serialized_state == this->m_serialized_state &&
//    rhs.m_left_paddle == this->m_left_paddle &&
//    rhs.m_right_paddle == this->m_right_paddle &&
    rhs.m_frame_number == this->m_frame_number &&
    rhs.m_episode_frame_number == this->m_episode_frame_number);
}
