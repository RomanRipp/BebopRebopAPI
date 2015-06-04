#pragma once

// Includes
#include "CNetworkInterface.h"
#include "CVideoInterface.h"
#include "CBebopState.h"
#include "CBebopCallbacks.h"

#include "Utility.h"

namespace rebop {

class CVehicleInterface {
public:
	//Methods
	CVehicleInterface();
	virtual ~CVehicleInterface();

protected:
	bool CommonCommonAllStates();

	// Attributes
	CNetworkInterface m_networkInterface;
	CVideoInterface m_videoInterface;
	CBebopCallbacks m_bebopCallbacks;

	bool m_isConnected;
};

}
