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
 *  stella_environment.cpp
 *
 *  A class that wraps around the Stella core to provide users with a typical
 *  reinforcement learning environment interface.
 *  
 **************************************************************************** */

#include "retro_environment.hpp"

#include <sstream>

using namespace ale;

RetroEnvironment::RetroEnvironment(AleSystem* alesystem, RomSettings* settings) :
  m_alesystem(alesystem),
  m_settings(settings),
  m_phosphor_blend( ),	// TODO pass AleSystem
  m_screen(m_alesystem->getRetroAgent().getHeight(),
		  m_alesystem->getRetroAgent().getWidth()),
  m_player_a_action(PLAYER_A | JOYPAD_NOOP),
  m_player_b_action(PLAYER_B | JOYPAD_NOOP) {

	// TODO SN : Add support for paddle-based games
  // Determine whether this is a paddle-based game
//  if (m_alesystem->console().properties().get(Controller_Left) == "PADDLES" ||
//      m_alesystem->console().properties().get(Controller_Right) == "PADDLES") {
//	  m_use_paddles = true;
//	  m_state.resetPaddles(m_alesystem->event());
//  } else {
//	  m_use_paddles = false;
//  }

  m_num_reset_steps = 4;

  m_max_num_frames_per_episode = m_alesystem->settings().getInt("max_num_frames_per_episode");
  m_colour_averaging = m_alesystem->settings().getBool("color_averaging");

  m_repeat_action_probability = m_alesystem->settings().getFloat("repeat_action_probability");

  m_frame_skip = m_alesystem->settings().getInt("frame_skip");
  if (m_frame_skip < 1) {
    ale::Logger::Warning << "Warning: frame skip set to < 1. Setting to 1." << std::endl;
    m_frame_skip = 1;
  }

  // If so desired, we record all emulated frames to a given directory
  std::string recordDir = m_alesystem->settings().getString("record_screen_dir");

  getPixelFormat(*m_screen.m_pixelFormat);

  if (!recordDir.empty()) {
    ale::Logger::Info << "Recording screens to directory: " << recordDir << std::endl;


//    TODO SN : implement screen_exporter
    // Create the screen exporter
//    m_screen_exporter.reset(new ScreenExporter(m_alesystem->colourPalette(), recordDir));
  }
}
void RetroEnvironment::getPixelFormat(struct pixelFormat &pixel_format){
	uint32_t rmask, gmask, bmask, amask;
	uint32_t rShift, gShift, bShift, aShift;

	pixel_format.Bpp=m_alesystem->getRetroAgent().getBpp()/8; //in bytes
	m_alesystem->getRetroAgent().getRgbMask(rmask,  gmask,  bmask,  amask);
	m_alesystem->getRetroAgent().getRgbShift(rShift,  gShift,  bShift,  aShift);
	//mask
	pixel_format.rmask=rmask;
	pixel_format.gmask=gmask;
	pixel_format.bmask=bmask;
	pixel_format.amask=amask;
	//shift
	pixel_format.rShift=rShift;
	pixel_format.gShift=gShift;
	pixel_format.bShift=bShift;
	pixel_format.aShift=aShift;



}

/** Resets the system to its start state. */
void RetroEnvironment::reset() {
  m_state.resetEpisodeFrameNumber();
  // Reset the paddles
//  m_state.resetPaddles(m_alesystem->event());

  // Reset the emulator
//  m_alesystem->console().system().reset();
  m_alesystem->getRetroAgent().reset();

  // NOOP for 60 steps in the deterministic environment setting, or some random amount otherwise
  int noopSteps;
  noopSteps = 60;

  emulate(PLAYER_A | JOYPAD_NOOP, PLAYER_B | JOYPAD_NOOP, noopSteps);
  // reset for n steps
  emulate(JOYPAD_RESET, PLAYER_B | JOYPAD_NOOP, m_num_reset_steps);

  // reset the rom (after emulating, in case the NOOPs led to reward)
  m_settings->reset();

  // Apply necessary actions specified by the rom itself
  ActionVect startingActions = m_settings->getStartingActions();
  for (size_t i = 0; i < startingActions.size(); i++){
    emulate(startingActions[i], PLAYER_B | JOYPAD_NOOP);
    // added for debug
        m_alesystem->p_display_screen->display_screen();
  }
}

/** Save/restore the environment state. */
void RetroEnvironment::save() {
  // Store the current state into a new object
  ALEState new_state = cloneState();
  m_saved_states.push(new_state);
}

void RetroEnvironment::load() {
  // Get the state on top of the stack
  ALEState& target_state = m_saved_states.top();

  // Deserialize it into 'm_state'
  restoreState(target_state);
  m_saved_states.pop();
}

ALEState RetroEnvironment::cloneState() {
  return m_state.save(m_alesystem, m_settings, false);
}

//void RetroEnvironment::restoreState(const ALEState& target_state) {
//  m_state.load(m_alesystem, m_settings, m_cartridge_md5, target_state, false);
//}
//
//ALEState RetroEnvironment::cloneSystemState() {
//  return m_state.save(m_alesystem, m_settings, m_cartridge_md5, true);
//}
//
//void RetroEnvironment::restoreSystemState(const ALEState& target_state) {
//  m_state.load(m_alesystem, m_settings, m_cartridge_md5, target_state, true);
//}
//
void RetroEnvironment::noopIllegalActions(Action & player_a_action, Action & player_b_action) {
  if ( ((player_a_action & PLAYER_A) == PLAYER_A) &&
        !m_settings->isLegal(player_a_action)) {
    player_a_action = PLAYER_A | JOYPAD_NOOP;
  }
  // Also drop RESET, which doesn't play nice with our clean notions of RL environments
  else if (player_a_action == JOYPAD_RESET)
    player_a_action = PLAYER_A |JOYPAD_NOOP;

  if ( ((player_b_action & PLAYER_B) == PLAYER_B) &&
        !m_settings->isLegal(player_b_action) ) {
    player_b_action = PLAYER_B | JOYPAD_NOOP;
  }
  else if (player_b_action == (PLAYER_B | JOYPAD_RESET))
    player_b_action = PLAYER_B | JOYPAD_NOOP;
}

reward_t RetroEnvironment::act(Action player_a_action, Action player_b_action) {

  // Total reward received as we repeat the action
  reward_t sum_rewards = 0;

  Random& rng = m_alesystem->rng();

  // Apply the same action for a given number of times... note that act() will refuse to emulate
  //  past the terminal state
  for (size_t i = 0; i < m_frame_skip; i++) {

    // Stochastically drop actions, according to m_repeat_action_probability
    if (rng.nextDouble() >= m_repeat_action_probability)
      m_player_a_action = player_a_action;
    // @todo Possibly optimize by avoiding call to rand() when player B is "off" ?
    if (rng.nextDouble() >= m_repeat_action_probability)
      m_player_b_action = player_b_action;

//    TODO SN : fix record of sound
    // If so desired, request one frame's worth of sound (this does nothing if recording
    // is not enabled)
//    m_alesystem->sound().recordNextFrame();

    // Similarly record screen as needed
//    if (m_screen_exporter.get() != NULL)
//        m_screen_exporter->saveNext(m_screen);

    // Use the stored actions, which may or may not have changed this frame
    sum_rewards += oneStepAct(m_player_a_action, m_player_b_action);
  }

  return sum_rewards;
}

/** Applies the given actions (e.g. updating paddle positions when the paddle is used)
  *  and performs one simulation step in Stella. */
reward_t RetroEnvironment::oneStepAct(Action player_a_action, Action player_b_action) {
  // Once in a terminal state, refuse to go any further (special actions must be handled
  //  outside of this environment; in particular reset() should be called rather than passing
  //  RESET or SYSTEM_RESET.
  if (isTerminal())
    return 0;

  // Convert illegal actions into NOOPs; actions such as reset are always legal
  noopIllegalActions(player_a_action, player_b_action);

  // Emulate in the emulator
  emulate(player_a_action, player_b_action);
  // Increment the number of frames seen so far
  m_state.incrementFrame();

  return m_settings->getReward();
}

bool RetroEnvironment::isTerminal() {
  return (m_settings->isTerminal() ||
    (m_max_num_frames_per_episode > 0 &&
     m_state.getEpisodeFrameNumber() >= m_max_num_frames_per_episode));
}

void RetroEnvironment::emulate(Action player_a_action, Action player_b_action, size_t num_steps) {

//  // Handle paddles separately: we have to manually update the paddle positions at each step
//  if (m_use_paddles) {
//    // Run emulator forward for 'num_steps'
//    for (size_t t = 0; t < num_steps; t++) {
//      // Update paddle position at every step
//      m_state.applyActionPaddles(event, player_a_action, player_b_action);
//
//      m_alesystem->console().mediaSource().update();
//      m_settings->step(m_alesystem->console().system());
//    }
//  }
//  else {
    // In joystick mode we only need to set the action events once
//    m_state.setActionJoysticks(player_a_action, player_b_action);
	m_alesystem->getRetroAgent().SetActions(player_a_action,player_b_action);

    for (size_t t = 0; t < num_steps; t++) {
    	m_alesystem->step();
    	m_settings->step(*m_alesystem);
    }
//  }

    // TODO SN : implement functions below
  // Parse screen and RAM into their respective data structures
  processScreen();
//  processRAM();
}

///** Accessor methods for the environment state. */
//void RetroEnvironment::setState(const ALEState& state) {
//  m_state = state;
//}
//
//const ALEState& RetroEnvironment::getState() const {
//  return m_state;
//}

void RetroEnvironment::processScreen() {
//  if (m_colour_averaging) {
//    // Perform phosphor averaging; the blender stores its result in the given screen
//    m_phosphor_blend.process(m_screen);
//  }
//  else {
    // Copy screen over and we're done!
//    memcpy(m_screen.getArray(),(uint32_t*) m_alesystem->getCurrentFrameBuffer(), 4*m_alesystem->getRetroAgent().getBufferSize()); //shai: consider adding min/max of size // *4 to go to pixel
	int height = m_alesystem->getRetroAgent().getHeight();
	int width = m_alesystem->getRetroAgent().getWidth();
	int Bpp = m_alesystem->getRetroAgent().getBpp() / 8;
	int pitch = m_alesystem->getRetroAgent().getPitch();
	uint8_t* buffer = m_alesystem->getCurrentFrameBuffer();
	for(int i = 0 ; i < height; i++){
		memcpy((uint8_t*)m_screen.getArray() + i*width *Bpp , buffer + i*pitch, width * Bpp);
	}


////	FILE *a = fopen("/home/administrator/DQN/DeepMind-Atari-Deep-Q-Learner/cWrite1.txt", "w");
//	FILE *b = fopen("/home/administrator/DQN/DeepMind-Atari-Deep-Q-Learner/cWrite2.txt", "w");
//    uint8_t* originBuffer = m_alesystem->getCurrentFrameBuffer();
//    uint8_t* destBuffer = (uint8_t*)m_screen.getArray();
//    size_t size = m_alesystem->getRetroAgent().getHeight() * m_alesystem->getRetroAgent().getPitch();
//    for( int i = 0; i < height * width * Bpp ; i++){
////    	if(i % m_alesystem->getRetroAgent().getPitch() == 0){
////    		fprintf(a,"\n");
////    	}
//    	if(i % (width * Bpp) == 0){
//			fprintf(b,"\n");
//		}
////    	fprintf(a,"%x, ", originBuffer[i]);
//    	fprintf(b,"%x, ", destBuffer[i]);
////		fprintf(a,"%d, ", i);
//
//    }
////    fclose(a);
//    fclose(b);
}

void RetroEnvironment::processRAM() {
  // Copy RAM over
////  for (size_t i = 0; i < m_ram.size(); i++)
////    *m_ram.byte(i) = m_alesystem->console().system().peek(i + 0x80);
//	uint32_t* memPtr = m_alesystem->getRetroAgent().getRamAddress(RETRO_MEMORY_SYSTEM_RAM);
//	uint32_t memSize = m_alesystem->getRetroAgent().getRamSize();
//	DEBUG2("mem address " << memPtr);
//	DEBUG2("mem size " << memSize );
//	DEBUG2("value at end " << *(memPtr + memSize/8) );
////	std::copy(memPtr, memPtr + memSize/8, m_ram.array().begin());
//	m_ram.array().clear();
//	m_ram.array().reserve(memSize);
//	std::copy(memPtr, memPtr + memSize/8, m_ram.array().begin());
}
