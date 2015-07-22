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
	// Register codecs
	avcodec_register_all();

	// This did not work for some reason
	//av_log_set_level(AV_LOG_QUIET);

	// get h264 decoder
	m_ffmpegDecoder.m_codec = *avcodec_find_decoder(AV_CODEC_ID_H264);

	// allocate context
	m_ffmpegDecoder.m_codecCtx = *avcodec_alloc_context3(&m_ffmpegDecoder.m_codec);

	// Initialize condec context
	m_ffmpegDecoder.m_codecCtx.pix_fmt = PIX_FMT_YUV420P;
	m_ffmpegDecoder.m_codecCtx.skip_frame = AVDISCARD_DEFAULT;
	m_ffmpegDecoder.m_codecCtx.error_concealment = FF_EC_GUESS_MVS | FF_EC_DEBLOCK;
	m_ffmpegDecoder.m_codecCtx.skip_loop_filter = AVDISCARD_DEFAULT;
	m_ffmpegDecoder.m_codecCtx.workaround_bugs = FF_BUG_AUTODETECT;
	m_ffmpegDecoder.m_codecCtx.codec_type = AVMEDIA_TYPE_VIDEO;
	m_ffmpegDecoder.m_codecCtx.codec_id = AV_CODEC_ID_H264;
	m_ffmpegDecoder.m_codecCtx.skip_idct = AVDISCARD_DEFAULT;

	if (avcodec_open2(&m_ffmpegDecoder.m_codecCtx, &m_ffmpegDecoder.m_codec, nullptr) != 0)
	{
		LOG(ERROR) << "Failed to open codec";
		return false;
	}

	//Allocate frame
	//Deprecated
	m_ffmpegDecoder.m_decodedFrame = *avcodec_alloc_frame();

	// Initialize packet
	av_init_packet(&m_ffmpegDecoder.m_avpkt);

	return true;
}

TDecodedFrame CVideoDecoder::DecodeFrame(const TRawFrame& rawFrame) const
{

	if (!rawFrame.IsValid())
	{
		LOG(ERROR) << "failed to decode frame";
		return TDecodedFrame();
	}

	return FFMPEGDecodeFrame(rawFrame);
}

TDecodedFrame CVideoDecoder::YVUtoRGB(const TDecodedFrame& yvuframe) const
{

	if (!yvuframe.IsValid())
	{
		return TDecodedFrame();
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

	return TDecodedFrame();
}

TDecodedFrame CVideoDecoder::FFMPEGDecodeFrame(const TRawFrame& rawFrame) const
{
	/* -- Decode one frame with FFMPEG -- */
	int frameFinished(0);
	int length(0);
	TDecodedFrame decodedFrame;

	m_ffmpegDecoder.m_avpkt.data = rawFrame.GetRawData();
	m_ffmpegDecoder.m_avpkt.size = rawFrame.GetRawFrameDataSize();

	while (m_ffmpegDecoder.m_avpkt.size > 0)
	{
		length = avcodec_decode_video2(&m_ffmpegDecoder.m_codecCtx,
				&m_ffmpegDecoder.m_decodedFrame,
				&frameFinished,
				&m_ffmpegDecoder.m_avpkt);

		if (length > 0)
		{
			if (frameFinished)
			{
				std::vector<FrameConponent> components;

				//Y component
				components.push_back(
				{
//					m_ffmpegDecoder.m_decodedFrame->data[0],
//					m_ffmpegDecoder.m_decodedFrame->linesize[0],
//					m_ffmpegDecoder.m_decodedFrame->linesize[0] * m_ffmpegDecoder.m_decodedFrame->height
				});

				//U component
				components.push_back(
				{
//					m_ffmpegDecoder.m_decodedFrame->data[1],
//					m_ffmpegDecoder.m_decodedFrame->linesize[1],
//					(m_ffmpegDecoder.m_decodedFrame->linesize[1] * (m_ffmpegDecoder.m_decodedFrame->height / 2))
				});

				//V component
				components.push_back(
				{
//					m_ffmpegDecoder.m_decodedFrame->data[2],
//					m_ffmpegDecoder.m_decodedFrame->linesize[2],
//					(m_ffmpegDecoder.m_decodedFrame->linesize[2] * (m_ffmpegDecoder.m_decodedFrame->height / 2))
				});

				decodedFrame = TDecodedFrame(rawFrame,
						commands::bebop::video::EncodingType::YVU,
						m_ffmpegDecoder.m_decodedFrame.width,
						m_ffmpegDecoder.m_decodedFrame.height,
						components);
			}

			m_ffmpegDecoder.m_avpkt.size -= length;
			m_ffmpegDecoder.m_avpkt.data += length;
		}
	}

	return decodedFrame;
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
