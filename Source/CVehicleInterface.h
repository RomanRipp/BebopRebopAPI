#pragma once

// Includes
#include "CNetworkInterface.h"
#include "CVideoInterface.h"
#include "Utility.h"

namespace rebop {

class CVehicleInterface {
public:
	// Pointers

	// Attributes
	CNetworkInterface m_networkInterface;
	CVideoInterface m_videoInterface;

	bool m_isConnected;

	//Methods
	CVehicleInterface();
	virtual ~CVehicleInterface();
};

}
