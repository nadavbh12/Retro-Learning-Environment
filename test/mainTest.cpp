#include "gtest/gtest.h"
#include "gtest_arguments.h"

std::string gtest_arguments::romName = "";
std::string gtest_arguments::coreName = "";

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	gtest_arguments::romName = argv[1];
	gtest_arguments::coreName = argv[2];
	int ret = RUN_ALL_TESTS();
	return ret;
}
