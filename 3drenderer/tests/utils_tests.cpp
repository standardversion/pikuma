#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../utils.h"

namespace utils_tests
{

	TEST(utils, split)
	{
		std::string line{ "banana,apple,mango,kiwi" };
		std::vector<std::string> parts{ utils::split(line, ",") };
		EXPECT_EQ(parts[0], std::string{ "banana" });
		EXPECT_EQ(parts[1], std::string{ "apple" });
		EXPECT_EQ(parts[2], std::string{ "mango" });
		EXPECT_EQ(parts[3], std::string{ "kiwi" });
	}

}