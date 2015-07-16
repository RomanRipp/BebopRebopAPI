/*
 * CVideoInterface.h
 *
 *  Created on: May 16, 2015
 *      Author: romanripp
 */
#pragma once

extern "C"
{
#include <cstdio>
#include <stdlib.h>
#include <libARSAL/ARSAL.h>
#include <libARNetwork/ARNetwork.h>
#include <libARStream/ARStream.h>
}
#include <list>
#include <atomic>

#include "IVideoInterface.h"
#include "CVideoDecoder.h"


namespace rebop {

class CVideoInterface : public IVideoInterface
{
public:
	CVideoInterface();
	CVideoInterface(ARNETWORK_Manager_t* netManager);
	virtual ~CVideoInterface();

	virtual bool StartVideo(const CNetworkInterface& network);
	virtual bool StopVideo(const CNetworkInterface& network);

	virtual bool HasFrame() const override;
	virtual commands::bebop::video::TDecodedFrame GetDecodedFrame(commands::bebop::video::EncodingType ecnoding) const override;
	virtual commands::bebop::video::TDecodedFrame GetYUVFrame() const override;
	virtual commands::bebop::video::TDecodedFrame GetRGBFrame() const override;

private:
	static uint8_t* FrameCompleteCallback (eARSTREAM_READER_CAUSE cause, uint8_t *frame, uint32_t frameSize, int numberOfSkippedFrames, int isFlushFrame, uint32_t *newBufferCapacity, void *custom);

	ARSAL_Thread_t m_videoRxThread;
	ARSAL_Thread_t m_videoTxThread;
	ARSTREAM_Reader_t *m_streamReader;
	CVideoDecoder m_videoDecoder;

	class CVideoStream
	{
	public:
		std::atomic<bool> m_lock;

		uint32_t m_videoFrameSize;
		uint8_t *m_videoFrame;
		FILE *m_videoFile;
		mutable std::list<commands::bebop::video::TRawFrame> m_rawFrames;
		mutable std::list<commands::bebop::video::TDecodedFrame> m_decodedFrames;
	}* m_VideoStream;
};

} /* namespace rebop */
