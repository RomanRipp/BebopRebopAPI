// Includes
#include "CVehicleInterface.h"

#include "Utility.h"

// Set up easylogger
INITIALIZE_EASYLOGGINGPP

// Namespaces
using namespace rebop;

CVehicleInterface::CVehicleInterface()
{
	// Init attributes
	m_isConnected = false;
}

CVehicleInterface::~CVehicleInterface()
{

}

bool CVehicleInterface::CommonCommonAllStates()
{
	//Send "CommonCommonAllStates" command to avoid drifting
	CCommandPacket packet( 128 );

	// Generate command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateCommonCommonAllStates(
			packet.m_pData,
			packet.m_bufferSize,
			&packet.m_dataSize );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Command should be acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send anti-drift command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate anti-drift command. Err: " << cmdError;
		return false;
	}

	LOG( INFO ) << "Sent anti-drift command.";
	return true;
}
