/*
 * RleTest.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: nadav
 */

#include "rle_c_wrapper.h"
#include "gtest/gtest.h"

namespace {

class RleCInterfaceTest : public ::testing::Test {
};

TEST_F(RleCInterfaceTest, simpleCtorDtor) {
	RLEInterface* rle = RLE_new();
	RLE_del(rle);
}

//TEST_F(RleCInterfaceTest, getScreenRGB) {
//	RLEInterface *rle = RLE_new();
//	loadROM(rle,"/home/administrator/DQN/roms/mortal_kombat.sfc", "/home/administrator/DQN/tempDQN/Arcade-Learning-Environment-2.0/snes9x-next/snes9x_next_libretro.so");
//	reset_game(rle);
//	for(int i = 0; i < 50; ++i){
//		act(rle, (Action)JOYPAD_NOOP,(Action)JOYPAD_NOOP);
//	}
//	int height = getScreenHeight(rle);
//	int width = getScreenWidth(rle);
//	uint8_t* output_buffer = new uint8_t[height * width];
//	getScreenRGB(rle,output_buffer);
//}

void loadAndPlay(RLEInterface* rle){
	loadROM(rle,"/home/nadav/DQN/roms/mortal_kombat.sfc", "/home/nadav/DQN/Arcade-Learning-Environment-2.0/snes9x2010/snes9x2010_libretro.so");
	reset_game(rle);
	for(int i = 0; i < 50; ++i){
		act(rle, (Action)JOYPAD_NOOP,(Action)JOYPAD_NOOP);
	}
}

TEST_F(RleCInterfaceTest, getScreenGrayscrle) {
	RLEInterface *rle = RLE_new();
	loadAndPlay(rle);
	int height = getScreenHeight(rle);
	int width = getScreenWidth(rle);
	uint8_t* output_buffer = new uint8_t[height * width];
	getScreenGrayscrle(rle,output_buffer);
	RLE_del(rle);
}

TEST_F(RleCInterfaceTest, setBool) {
	RLEInterface* rle = RLE_new();
	setBool(rle, "display_screen", true);
	loadAndPlay(rle);
	reset_game(rle);
	RLE_del(rle);
}


}  // namespace
