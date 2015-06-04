/*
 * CBebopState.cpp
 *
 *  Created on: May 30, 2015
 *      Author: romanripp
 */

#include "CBebopState.h"

namespace rebop {

CBebopState::CBebopState()
 : m_stateContainer(),
   m_lock(false)
{
	m_stateContainer.m_isValid = true;
}

CBebopState::~CBebopState()
{
	// TODO Auto-generated destructor stub
}

void CBebopState::SetFlyingState(FlyingState state)
{
	util::SpinLock lock(m_lock);
	m_stateContainer.m_flyingState = state;
}

void CBebopState::SetBatteryCapacity(int capacity)
{
	util::SpinLock lock(m_lock);
	m_stateContainer.m_batteryCharge = capacity;
}

void CBebopState::SetNumberOfSattelites(int count)
{
	util::SpinLock lock(m_lock);
	m_stateContainer.m_numberOfSatelites = count;
}

const TBebopStateCantainer CBebopState::GetState() const
{
	util::SpinLock lock(m_lock);
	return m_stateContainer;
}

} /* namespace rebop */
