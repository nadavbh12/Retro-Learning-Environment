/*
 * RleTest.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: nadav
 */

#include <thread>
#include <string>
#include "ale_interface.hpp"
#include "gtest/gtest.h"
#include "common/AleException.h"

namespace {

using namespace ale;
using std::string;
// The fixture for testing class Foo.
class RleTest : public ::testing::Test {
 protected:

  // Objects declared here can be used by all tests in the test case for Foo.
	std::string corePath = "/home/nadav/DQN/Arcade-Learning-Environment-2.0/snes9x2010/snes9x2010_libretro.so";
	std::string romPath = "/home/nadav/DQN/roms/mortal_kombat.sfc";
};

TEST_F(RleTest, simpleCtor) {
	ale::ALEInterface ale;
}

static void run_example(ALEInterface* ale, string romPath, string corePath){
	ale->loadROM(romPath, corePath);
	ActionVect legal_actions = ale->getMinimalActionSet();

	for (int episode=0; episode<1; ++episode) {
		int steps(0);
		while (!ale->game_over() && steps < 100) {
			Action a = legal_actions[rand() % legal_actions.size()];
			ale->act(a);
			++steps;
		}
		ale->reset_game();
	}
}

TEST_F(RleTest, multiThreading) {
	int numThreads(8);
	ale::ALEInterface ale[numThreads];
	std::vector<std::thread> threads;
	for(int i(0); i < numThreads; ++i){
		threads.push_back(std::thread(run_example, &ale[i], romPath, corePath));
	}
	for (auto& th : threads){
	    th.join();
	}
}

TEST_F(RleTest, runTwoAgentsNoDelete) {
	ale::ALEInterface ale;
	run_example(&ale, romPath, corePath);
	ale::ALEInterface ale2;
	ale.getEpisodeFrameNumber();
}

TEST_F(RleTest, runTwoAgentsNoDeleteWithDoubleInit) {
	ale::ALEInterface ale;
	run_example(&ale, romPath, corePath);
	ale::ALEInterface ale2;
	run_example(&ale2, romPath, corePath);
	ale.getEpisodeFrameNumber();
}

TEST_F(RleTest, runTwoAgentsWithDelete) {
	auto ale = new ale::ALEInterface();
	run_example(ale, romPath, corePath);
	delete ale;

	ale::ALEInterface ale2;
	run_example(&ale2, romPath, corePath);
}


}  // namespace
