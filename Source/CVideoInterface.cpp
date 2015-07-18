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
	LOG (INFO) << "Starting video stream";

    eARSTREAM_ERROR err = ARSTREAM_OK;

    CVideoSettings videoSettings;

    m_VideoStream = new CVideoStream();
    m_VideoStream->m_videoFrameSize = videoSettings.AR_STREAM_FRAG_SIZE * videoSettings.AR_STREAM_FRAG_NB;
    m_VideoStream->m_videoFrame = (uint8_t*) malloc (m_VideoStream->m_videoFrameSize);
    if (!m_VideoStream->m_videoFrame)
    {
    	LOG(ERROR) << "Failed to create video frame";
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

	LOG(INFO) << "Video stream started";
    return true;
}

bool CVideoInterface::StopVideo(const CNetworkInterface& network)
{
	LOG (INFO) << "Stopping video stream";

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

	LOG (INFO) << "Video stream stopped";
	return true;
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

    //LOG(INFO) << "Frame received";

    switch(cause)
    {
        case ARSTREAM_READER_CAUSE_FRAME_COMPLETE:
            ret = videoStream->m_videoFrame;
            *newBufferCapacity = videoStream->m_videoFrameSize;

            if (frame)
            {
            	util::SpinLock lock(videoStream->m_lock);
            	commands::bebop::video::TRawFrame tFrame(frame, frameSize, isFlushFrame, numberOfSkippedFrames);

            	if (tFrame.IsFlushFrame())
            	{
            		LOG(INFO) << "Flush frame";
            		videoStream->m_rawFrames.clear();
            	}

				videoStream->m_rawFrames.push_back(tFrame);
            	if (videoStream->m_rawFrames.size() > CVideoSettings::VIDEO_CONTAINER_SIZE)
            	{
            		videoStream->m_rawFrames.pop_front();
            	}
            }

            // Test wtite video to a file
            // if (videoStream->m_videoFile)
            // {
            //		LOG( INFO ) << "File is ok";
            // 		fwrite(frame, frameSize, 1, videoStream->m_videoFile);
            // 		fflush(videoStream->m_videoFile);
            // }
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

bool CVideoInterface::HasFrame() const
{
	util::SpinLock lock(m_VideoStream->m_lock);
	return (m_VideoStream->m_rawFrames.size() > 0);
}

commands::bebop::video::TRawFrame CVideoInterface::GetRawFrame() const
{
	const auto& rawFrame(m_VideoStream->m_rawFrames.back());
	if (!rawFrame.IsValid())
	{
		LOG( ERROR ) << "Decoding failed";
		return commands::bebop::video::TRawFrame();
	}

	m_VideoStream->m_rawFrames.pop_back();
	return rawFrame;
}

commands::bebop::video::TDecodedFrame CVideoInterface::GetDecodedFrame(commands::bebop::video::EncodingType encoding) const
{
	auto decodedFrame = commands::bebop::video::TDecodedFrame();
	switch(encoding)
	{
	case commands::bebop::video::EncodingType::YVU:
		decodedFrame = GetYUVFrame();
		break;
	case commands::bebop::video::EncodingType::RGB:
		decodedFrame = GetRGBFrame();
		break;
	default:
		LOG( ERROR ) << "Invalid decoding format";
	}

	return decodedFrame;
}

commands::bebop::video::TDecodedFrame CVideoInterface::GetYUVFrame() const
{
	util::SpinLock lock(m_VideoStream->m_lock);
	if (m_VideoStream->m_rawFrames.size() < 1)
	{
		LOG( ERROR ) << "No Frames Available!";
		return commands::bebop::video::TDecodedFrame();
	}

	const auto& rawFrame(m_VideoStream->m_rawFrames.back());
	const auto& decodedFrame(m_videoDecoder.DecodeFrame(rawFrame));

	if (!decodedFrame.IsValid())
	{
		LOG( ERROR ) << "Decoding failed";
		return commands::bebop::video::TDecodedFrame();
	}

	m_VideoStream->m_rawFrames.pop_back();
	return decodedFrame;
}

commands::bebop::video::TDecodedFrame CVideoInterface::GetRGBFrame() const
{
	return m_videoDecoder.YVUtoRGB(GetYUVFrame());
}


} /* namespace rebop */

