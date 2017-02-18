/*
 * RleTest.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: nadav
 */

#include <thread>
#include <string>
#include "rle_interface.hpp"
#include "gtest/gtest.h"
#include "common/RleException.h"
#include "gtest_arguments.h"
#include "../src/environment/Settings.hxx"

namespace {

using namespace rle;
using std::string;
// The fixture for testing class Foo.
class SettingsTest : public ::testing::Test {
 protected:

  // Objects declared here can be used by all tests in the test case for Foo.
	std::string corePath = gtest_arguments::coreName;
	std::string romPath = gtest_arguments::romName;
};

TEST_F(SettingsTest, simpleCtor) {
	rle::Settings settings;
}

TEST_F(SettingsTest, getSet) {
	rle::Settings settings;
	EXPECT_EQ(0, settings.getInt("random_seed"));
	EXPECT_EQ(0, settings.getBool("two_players"));
}


}  // namespace
