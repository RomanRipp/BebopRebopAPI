/*
 * IBebop.h
 *
 *  Created on: May 9, 2015
 *      Author: romanripp
 */

#pragma once

#include "BebopCommandsAndData.h"
#include "IVideoInterface.h"
#include "Utility.h"

using namespace rebop::commands::bebop::navigation;
using namespace rebop::commands::bebop::video;
using namespace rebop::state::bebop;

namespace rebop
{
class IBebopInterface
{
public:
	virtual ~IBebopInterface(){}

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Cleanup() = 0;
	virtual bool IsConnected() = 0;

	virtual bool Takeoff() = 0;
	virtual bool Land() = 0;
	virtual bool Emergency() = 0;
	virtual bool FlatTrim() = 0;
	virtual bool NavigateHome( ENavigateHome startOrStopIn ) = 0;
	virtual bool SendPilotCommand( const TPilotCommand &poseIn ) = 0;
	virtual bool SetAutoTakeoffMode( EAutoTakeoffMode modeIn ) = 0;
	virtual bool Flip( EFlipDirection directionIn ) = 0;
	virtual bool SetMaxAltitude( float maxAltitudeMetersIn ) = 0;
	virtual bool SetMaxTilt( float maxTiltDegreesIn ) = 0;
	virtual bool SetMaxVerticalSpeed( float maxVerticalSpeedMetersPerSecIn ) = 0;
	virtual bool SetMaxRotationSpeed( float maxRotationSpeedDegPerSecIn ) = 0;
	virtual bool SetAbsoluteControlMode( EAbsoluteControlMode modeIn ) = 0;
	virtual bool SetHullProtectionPresence( EHullPresence presenceIn ) = 0;
	virtual bool SetOutdoorMode( EOutdoorMode modeIn ) = 0;
	virtual bool SetGpsHomeLocation( const TGpsHomeLocation &locationIn ) = 0;
	virtual bool ResetGpsHome() = 0;
	virtual bool StartVideo() = 0;
	virtual bool StopVideo() = 0;

	virtual IVideoInterface GetVideo() const = 0;
	virtual bool GetFlyingState(FlyingState& state) const = 0;
	virtual bool GetBatteryCharge(int& charge) const = 0;
};
}
