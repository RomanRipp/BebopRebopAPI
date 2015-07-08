/*
 * IVideoInterface.h
 *
 *  Created on: Jun 5, 2015
 *      Author: romanripp
 */

#ifndef IVIDEOINTERFACE_H_
#define IVIDEOINTERFACE_H_

#include "BebopCommandsAndData.h"

namespace rebop {

	class IVideoInterface
	{
	public:
		virtual bool HasFrame() const = 0;
		virtual commands::bebop::video::TDecodedFrame GetDecodedFrame() = 0;
		virtual commands::bebop::video::TRawFrame GetYUVFrame() const = 0;
		virtual commands::bebop::video::TRawFrame GetRGBFrame() const = 0;
	};
}

#endif /* IVIDEOINTERFACE_H_ */
