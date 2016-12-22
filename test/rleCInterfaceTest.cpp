/*
 * RleTest.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: nadav
 */

#include "rle_c_wrapper.h"
#include "gtest/gtest.h"
#include "gtest_arguments.h"

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
	loadROM(rle, gtest_arguments::romName.c_str(), gtest_arguments::coreName.c_str());
	reset_game(rle);
	for(int i = 0; i < 50; ++i){
		act(rle, (Action)JOYPAD_NOOP,(Action)JOYPAD_NOOP);
	}
}

TEST_F(RleCInterfaceTest, getScreenGrayscale) {
	RLEInterface *rle = RLE_new();
	loadAndPlay(rle);
	int height = getScreenHeight(rle);
	int width = getScreenWidth(rle);
	uint8_t* output_buffer = new uint8_t[height * width];
	getScreenGrayscale(rle,output_buffer);
	RLE_del(rle);
}

TEST_F(RleCInterfaceTest, setBool) {
	RLEInterface* rle = RLE_new();
	setBool(rle, "display_screen", true);
	loadAndPlay(rle);
	reset_game(rle);
	RLE_del(rle);
}

TEST_F(RleCInterfaceTest, testRam) {
	RLEInterface *rle = RLE_new();
	EXPECT_ANY_THROW(getRAMSize(rle));
	loadAndPlay(rle);
	size_t ramSize = getRAMSize(rle);
	EXPECT_EQ(128 * 1024, ramSize);
	unsigned char * ram = (unsigned char *)malloc(sizeof(unsigned char *)*ramSize);
	getRAM(rle, ram);
	EXPECT_NO_THROW(ram[0]);
	EXPECT_NO_THROW(ram[ramSize-1]);
}


}  // namespace
