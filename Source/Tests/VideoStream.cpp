/*
 * VideoStream.cpp
 *
 *  Created on: Jun 11, 2015
 *      Author: Roman
 */

#include <gtest/gtest.h>
#include <iostream>
#include <unistd.h>
#include "../CBebopInterface.h"

namespace rebop{
namespace unitTests{

	TEST(BebopApiUnitTests, RawVideoStreaming)
	{
		CBebopInterface bebop;
		ASSERT_TRUE(bebop.Initialize());
		ASSERT_TRUE(bebop.IsConnected());
		ASSERT_TRUE(bebop.StartVideo());
		const auto& videoInterface(bebop.GetVideo());

		LOG(INFO) << "Create file";
		auto file = fopen("./video_fifo", "w");
		int count(0);
		while(count < 500)
		{
			LOG(INFO) << "Check frame";
			if (videoInterface.HasFrame())
			{
				LOG(INFO) << "write frame";
				auto rawFrame(videoInterface.GetRGBFrame());
				fwrite(rawFrame.GetRawData(), rawFrame.GetRawFrameDataSize(), 1, file);
				fflush (file);
				count++;
			}
			usleep(70*1000);
		}

		fclose(file);
		system("/usr/local/bin/mplayer video_fifo");
		ASSERT_TRUE(bebop.StopVideo());
		bebop.Cleanup();
		ASSERT_TRUE(true);
	}

//	TEST(BebopApiUnitTests, RawVideoStreaming)
//	{
//
//		pid_t child = 0;
////		if ((child = fork()) == 0)
////		{
//			// redirect stdout and stderr of mplayer to dev/null to avoid messing with ncurse
////			auto stdout_fd = fopen("/dev/null", 0);
////			std::dup2(stdout_fd, 1);
////			fclose(stdout_fd);
//
////			auto stderr_fd = fopen("/dev/null", 0);
////			std::dup2(stderr_fd, 1);
////			fclose(stderr_fd);
//
////			execlp("mplayer", "mplayer", "./video_decoded_fifo", "-demuxer", "rawvideo", "-rawvideo", "w=640:h=368:fps=30:format=i420", ">/dev/null", "2>/dev/null", NULL);
////		}
//
//		CBebopInterface bebop;
//		ASSERT_TRUE(bebop.Initialize());
//		ASSERT_TRUE(bebop.IsConnected());
//		ASSERT_TRUE(bebop.StartVideo());
//		const auto& videoInterface(bebop.GetVideo());
//
//		auto file = fopen("./video_decoded_fifo", "w");
//		int count(0);
//		while(count < 500)
//		{
//			if (videoInterface.HasFrame())
//			{
//				auto decodedFrame(videoInterface.GetDecodedFrame());
//
//                AVFrame *avFrame = avcodec_alloc_frame();
//                if (avFrame != NULL)
//                {
//                    avFrame->width = decodedFrame->width;
//                    avFrame->height = decodedFrame->height;
//                    avFrame->format = AV_PIX_FMT_YUV420P;
//
//                    avpicture_fill((AVPicture*)avFrame, NULL, PIX_FMT_YUV420P, decodedFrame->width, decodedFrame->height);
//                    avFrame->linesize[0] = decodedFrame->componentArray[0].lineSize;
//                    avFrame->linesize[1] = decodedFrame->componentArray[1].lineSize;
//                    avFrame->linesize[2] = decodedFrame->componentArray[2].lineSize;
//
//                    avFrame->data[0] = decodedFrame->componentArray[0].data;
//                    avFrame->data[1] = decodedFrame->componentArray[1].data;
//                    avFrame->data[2] = decodedFrame->componentArray[2].data;
//
//                    avpicture_layout((AVPicture*)avFrame, PIX_FMT_YUV420P, decodedFrame->width, decodedFrame->height, decodedOut, pic_size);
//                    avcodec_free_frame(&avFrame);
//                }
//
//				fwrite(decodedFrame.GetRawData(), decodedFrame.GetRawFrameDataSize(), 1, file);
//				fflush (file);
//				count++;
//			}
//		}
//
//		fclose(file);
//		system("/usr/local/bin/mplayer ./video_decoded_fifo -demuxer rawvideo -rawvideo w=640:h=368:fps=30:format=i420");
//		ASSERT_TRUE(bebop.StopVideo());
//		bebop.Cleanup();
//		ASSERT_TRUE(true);
//	}


}
}


