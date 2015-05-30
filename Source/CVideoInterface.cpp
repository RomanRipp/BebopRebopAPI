/*
 * CVideoInterface.cpp
 *
 *  Created on: May 16, 2015
 *      Author: romanripp
 */

#include "CVideoSettings.h"
#include "CNetworkInterface.h"
#include "CVideoInterface.h"

#include "Utility.h"

namespace rebop {

CVideoInterface::CVideoInterface() : m_VideoStream(new CVideoStream())
{
	// TODO Auto-generated constructor stub
	m_streamReader = nullptr;
	m_videoRxThread = nullptr;
	m_videoTxThread = nullptr;
}

CVideoInterface::~CVideoInterface() {
	// TODO Auto-generated destructor stub
}

bool CVideoInterface::StartVideo(const CNetworkInterface& network)
{
    eARSTREAM_ERROR err = ARSTREAM_OK;

    CVideoSettings videoSettings;

    m_VideoStream = new CVideoStream();
    m_VideoStream->m_videoFrameSize = videoSettings.AR_STREAM_FRAG_SIZE * videoSettings.AR_STREAM_FRAG_NB;
    m_VideoStream->m_videoFrame = (uint8_t*) malloc (m_VideoStream->m_videoFrameSize);
    if (!m_VideoStream->m_videoFrame)
    {
    	return false;
    }

    m_streamReader = ARSTREAM_Reader_New (network.m_pNetworkManager
    		, (int) EInboundBufferId::INBOUND_VIDEO
    		, (int) EOutboundBufferId::OUTBOUND_VIDEO_ACK
    		, CVideoInterface::FrameCompleteCallback
    		, m_VideoStream->m_videoFrame
    		, m_VideoStream->m_videoFrameSize
    		, videoSettings.AR_STREAM_FRAG_SIZE
    		, videoSettings.AR_STREAM_ACK_DELAY
    		, m_VideoStream
    		, &err);

    if (err != ARSTREAM_OK)
    {
        LOG( ERROR ) << "Error during ARStream creation : " << ARSTREAM_Error_ToString(err);
        return false;
    }

	// Create and start Tx and Rx threads.
	if (ARSAL_Thread_Create(&(m_videoRxThread), ARSTREAM_Reader_RunDataThread, m_streamReader) != 0)
	{
		LOG( ERROR ) << "Creation of video Rx thread failed.";
		return false;
	}

	if (ARSAL_Thread_Create(&(m_videoRxThread), ARSTREAM_Reader_RunAckThread, m_streamReader) != 0)
	{
		LOG( ERROR ) << "Creation of video Tx thread failed.";
		return false;
	}

	m_VideoStream->m_videoFile = fopen("./video_fifo", "w");
	if (!m_VideoStream->m_videoFile)
	{
		LOG (ERROR) << "Video file is null";
		return false;
	}

    return true;
}

bool CVideoInterface::StopVideo(const CNetworkInterface& network)
{
	if (m_streamReader)
	{
		ARSTREAM_Reader_StopReader(m_streamReader);

		// Optionnal, but better for speed:
		ARNETWORKAL_Manager_Unlock(network.m_pNetworkALManager);

		if (m_videoRxThread != NULL)
		{
			ARSAL_Thread_Join(m_videoRxThread, NULL);
			ARSAL_Thread_Destroy(&(m_videoRxThread));
			m_videoRxThread = nullptr;
		}
		if (m_videoTxThread != NULL)
		{
			ARSAL_Thread_Join(m_videoTxThread, NULL);
			ARSAL_Thread_Destroy(&(m_videoTxThread));
			m_videoTxThread = NULL;
		}

		ARSTREAM_Reader_Delete (&(m_streamReader));
	}

	if (m_VideoStream->m_videoFrame)
	{
		delete m_VideoStream->m_videoFrame;
		m_VideoStream->m_videoFrame = NULL;
	}
	return false;
}

uint8_t* CVideoInterface::FrameCompleteCallback (eARSTREAM_READER_CAUSE cause
		, uint8_t *frame
		, uint32_t frameSize
		, int numberOfSkippedFrames
		, int isFlushFrame
		, uint32_t *newBufferCapacity
		, void *custom)
{
    uint8_t *ret = nullptr;
    CVideoStream *videoStream = (CVideoStream *)custom;

    switch(cause)
    {
        case ARSTREAM_READER_CAUSE_FRAME_COMPLETE:
            /* Here, the mjpeg video frame is in the "frame" pointer, with size "frameSize" bytes
             You can do what you want, but keep it as short as possible, as the video is blocked until you return from this callback.
             Typically, you will either copy the frame and return the same buffer to the library, or store the buffer
             in a fifo for pending operations, and provide a new one.
             In this sample, we do nothing and just pass the buffer back*/

            ret = videoStream->m_videoFrame;
            *newBufferCapacity = videoStream->m_videoFrameSize;

            if (frame)
            {
            	LOG( INFO ) << "Video frame received";
            	commands::bebop::video::TFrame tFrame(frame, frameSize, isFlushFrame, numberOfSkippedFrames);

				videoStream->m_videoFrames.push_back(tFrame);

            	if (videoStream->m_videoFrames.size() > CVideoSettings::VIDEO_CONTAINER_SIZE)
            	{
            		videoStream->m_videoFrames.pop_front();
            	}
            }

            /* Again, don't write files in this thread, that is just for the example :) */
//            if (videoStream->m_videoFile)
//            {
//            	LOG( INFO ) << "File is ok";
//            	fwrite(frame, frameSize, 1, videoStream->m_videoFile);
//            	fflush(videoStream->m_videoFile);
//            }
            break;
        case ARSTREAM_READER_CAUSE_FRAME_TOO_SMALL:
            /* This case should not happen, as we've allocated a frame pointer to the maximum possible size. */
            ret = videoStream->m_videoFrame;
            *newBufferCapacity = videoStream->m_videoFrameSize;
            break;
        case ARSTREAM_READER_CAUSE_COPY_COMPLETE:
            /* Same as before ... but return value are ignored, so we just do nothing */
            break;
        case ARSTREAM_READER_CAUSE_CANCEL:
            /* Called when the library closes, return values ignored, so do nothing here */
            break;
        default:
            break;
    }

    return ret;
}

commands::bebop::video::TFrame CVideoInterface::GetFrame() const
{

	if (m_VideoStream->m_videoFrames.size() > 0)
	{
		LOG( INFO ) << "Frame available";
		return m_VideoStream->m_videoFrames.back();
	}

	LOG( ERROR ) << "No Frames Available!";
	return commands::bebop::video::TFrame();
}

} /* namespace rebop */

