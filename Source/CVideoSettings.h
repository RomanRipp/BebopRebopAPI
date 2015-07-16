/*
 * VideoSettings.h
 *
 *  Created on: May 16, 2015
 *      Author: romanripp
 */
#pragma once

#include <libARNetwork/ARNETWORK_IOBufferParam.h>

namespace rebop
{
	class CVideoSettings
	{
	public:
		CVideoSettings() { }
		~CVideoSettings() { }

		static const int VIDEO_CONTAINER_SIZE = 5;

		static const int AR_STREAM_ACK_DELAY = 0;
		static const int AR_STREAM_FRAG_SIZE = 65000;
		static const int AR_STREAM_FRAG_NB = 4;

	};
}
