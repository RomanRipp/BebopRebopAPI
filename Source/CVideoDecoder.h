/*
 * CVideoDecoder.h
 *
 *  Created on: Jun 4, 2015
 *      Author: romanripp
 */

#ifndef CVIDEODECODER_H_
#define CVIDEODECODER_H_

#include "BebopCommandsAndData.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

using namespace rebop::commands::bebop::video;

namespace rebop {

class CVideoDecoder {
public:
	CVideoDecoder();
	virtual ~CVideoDecoder();

	virtual bool Init();
	virtual TDecodedFrame DecodeFrame(const TRawFrame& rawFrame);
	virtual TRawFrame YVUtoRGB(const TRawFrame& yvuframe) const;

private:
	virtual TDecodedFrame FFMPEGDecodeFrame(const TRawFrame& rawFrame);
	void YVUtoRGBPixel(int y, int u, int v, uint8_t& r, uint8_t& g, uint8_t& b) const;

	struct FFMPEGDecoder
	{
		AVCodec m_codec;
		AVCodecContext m_codecCtx;
		AVFrame m_decodedFrame;
		AVPacket m_avpkt;
		uint8_t m_outputData;
		int m_outputDataSize;
	} m_ffmpegDecoder;

};

} /* namespace rebop */

#endif /* CVIDEODECODER_H_ */
