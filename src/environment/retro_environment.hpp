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
 *  stella_environment.hpp
 *
 *  A class that wraps around the Stella core to provide users with a typical
 *  reinforcement learning environment interface.
 *  
 **************************************************************************** */

#ifndef __RLE_ENVIRONMENT_HPP__
#define __RLE_ENVIRONMENT_HPP__

#include "rle_state.hpp"
#include "../rle_interface.hpp"
#include "phosphor_blend.hpp"
#include "../games/RomSettings.hpp"
#include "../common/ScreenExporter.hpp"
#include "../common/Log.hpp"
#include "../common/DebugMacros.h"

#include "RetroAgent.h"
#include "RleSystem.hxx"
#include <stack>

namespace rle {

struct pixelFormat {
   uint32_t rmask;
   uint32_t gmask;
   uint32_t bmask;
   uint32_t amask;
   uint32_t rShift;
   uint32_t gShift;
   uint32_t bShift;
   uint32_t aShift;
   uint32_t rFill;
   uint32_t gFill;
   uint32_t bFill;
   int Bpp; //Bytes per pixel
};

// This defines the number of "random" environments
#define NUM_RANDOM_ENVIRONMENTS (500)

class RetroEnvironment {

	public:
    RetroEnvironment(
    		RleSystem * system,
    		RomSettings * settings);

    /** Resets the system to its start state. */
    void reset();

    /** Save/restore the environment state onto the stack. */
    void save();
    void load();

    /** Returns a copy of the current emulator state. Note that this doesn't include
        pseudorandomness, so that clone/restoreState are suitable for planning. */
    RLEState cloneState();
    /** Restores a previously saved copy of the state. */
    void restoreState(const RLEState&);

    /** Returns a copy of the current emulator state. This includes RNG state information, and
        more generally should lead to exactly reproducibility. */
    RLEState cloneSystemState();
    /** Restores a previously saved copy of the state, including RNG state information. */
    void restoreSystemState(const RLEState&);

    /** Applies the given actions (e.g. updating paddle positions when the paddle is used)
      *  and performs one simulation step in Stella. Returns the resultant reward. When 
      *  frame skip is set to > 1, up the corresponding number of simulation steps are performed.
      *  Note that the post-act() frame number might not correspond to the pre-act() frame
      *  number plus the frame skip.
      */
    reward_t act(Action player_a_action, Action player_b_action);

    /** Returns true once we reach a terminal state */
    bool isTerminal();

    /** Accessor methods for the environment state. */
    void setState(const RLEState & state);
    const RLEState &getState() const;

    /** Returns the current screen after processing (e.g. color averaging) */

    const RLEScreen &getScreen() const { return m_screen; }
    const RLERAM &getRAM() const { return m_ram; }

    int getFrameNumber() const{ return m_state.getFrameNumber(); }
    int getEpisodeFrameNumber() const {return m_state.getEpisodeFrameNumber(); }

  private:
    /** This applies an action exactly one time step. Helper function to act(). */
    reward_t oneStepAct(Action player_a_action, Action player_b_action);

    /** Actually emulates the emulator for a given number of steps. */
    void emulate(const Action& player_a_action, const Action& player_b_action, size_t num_steps = 1);

    /** Emulates the emulator without processing ram and screen for a given number of steps. */
	void emulateStart(Action player_a_action, Action player_b_action, size_t num_steps = 1);

    /** Drops illegal actions, such as the fire button in skiing. Note that this is different
      *   from the minimal set of actions. */
    void noopIllegalActions(Action& player_a_action, Action& player_b_action);

    /** Processes the current emulator screen and saves it in m_screen */
    void processScreen();

    void getPixelFormat(struct pixelFormat &m_pixelFormat);

  private:
    RleSystem *m_rlesystem;
    RomSettings *m_settings;
    PhosphorBlend m_phosphor_blend; // For performing phosphor colour averaging, if so desired
//    std::string m_cartridge_md5; // Necessary for saving and loading emulator state

    std::stack<RLEState> m_saved_states; // States are saved on a stack
    
    RLEState m_state; // Current environment state    
    RLEScreen m_screen; // The current ALE screen (possibly colour-averaged)
    RLERAM m_ram; // The current ALE RAM

//    bool m_use_paddles;  // Whether this game uses paddles
    
    /** Parameters loaded from Settings. */
    int m_num_reset_steps; // Number of RESET frames per reset
    bool m_colour_averaging; // Whether to average frames
    int m_max_num_frames_per_episode; // Maxmimum number of frames per episode 
    size_t m_frame_skip; // How many frames to emulate per act()
    float m_repeat_action_probability; // Stochasticity of the environment
//    std::shared_ptr<ScreenExporter> m_screen_exporter; // Automatic screen recorder

    // The last actions taken by our players
    Action m_player_a_action, m_player_b_action;

};

} // namespace rle

#endif // __RLE_ENVIRONMENT_HPP__
