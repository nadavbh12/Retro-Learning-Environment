/*
 * RleTest.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: nadav
 */

#include "../src/environment/RetroAgent.h"
#include "../src/environment/Serializer.hxx"
#include "../src/environment/Deserializer.hxx"
#include <algorithm>
#include <iterator>
#include <string>
#include "gtest/gtest.h"

namespace {
using namespace ale;
// The fixture for testing class Foo.
class RetroAgentTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	RetroAgentTest() {
    // You can do set-up work for each test here.
		adapter.loadCore("/home/nadav/DQN/Arcade-Learning-Environment-2.0/snes9x-next/snes9x_next_libretro.so");
		adapter.loadRom("/home/nadav/DQN/roms/mortal_kombat.sfc");
		int numSteps = 50;

		for(int i=0; i< numSteps; i++){
			adapter.run();
		}

  }

  // Objects declared here can be used by all tests in the test case for Foo.
  RetroAgent adapter;
};

TEST_F(RetroAgentTest, serialization) {
	std::vector<uint8_t> dataOld;
	std::vector<uint8_t> dataNew;
	dataOld.assign(adapter.getRamAddress(2), adapter.getRamAddress(2) + adapter.getRamSize());
	Serializer ser;
	adapter.serialize(ser);

	int numSteps = 50;
	for(int i = 0 ; i < numSteps ; i++){
		adapter.run();
	}

	std::string serString = ser.get_str();
	Deserializer des(ser.get_str());
	adapter.deserialize(des);
	dataNew.assign(adapter.getRamAddress(2), adapter.getRamAddress(2) + adapter.getRamSize());

	EXPECT_EQ(dataOld, dataNew);
}

}
