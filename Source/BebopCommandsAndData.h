#pragma once

#include <stdint.h>
#include "libARCommands/ARCOMMANDS_Types.h"

// Enumerations
namespace rebop{
namespace bebop
{
	enum class ENavCommands
	{
		FlatTrim,
		Takeoff,
		SetPose,
		Land,
		Emergency,
		NavigateHome,
		SetAutoTakeoffMode,
		Flip,
		SetMaxAltitude,
		SetMaxTilt,
		SetAbsoluteControl,
		SetMaxVerticalSpeed,
		SetMaxRotationSpeed,
		SetHullProtectionPresence,
		SetOutdoorMode,
		SetGpsHome,
		ResetGpsHome
	};

	enum class EMediaCommands
	{
		RecordPicture,
		RecordVideo,
		SetCameraOrientation,
		SetPictureFormatSelection,
		SetAutoWhiteBalanceSelection,
		SetExposureSelection,
		SetExpositionSelection,
		SetSaturationSelection,
		SetTimelapseSelection,
		SetVideoAutorecordSelection
	};

	enum class ENetworkCommands
	{
		LaunchWifiScan,
		InquireAuthChannels,
		SetWifiSelection
	};

	enum class EDebugCommands
	{
		VideoEnableWobbleCancellation,
		VideoSyncAngleGyros,
		VideoManualWhiteBalance,
		UseDrone2Battery
	};
}
}

// Data structures
namespace rebop{
namespace commands{
namespace bebop{
namespace navigation{

	struct TPilotCommand
	{
		uint8_t flag;		// Boolean flag to activate roll/pitch movement
		int8_t roll;		// Roll consign for the drone 								[-100:100] Normalized
		int8_t pitch;		// Pitch consign for the drone 								[-100:100] Normalized
		int8_t yaw;			// Yaw rate consign for the drone 							[-100:100] Normalized
		int8_t gaz;			// Altitude rate consign for the drone 						[-100:100] Normalized
		int8_t psi;			// Magnetic north heading of the controlling device (deg) 	[-180:180] Degrees
	};

	struct TGpsHomeLocation
	{
		double latitude;	// Decimal degrees
		double longitude;	// Decimal degrees
		double altitude;	// Meters
	};

	enum class ENavigateHome
	{
		STOP 	= 0,
		START 	= 1
	};

	enum class EAutoTakeoffMode
	{
		DISABLED 	= 0,
		ENABLED 	= 1
	};

	enum class EFlipDirection
	{
		FRONT = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_FRONT,
		BACK = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_BACK,
		RIGHT = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_RIGHT,
		LEFT = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_LEFT,
		MAX = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_MAX
	};

	enum class EAbsoluteControlMode
	{
		DISABLED 	= 0,
		ENABLED 	= 1
	};

	enum class EHullPresence
	{
		NOT_PRESENT = 0,
		PRESENT 	= 1
	};

	enum class EOutdoorMode
	{
		INDOOR		= 0,
		OUTDOOR		= 1
	};
}
namespace video
{

	class TFrame
	{
	public:
		TFrame()
		: m_frameData(0),
		  m_frameDataSize(0),
		  m_numberOfSkippedFrames(-1),
		  m_isFlushFrame(-1),
		  m_isValid(false)
		{

		}

		TFrame(uint8_t* frameData, uint32_t frameDataSize, int numberOfSkippedFrames, int isFlushFrame)
			: m_frameData(frameData),
			  m_frameDataSize(frameDataSize),
			  m_numberOfSkippedFrames(numberOfSkippedFrames),
			  m_isFlushFrame(isFlushFrame),
			  m_isValid(true)
		{

		}

		~TFrame()
		{

		}

		uint8_t* GetData() const
		{
			return m_frameData;
		}

		uint32_t GetFrameDataSize() const
		{
			return m_frameDataSize;
		}

		int GetNumberOfSkippedFrames() const
		{
			return m_numberOfSkippedFrames;
		}

		bool IsFlushFrame() const
		{
			return (m_isFlushFrame > 0);
		}

		bool IsValid() const
		{
			return m_isValid;
		}

	private:
		uint8_t* m_frameData;
		uint32_t m_frameDataSize;
		int 	 m_numberOfSkippedFrames;
		int 	 m_isFlushFrame;
		bool 	 m_isValid;
	};
}
}
}

namespace state{
namespace bebop{

	enum FlyingState
	{
		LANDED,
		TAKINGOFF,
		HOVERING,
		FLYING,
		LANDING,
		EMERGENCY
	};

	class TBebopStateCantainer
	{
	public:

		TBebopStateCantainer()
			: m_isValid(false),
			  m_batteryCharge(-1),
			  m_flyingState(FlyingState::LANDED),
			  m_numberOfSatelites(-1)
		{

		}

//		TBebopStateCantainer(int batteryCharge, FlyingState flyingState)
//			: m_isValid(true),
//			  m_batteryCharge(batteryCharge),
//			  m_flyingState(flyingState)
//		{
//
//		}

		~TBebopStateCantainer()
		{

		}

		bool m_isValid;
		int m_batteryCharge;
		FlyingState m_flyingState;
		int m_numberOfSatelites;
	};

}
}
}
