/*
 * Tests/Utility.h
 *
 *  Created on: Jun 11, 2015
 *      Author: Roman
 */

#pragma once

namespace rebop{
namespace unitTests{

TEST(Utilities, ReadWriteVideo)
{
	std::vector<TRawFrame> frames;
	ASSERT_TRUE(util::ReadFramesFromFile("../TestFiles/test_video", frames));

	// Write Files
	ASSERT_TRUE(util::WriteFramesToFile("../TestFiles/test_video1", frames));

	system("mplayer ../TestFiles/test_video1");
}

}
}
