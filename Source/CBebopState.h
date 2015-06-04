/*
 * CBebopState.h
 *
 *  Created on: May 30, 2015
 *      Author: romanripp
 */

#ifndef CBEBOPSTATE_H_
#define CBEBOPSTATE_H_

#include "BebopCommandsAndData.h"
#include "Utility.h"

using namespace rebop::state::bebop;

namespace rebop {

class CBebopState {
public:
	CBebopState();
	virtual ~CBebopState();

	void SetFlyingState(FlyingState state);
	void SetBatteryCapacity(int capacity);
	void SetNumberOfSattelites(int count);

	const TBebopStateCantainer GetState() const;

private:
//	uint8_t m_batteryCapacity;
//	FlyingState m_flyingState;
	TBebopStateCantainer m_stateContainer;
	mutable std::atomic<bool> m_lock;
};

} /* namespace rebop */

#endif /* CBEBOPSTATE_H_ */
