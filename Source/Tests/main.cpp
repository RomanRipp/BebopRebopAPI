//
//// Run all the tests that were declared with TEST()

#include <gtest/gtest.h>
#include "VideoStream.cpp"

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
