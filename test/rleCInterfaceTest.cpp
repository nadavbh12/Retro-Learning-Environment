/*
 * RleTest.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: nadav
 */

#include "ale_c_wrapper.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class RleCInterfaceTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	RleCInterfaceTest() {
    // You can do set-up work for each test here.
  }

  virtual ~RleCInterfaceTest() {
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
};

TEST_F(RleCInterfaceTest, simpleCtorDtor) {
	ALEInterface* ale = ALE_new();
	ALE_del(ale);
}

//TEST_F(RleCInterfaceTest, getScreenRGB) {
//	ALEInterface *ale = ALE_new();
//	loadROM(ale,"/home/administrator/DQN/roms/mortal_kombat.sfc", "/home/administrator/DQN/tempDQN/Arcade-Learning-Environment-2.0/snes9x-next/snes9x_next_libretro.so");
//	reset_game(ale);
//	for(int i = 0; i < 50; ++i){
//		act(ale, (Action)JOYPAD_NOOP,(Action)JOYPAD_NOOP);
//	}
//	int height = getScreenHeight(ale);
//	int width = getScreenWidth(ale);
//	uint8_t* output_buffer = new uint8_t[height * width];
//	getScreenRGB(ale,output_buffer);
//}

void loadAndPlay(ALEInterface* ale){
	loadROM(ale,"/home/nadav/DQN/roms/mortal_kombat.sfc", "/home/nadav/DQN/Arcade-Learning-Environment-2.0/snes9x2010/snes9x2010_libretro.so");
	reset_game(ale);
	for(int i = 0; i < 50; ++i){
		act(ale, (Action)JOYPAD_NOOP,(Action)JOYPAD_NOOP);
	}
}

TEST_F(RleCInterfaceTest, getScreenGrayscale) {
	ALEInterface *ale = ALE_new();
	loadAndPlay(ale);
	int height = getScreenHeight(ale);
	int width = getScreenWidth(ale);
	uint8_t* output_buffer = new uint8_t[height * width];
	getScreenGrayscale(ale,output_buffer);
	ALE_del(ale);
}

TEST_F(RleCInterfaceTest, setBool) {
	ALEInterface* ale = ALE_new();
	ale->setBool("display_screen", true);
	loadAndPlay(ale);
	reset_game(ale);
	ALE_del(ale);
}


}  // namespace
