#include "TOF_ChannelConversion.h"

ClassImp( TOF_ChannelConversion );

uint32_t TOF_ChannelConversion::getAbsoluteChannelID( uint8_t febD_connID, uint8_t febS_chanID )
{
	uint8_t portID  = 0; // DAQ portID
	uint8_t slaveID = 0; // ID used for daisy chain of FEB/D
	uint8_t asicID0 = (febD_connID-1)*2; 
	uint8_t asicID1 = asicID0 + 1;
	uint8_t chanID = (febS_chanID-1)%64;

	uint8_t asicID = 99;
	if( (febS_chanID-1)/64 == 0 ) asicID = asicID0;
	else asicID = asicID1;

	/// According to the user manual, TOFPET2 ASIC SIPM Readout System Software user guide,
	/// absolute channel ID = 131072*portID + 4096*slaveID + 64*chipID + ChannelID
	uint32_t absChanID = pow(2, 17) * portID
		                 + pow(2, 12) * slaveID
		                 + pow(2,  6) * asicID
	                   + chanID;

	//cout << Form("portID= %2d, slaveID= %2d, asicID0= %2d, asicID1= %2d, chanID= %2d ==> Abs. channelID = %7d", portID, slaveID, asicID0, asicID1, chanID, absChanID) << endl;

	return absChanID;
}


