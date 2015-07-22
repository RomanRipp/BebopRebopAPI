//
//// Run all the tests that were declared with TEST()

#include <gtest/gtest.h>
#include "VideoStream.h"
#include "Utility.h"

std::string BIN_PATH;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	BIN_PATH = std::string(argv[0]);
	return RUN_ALL_TESTS();
}
