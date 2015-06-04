/*
 * CBebopCallbacks.cpp
 *
 *  Created on: May 30, 2015
 *      Author: romanripp
 */

extern "C"
{
#include "libARCommands/ARCOMMANDS_Types.h"
#include "libARCommands/ARCOMMANDS_Decoder.h"
}

#include "CBebopCallbacks.h"
#include "Utility.h"

namespace rebop {

CBebopCallbacks::CBebopCallbacks()
{
	m_bebopState = new CBebopState();
}

bool CBebopCallbacks::Init()
{
	LOG(INFO) << "Registering callbacks";

	if (!m_bebopState)
	{
		LOG(ERROR) << "Bebop state object is invalid";
		return false;
	}

	//Awesome callbacks go here
	ARCOMMANDS_Decoder_SetCommonCommonStateBatteryStateChangedCallback(CBebopCallbacks::BatteryStateChangedCallback, m_bebopState);
	ARCOMMANDS_Decoder_SetARDrone3PilotingStateFlyingStateChangedCallback(CBebopCallbacks::FlyingStateChangedCallback, m_bebopState);
	ARCOMMANDS_Decoder_SetARDrone3DebugGPSDebugStateNbSatelliteChangedCallback(CBebopCallbacks::NumberOfSatelitesChangedCallback, m_bebopState);

	return true;
}

CBebopCallbacks::~CBebopCallbacks()
{
	delete m_bebopState;
}

void CBebopCallbacks::BatteryStateChangedCallback (uint8_t percent, void *custom)
{
	LOG(INFO) << "Battery state changed: " << (int) percent << "%";
	CBebopState *bebopState = (CBebopState*)custom;
	if (!bebopState)
	{
		LOG(ERROR) << "bebop state object is invalid";
		return;
	}

	bebopState->SetBatteryCapacity(percent);
}

void CBebopCallbacks::FlyingStateChangedCallback (eARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE state, void *custom)
{
	LOG(INFO) << "Flying state changed :";
	CBebopState *bebopState = (CBebopState*)custom;

	if (!bebopState)
	{
		LOG(ERROR) << "bebop state object is invalid";
		return;
	}

	switch (state) {
		case ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_LANDED:
		{
			bebopState->SetFlyingState(FlyingState::LANDED);
			LOG(INFO) << "Flying state : LANDED";
		}
			break;
		case ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_TAKINGOFF:
		{
			bebopState->SetFlyingState(FlyingState::TAKINGOFF);
			LOG(INFO) << "Flying state : TAKING OFF";
		}
			break;
		case ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_HOVERING:
		{
			bebopState->SetFlyingState(FlyingState::HOVERING);
			LOG(INFO) << "Flying state : HOVERING";
		}
			break;
		case ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_FLYING:
		{
			bebopState->SetFlyingState(FlyingState::FLYING);
			LOG(INFO) << "Flying state : FLYING";
		}
			break;
		case ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_LANDING:
		{
			bebopState->SetFlyingState(FlyingState::LANDING);
			LOG(INFO) << "Flying state : LANDING";
		}
			break;
		case ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_EMERGENCY:
		{
			bebopState->SetFlyingState(FlyingState::EMERGENCY);
			LOG(INFO) << "Flying state : EMERGENCY";
		}
			break;
		default:
			break;
	}

}

void CBebopCallbacks::NumberOfSatelitesChangedCallback(uint8_t numberOfSatelites, void *custom)
{
	LOG(INFO) << "Number of satellites: " << (int) numberOfSatelites;
	CBebopState *bebopState = (CBebopState*)custom;

	if (!bebopState)
	{
		LOG(ERROR) << "bebop state object is invalid";
		return;
	}

	bebopState->SetNumberOfSattelites(numberOfSatelites);
}

TBebopStateCantainer CBebopCallbacks::GetBebopState() const
{
	if (!m_bebopState)
	{
		LOG(ERROR) << "Invalid bebop state";
		return TBebopStateCantainer();
	}

	return TBebopStateCantainer(m_bebopState->GetState());
}

} /* namespace rebop */
