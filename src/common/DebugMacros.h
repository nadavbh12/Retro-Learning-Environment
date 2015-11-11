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
 *  common_constants.h
 *
 *  Defines a set of constants used by various parts of the player agent code
 *
 **************************************************************************** */

#ifndef __DEBUG_MACROS_H__
#define __DEBUG_MACROS_H__

using namespace std;

#define COLOR_RESET   "\033[0m"
#define COLOR_BLACK   "\033[30m"      /* Black */
#define COLOR_RED     "\033[31m"      /* Red */
#define COLOR_GREEN   "\033[32m"      /* Green */
#define COLOR_YELLOW  "\033[33m"      /* Yellow */
#define COLOR_BLUE    "\033[34m"      /* Blue */
#define COLOR_MAGENTA "\033[35m"      /* Magenta */
#define COLOR_CYAN    "\033[36m"      /* Cyan */
#define COLOR_WHITE   "\033[37m"      /* White */


// Debug functions
#define PRINT_FUNCTION_NAME 1
#define DEBUG 1

#define DEBUG2(x) \
    if(DEBUG == 1){\
    cout << COLOR_YELLOW <<  "line: " << __LINE__ <<" "<< (x) << COLOR_RESET << endl;}


#define FUNCTION_NAME \
if (PRINT_FUNCTION_NAME){\
    cout << "function: " << __func__ << endl;\
}

#define EMPTY_FUNCTION \
	std::cout << COLOR_BLUE <<  __FUNCTION__ << " is empty" << COLOR_RESET << endl;


#endif // __DEBUG_MACROS_H__

