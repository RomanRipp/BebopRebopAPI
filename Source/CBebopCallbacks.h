/*
 * CBebopCallbacks.h
 *
 *  Created on: May 30, 2015
 *      Author: romanripp
 */

#ifndef CBEBOPCALLBACKS_H_
#define CBEBOPCALLBACKS_H_

#include "CBebopState.h"

namespace rebop {

class CBebopCallbacks
{
public:
	CBebopCallbacks();
	virtual ~CBebopCallbacks();

	bool Init();
	static void BatteryStateChangedCallback (uint8_t percent, void *custom);
	static void FlyingStateChangedCallback (eARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE state, void *custom);
	static void NumberOfSatelitesChangedCallback(uint8_t numberOfSatelites, void *custom);

	TBebopStateCantainer GetBebopState() const;

private:
	CBebopState* m_bebopState;

};

} /* namespace rebop */

#endif /* CBEBOPCALLBACKS_H_ */
