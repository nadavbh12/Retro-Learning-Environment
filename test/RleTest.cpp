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
  // You can remove any or all of the following functions if its body
  // is empty.

	RleTest() {
    // You can do set-up work for each test here.
  }

  virtual ~RleTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

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

	EXPECT_THROW(ale::ALEInterface ale2, AleException);
}

TEST_F(RleTest, runTwoAgentsWithDelete) {
	auto ale = new ale::ALEInterface();
	run_example(ale, romPath, corePath);
	delete ale;

	ale::ALEInterface ale2;
	run_example(&ale2, romPath, corePath);
}


}  // namespace
