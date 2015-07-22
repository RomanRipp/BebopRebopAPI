/*
 * VideoStream.h
 *
 *  Created on: Jun 11, 2015
 *      Author: Roman
 */

#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include <unistd.h>
#include "../CBebopInterface.h"
#include "../Utility.h"

extern std::string BIN_PATH;

namespace rebop{
namespace unitTests{


	std::string GetTestDir()
	{
		//Terrible code goes here
		std::string testDir(BIN_PATH);
		auto separator(testDir.find_last_of("/"));
		if (std::string::npos != separator)
		{
			testDir.erase(separator, testDir.size());
		}
		separator = testDir.find_last_of("/");
		if (std::string::npos != separator)
		{
			testDir.erase(separator, testDir.size());
		}
		testDir += "/TestFiles";

		LOG(INFO) << testDir;
		return testDir;
	}

	TEST(Video, StartMPlayer)
	{
		auto testFile(GetTestDir());
		testFile += "/test_video";

		auto cmd(std::string("mplayer "));
		cmd += testFile;
		system(cmd.c_str());

		ASSERT_TRUE(true);
	}

	TEST(Video, RawStream)
	{
		CBebopInterface bebop;
		ASSERT_TRUE(bebop.Initialize());
		ASSERT_TRUE(bebop.IsConnected());
		ASSERT_TRUE(bebop.StartVideo());
		const auto& videoInterface(bebop.GetVideo());

		LOG(INFO) << "Create file";
		auto file = fopen("./video_fifo", "w");

		bool opened(false);
		int count(0);
		while(count < 500)
		{
			LOG(INFO) << "Check frame";
			if (videoInterface.HasFrame())
			{
				LOG(INFO) << "write frame";
				auto rawFrame(videoInterface.GetRawFrame());
				fwrite(rawFrame.GetRawData(), rawFrame.GetRawFrameDataSize(), 1, file);
				fflush (file);

				if (!opened)
				{
					//system("mplayer BebopRebopAPI/Build/video_fifo");
					opened = true;
				}

				count++;
			}
			usleep(50*1000);
		}

		fclose(file);

		ASSERT_TRUE(bebop.StopVideo());
		bebop.Cleanup();
		ASSERT_TRUE(true);
	}

	TEST(Video, DecodeStream)
	{
		CVideoDecoder decoder;
		LOG(INFO) << "Initialize decoder";
		ASSERT_TRUE(decoder.Init());

		LOG(INFO) << "Get test video file";
		auto testDir(GetTestDir());
		auto testFile(testDir);
		testFile += "/test_video";
		LOG(INFO) << testFile;

		std::vector<TRawFrame> frames;
		std::vector<TDecodedFrame> decodedFrames;
		ASSERT_TRUE(util::ReadFramesFromFile(testFile, frames));
		ASSERT_FALSE(frames.empty());

		testFile = testDir;
		testFile += "/test_video2";
		ASSERT_TRUE(util::WriteFramesToFile(testFile, frames));
		auto cmd(std::string("mplayer "));
		cmd += testFile;
		system(cmd.c_str());

//		for (auto frame : frames)
//		{
//			auto decodedFrame(decoder.DecodeFrame(frame));
//			decodedFrames.push_back(decodedFrame);
//		}

		//ASSERT_FALSE(decodedFrames.empty());
		ASSERT_TRUE(true);
	}


}
}


