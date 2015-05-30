#pragma once

// Includes
#include "CVehicleInterface.h"
#include "IBebopInterface.h"
#include "BebopCommandsAndData.h"

// Normally, this is forbidden by my style guidelines, but you SHOULD only ever be including one vehicle's namespace
using namespace rebop::commands::bebop::navigation;
using namespace rebop::commands::bebop::video;

namespace rebop
{

class CBebopInterface : public IBebopInterface, public CVehicleInterface
{
public:
	CBebopInterface();
	virtual ~CBebopInterface();

	virtual void Initialize();
	virtual void Update();
	virtual void Cleanup();
	virtual bool IsConnected();

	virtual bool Takeoff();
	virtual bool Land();
	virtual bool Emergency();
	virtual bool FlatTrim();
	virtual bool NavigateHome( ENavigateHome startOrStopIn );
	virtual bool SendPilotCommand( const TPilotCommand &poseIn );
	virtual bool SetAutoTakeoffMode( EAutoTakeoffMode modeIn );
	virtual bool Flip( EFlipDirection directionIn );
	virtual bool SetMaxAltitude( float maxAltitudeMetersIn );
	virtual bool SetMaxTilt( float maxTiltDegreesIn );
	virtual bool SetMaxVerticalSpeed( float maxVerticalSpeedMetersPerSecIn );
	virtual bool SetMaxRotationSpeed( float maxRotationSpeedDegPerSecIn );
	virtual bool SetAbsoluteControlMode( EAbsoluteControlMode modeIn );
	virtual bool SetHullProtectionPresence( EHullPresence presenceIn );
	virtual bool SetOutdoorMode( EOutdoorMode modeIn );
	virtual bool SetGpsHomeLocation( const TGpsHomeLocation &locationIn );
	virtual bool ResetGpsHome();
	virtual bool StartVideo();
	virtual bool StopVideo();

	virtual TFrame GetVideoFrame() const;
};

}
