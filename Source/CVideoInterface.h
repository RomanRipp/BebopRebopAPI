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

#include "BebopCommandsAndData.h"

#include <list>
#include <atomic>

namespace rebop {

class CVideoInterface {
public:
	CVideoInterface();
	CVideoInterface(ARNETWORK_Manager_t* netManager);
	virtual ~CVideoInterface();

	virtual bool StartVideo(const CNetworkInterface& network);
	virtual bool StopVideo(const CNetworkInterface& network);

	virtual commands::bebop::video::TFrame GetFrame() const;

private:
	static uint8_t* FrameCompleteCallback (eARSTREAM_READER_CAUSE cause, uint8_t *frame, uint32_t frameSize, int numberOfSkippedFrames, int isFlushFrame, uint32_t *newBufferCapacity, void *custom);

	ARSAL_Thread_t m_videoRxThread;
	ARSAL_Thread_t m_videoTxThread;
	ARSTREAM_Reader_t *m_streamReader;

	class CVideoStream
	{
	public:
		std::atomic<bool> m_lock;

		uint32_t m_videoFrameSize;
		uint8_t *m_videoFrame;
		FILE *m_videoFile;
		std::list<commands::bebop::video::TFrame> m_videoFrames;
	}* m_VideoStream;
};

} /* namespace rebop */
