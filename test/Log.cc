#include <regex>

#include "gtest/gtest.h"
#include "constants.hpp"
#include "Log.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;
using namespace Test;

TEST(Log, constructor){

	ASSERT_NO_THROW(Log log("constructor"));

}

TEST(Log, output){
	testing::internal::CaptureStdout();

	Log log("test");

	log.log("Single");

	string output = testing::internal::GetCapturedStdout();

	EXPECT_TRUE(regex_match(output, regex("\\[.+\\]\\[Log\\]\\[test\\] Single\n")));

}
