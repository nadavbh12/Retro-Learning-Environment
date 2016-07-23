/*
 * RleTest.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: nadav
 */

#include "../src/environment/Serializer.hxx"
#include "../src/environment/Deserializer.hxx"
#include <string>
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class SerializeDeserializeTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	SerializeDeserializeTest() {
    // You can do set-up work for each test here.
  }

  virtual ~SerializeDeserializeTest() {
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

// Tests that the Foo::Bar() method does Abc.
TEST_F(SerializeDeserializeTest, onetring) {
	Serializer ser;
	std::string aString = "test";
	ser.putString(aString);

	Deserializer des(ser.get_str());
	EXPECT_EQ(aString, des.getString());
}

TEST_F(SerializeDeserializeTest, oneInt) {
	Serializer ser;
	int x = 4;
	ser.putInt(x);

	Deserializer des(ser.get_str());
	EXPECT_EQ(x, des.getInt());
}

TEST_F(SerializeDeserializeTest, oneBoolTrue) {
	Serializer ser;
	bool b = true;
	ser.putBool(b);

	Deserializer des(ser.get_str());
	EXPECT_EQ(b, des.getBool());
}

TEST_F(SerializeDeserializeTest, oneBoolFalse) {
	Serializer ser;
	bool b = false;
	ser.putBool(b);

	Deserializer des(ser.get_str());
	EXPECT_EQ(b, des.getBool());
}

TEST_F(SerializeDeserializeTest, oneIntArray) {
	Serializer ser;
	int intArray[] = {4, 5, 7};
	size_t size = 3;
	ser.putIntArray(intArray, size);

	Deserializer des(ser.get_str());
	int newArray[3];
	des.getIntArray(newArray, size);
	EXPECT_EQ(intArray[0], newArray[0]);
	EXPECT_EQ(intArray[1], newArray[1]);
	EXPECT_EQ(intArray[2], newArray[2]);
}

TEST_F(SerializeDeserializeTest, twpIntArray) {
	Serializer ser;
	int intArray1[] = {4, 5, 7};
	int intArray2[] = {4, 5, 7, 99, 3, 54, 234556};
	size_t size1 = 3;
	size_t size2 = 7;
	ser.putIntArray(intArray1, size1);
	ser.putIntArray(intArray2, size2);

	Deserializer des(ser.get_str());
	int newArray1[size1];
	des.getIntArray(newArray1, size1);
	EXPECT_EQ(intArray1[0], newArray1[0]);
	EXPECT_EQ(intArray1[1], newArray1[1]);
	EXPECT_EQ(intArray1[2], newArray1[2]);

	int newArray2[size2];
	des.getIntArray(newArray2, size2);
	EXPECT_EQ(intArray2[0], newArray2[0]);
	EXPECT_EQ(intArray2[1], newArray2[1]);
	EXPECT_EQ(intArray2[2], newArray2[2]);
	EXPECT_EQ(intArray2[3], newArray2[3]);
	EXPECT_EQ(intArray2[4], newArray2[4]);
	EXPECT_EQ(intArray2[5], newArray2[5]);
	EXPECT_EQ(intArray2[6], newArray2[6]);
}

TEST_F(SerializeDeserializeTest, twoInt) {
	Serializer ser;
	int x = 4;
	int y = 0xfffffff;
	ser.putInt(x);
	ser.putInt(y);

	Deserializer des(ser.get_str());
	EXPECT_EQ(x, des.getInt());
	EXPECT_EQ(y, des.getInt());
}

TEST_F(SerializeDeserializeTest, twoStrings) {
	Serializer ser;
	std::string aString = "test";
	std::string aSecondString = "another test";
	ser.putString(aString);
	ser.putString(aSecondString);

	Deserializer des(ser.get_str());
	EXPECT_EQ(aString, des.getString());
	EXPECT_EQ(aSecondString, des.getString());
}

TEST_F(SerializeDeserializeTest, intStringBool) {
	Serializer ser;
	std::string aString = "test";
	int x = 9;
	bool b = true;
	ser.putString(aString);
	ser.putInt(x);
	ser.putBool(b);

	Deserializer des(ser.get_str());
	EXPECT_EQ(aString, des.getString());
	EXPECT_EQ(x, des.getInt());
	EXPECT_EQ(b, des.getBool());
}

}
