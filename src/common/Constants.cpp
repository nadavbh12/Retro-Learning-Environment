/* *****************************************************************************
 * A.L.E (Atari 2600 Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf
 * Released under GNU General Public License www.gnu.org/licenses/gpl-3.0.txt
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  Constants.cpp
 *
 *  Defines a set of constants used by various parts of the player agent code
 *
 **************************************************************************** */

#include "Constants.h"
#include <sstream>

using namespace rle;

#define CAT_STR(s, x, y) if ((x & y) > 0){ if (first){first=false;s << #y ;} else {s << " | " << #y ;}}

std::string rle::action_to_string(rle::Action a) {
	std::stringstream ss;
	bool first = true;
	CAT_STR(ss, a, JOYPAD_B                 );
	CAT_STR(ss, a, JOYPAD_Y                 );
	CAT_STR(ss, a, JOYPAD_SELECT            );
	CAT_STR(ss, a, JOYPAD_START             );
	CAT_STR(ss, a, JOYPAD_UP                );
	CAT_STR(ss, a, JOYPAD_DOWN              );
	CAT_STR(ss, a, JOYPAD_LEFT              );
	CAT_STR(ss, a, JOYPAD_RIGHT             );
	CAT_STR(ss, a, JOYPAD_A                 );
	CAT_STR(ss, a, JOYPAD_X                 );
	CAT_STR(ss, a, JOYPAD_L                 );
	CAT_STR(ss, a, JOYPAD_R                 );
	CAT_STR(ss, a, JOYPAD_L2                );
	CAT_STR(ss, a, JOYPAD_R2                );
	CAT_STR(ss, a, JOYPAD_L3                );
	CAT_STR(ss, a, JOYPAD_R3                );
	CAT_STR(ss, a, JOYPAD_NOOP				);
	CAT_STR(ss, a, JOYPAD_RESET				);
	CAT_STR(ss, a, JOYPAD_UNDEFINED			);
	CAT_STR(ss, a, JOYPAD_RANDOM			);
	CAT_STR(ss, a, JOYPAD_SAVE_STATE		);
	CAT_STR(ss, a, JOYPAD_LOAD_STATE		);
	CAT_STR(ss, a, JOYPAD_SYSTEM_RESET		);
	CAT_STR(ss, a, JOYPAD_LAST_ACTION_INDEX	);
	CAT_STR(ss, a, PLAYER_A					);
	CAT_STR(ss, a, PLAYER_B					);
	return ss.str();
}
