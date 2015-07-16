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
		virtual commands::bebop::video::TDecodedFrame GetDecodedFrame(commands::bebop::video::EncodingType ecnoding) const = 0;
		virtual commands::bebop::video::TDecodedFrame GetYUVFrame() const = 0;
		virtual commands::bebop::video::TDecodedFrame GetRGBFrame() const = 0;
	};
}

#endif /* IVIDEOINTERFACE_H_ */
