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
#include <thread>
#include "gtest/gtest.h"
#include "gtest_arguments.h"

namespace {
using namespace rle;
// The fixture for testing class Foo.
class RetroAgentTest : public ::testing::Test {
 protected:

	RetroAgentTest() {}

	string corePath = gtest_arguments::coreName;
	string romPath = gtest_arguments::romName;
};

static void initRetroAgent(RetroAgent* adapter, string corePath, string romPath){
	adapter->loadCore(corePath);
	adapter->loadRom(romPath);
	int numSteps = 50;

	for(int i=0; i< numSteps; i++){
		adapter->run();
	}
}


TEST_F(RetroAgentTest, serialization) {
	RetroAgent adapter;
	initRetroAgent(&adapter, corePath, romPath);

	std::vector<uint8_t> dataOld;
	std::vector<uint8_t> dataNew;
	dataOld.assign(adapter.getRamAddress(), adapter.getRamAddress() + adapter.getRamSize());
	Serializer ser;
	adapter.serialize(ser);

	int numSteps = 500;
	for(int i = 0 ; i < numSteps ; i++){
		adapter.run();
	}

	std::string serString = ser.get_str();
	Deserializer des(ser.get_str());
	adapter.deserialize(des);
	dataNew.assign(adapter.getRamAddress(), adapter.getRamAddress() + adapter.getRamSize());

	EXPECT_EQ(dataOld, dataNew);
}

TEST_F(RetroAgentTest, multiThreading) {
	int numThreads(8);
	RetroAgent adapter[numThreads];
	std::vector<std::thread> threads;
	for(int i(0); i < numThreads; ++i){
		threads.push_back(std::thread(initRetroAgent, &adapter[i], corePath, romPath));
	}
	for (auto& th : threads){
	    th.join();
	}
}

TEST_F(RetroAgentTest, multiLoadRom) {
	RetroAgent agent;
	initRetroAgent(&agent, corePath, romPath);
	for(int i(0); i< 2; i++){
		agent.loadRom(romPath);
		for(int j(0); j< 100; j++){
			agent.run();
		}
//		agent.reset();
	}
}

}
