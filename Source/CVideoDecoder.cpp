/*
 * CVideoDecoder.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: romanripp
 */

extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include "CVideoDecoder.h"
#include "Utility.h"

namespace rebop {

CVideoDecoder::CVideoDecoder() {
	// TODO Auto-generated constructor stub

}

CVideoDecoder::~CVideoDecoder() {
	// TODO Auto-generated destructor stub
}

bool CVideoDecoder::Init()
{
	return true;
}

TDecodedFrame CVideoDecoder::DecodeFrame(const TRawFrame& rawFrame)
{

	if (!rawFrame.IsValid())
	{
		LOG(ERROR) << "failed to decode frame";
		return TDecodedFrame();
	}

	//TDecodedFrame decodedFrame;

	//ARCODECS_Manager_Frame_t *outputFrame = NULL;
	//int ret;
	//eARCODECS_ERROR err = ARCODECS_OK;

//	if(manager == NULL)
//	{
//		err = ARCODECS_ERROR_BAD_PARAMETER;
//	}

//	if(err == ARCODECS_OK)
//	{
		//ARCODECS_Manager_FFMPEGDecoder_t *ffmpegDecoder = (ARCODECS_Manager_FFMPEGDecoder_t *)(manager->decoder);
		//eARCODECS_ERROR error = ARCODECS_OK;
		//uint8_t *data = NULL;
		//int size = 0;

		/* callback to get the frame to decode */
		//size = manager->callback((&data), manager->callbackCustomData);

		//outputFrame = &manager->outputFrame;
		//error = ARCODECS_Manager_FFMPEGDecode (ffmpegDecoder, data, size, outputFrame);
		//auto decodedFrame = FFMPEGDecodeFrame(rawFrame);
//	}

//	/* return the error */
//	if (error != NULL)
//	{
//		*error = err;
//	}

	return FFMPEGDecodeFrame(rawFrame);
}

TRawFrame CVideoDecoder::YVUtoRGB(const TRawFrame& yvuframe) const
{

	if (!yvuframe.IsValid())
	{
		return TRawFrame();
	}

	int width(yvuframe.GetWidth());
	int height(yvuframe.GetHeight());
	uint8_t* yuv420(yvuframe.GetRawData());
	uint8_t* rgb8 = new uint8_t[3*width*height];
	int uvOffset(0);

	size_t pixelSize = width*height;
	//uv is stored below the luminance image: add offset
	size_t offset = pixelSize + uvOffset;

	for(size_t y=0, uv=0; y < pixelSize; y+=2, uv+=2) {

	  int v = yuv420[offset + uv  ] & 0xff;
	  v -= 128;
	  int u = yuv420[offset + uv+1] & 0xff;
	  u -= 128;

	  for(int i : { 0, 1, width, width+1 }) {//Walk through pixels belonging to uv block
		size_t pix_rgb = 3*(y + i); //location of rgb pixel
		int luma_val = yuv420[y + i] & 0xff;
		YVUtoRGBPixel(luma_val, u, v, rgb8[pix_rgb], rgb8[pix_rgb+1], rgb8[pix_rgb+2]);
	  }

	  if (y!=0 && (y+2)%width==0){//End of line
		y += width; //Skip one line in input b/c uv blocks are 2x2
	  }
	}

	return TRawFrame(rgb8,
			yvuframe.GetRawFrameDataSize(),
			yvuframe.GetNumberOfSkippedFrames(),
			yvuframe.IsFlushFrame());
}

TDecodedFrame CVideoDecoder::FFMPEGDecodeFrame(const TRawFrame& rawFrame)
{
//   /* -- Decode one frame with FFMPEG -- */
//	int frameFinished = 0;
//	int len = 0;
//
//	/* set the frame to decode */
//	m_ffmpegDecoder.m_avpkt.data = rawFrame.GetRawData();
//	m_ffmpegDecoder.m_avpkt.size = rawFrame.GetRawFrameDataSize();
//
//	/* while there are some data to decoding */
//	while (m_ffmpegDecoder.m_avpkt.size > 0)
//	{
//		/* decoding */
//		len = avcodec_decode_video2 (&m_ffmpegDecoder.m_codecCtx, &m_ffmpegDecoder.m_decodedFrame, &frameFinished, &m_ffmpegDecoder.m_avpkt);
//		if (len > 0)
//		{
//			/* if the frame is finished*/
//			if (frameFinished)
//			{
//				std::vector<FrameConponent> components;
//				{
//					FrameConponent component =
//					{
//							m_ffmpegDecoder.m_decodedFrame.data[0],
//							(uint32_t) m_ffmpegDecoder.m_decodedFrame.linesize[0],
//							(uint32_t) (m_ffmpegDecoder.m_decodedFrame.linesize[0] * m_ffmpegDecoder.m_decodedFrame.height)
//					};
//					components.push_back(component);
//				}
//				{
//					FrameConponent component =
//					{
//							m_ffmpegDecoder.m_decodedFrame.data[1],
//							(uint32_t) m_ffmpegDecoder.m_decodedFrame.linesize[1],
//							(uint32_t) (m_ffmpegDecoder.m_decodedFrame.linesize[1] * m_ffmpegDecoder.m_decodedFrame.height)
//					};
//					components.push_back(component);
//				}
//				{
//					FrameConponent component =
//					{
//							m_ffmpegDecoder.m_decodedFrame.data[2],
//							(uint32_t) m_ffmpegDecoder.m_decodedFrame.linesize[2],
//							(uint32_t) (m_ffmpegDecoder.m_decodedFrame.linesize[2] * m_ffmpegDecoder.m_decodedFrame.height)
//					};
//					components.push_back(component);
//				}
//
//				TDecodedFrame decodedFrame(rawFrame,
//					EncodingType::YVU,
//					m_ffmpegDecoder.m_decodedFrame.width,
//					m_ffmpegDecoder.m_decodedFrame.height,
//					components);
//			}
//		}
//
//		/* progress the data of the size of the data decoded */
//		if (m_ffmpegDecoder.m_avpkt.data)
//		{
//			m_ffmpegDecoder.m_avpkt.size -= len;
//			m_ffmpegDecoder.m_avpkt.data += len;
//		}
//	}
	return TDecodedFrame();
}

void CVideoDecoder::YVUtoRGBPixel(int y, int u, int v, uint8_t& r, uint8_t& g, uint8_t& b) const
{
	int tmp_r = y + (int)(1.772f*v);
    int tmp_g = y - (int)(0.344f*v + 0.714f*u);
    int tmp_b = y + (int)(1.402f*u);
    r = tmp_r>255? 255 : tmp_r<0 ? 0 : tmp_r;
    g = tmp_g>255? 255 : tmp_g<0 ? 0 : tmp_g;
    b = tmp_b>255? 255 : tmp_b<0 ? 0 : tmp_b;
}

} /* namespace rebop */
