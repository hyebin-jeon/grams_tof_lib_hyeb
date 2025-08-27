
#include "TOF_TreeData.h"

ClassImp( TOF_TreeData );

void TOF_TreeData::setBranchAddress()
{
  this->SetBranchAddress("step1"    , &step1    );
  this->SetBranchAddress("step2"    , &step2    );
  this->SetBranchAddress("frameID"  , &frameID  );
  this->SetBranchAddress("channelID", &channelID);
  this->SetBranchAddress("tacID"    , &tacID    );
  this->SetBranchAddress("tcoarse"  , &tCoarse  );
  this->SetBranchAddress("ecoarse"  , &eCoarse  );
  this->SetBranchAddress("tfine"    , &tFine    );
  this->SetBranchAddress("efine"    , &eFine    );

	return;
}

uint32_t TOF_TreeData::getAbsoluteChannelID( uint8_t febD_connID, uint8_t febS_chanID )
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

uint8_t TOF_TreeData::getChannelIdOnFebS( uint32_t absolute_chanID )
{
	//uint8_t portID    = (absolute_chanID >> 17) & 0x1F; 
  //uint8_t slaveID   = (absolute_chanID >> 12) & 0x1F;
  uint8_t asicID    = (absolute_chanID >> 6)  & 0x3F; // 0 to 15
	//uint8_t connID    = asicID/2;
  uint8_t channelID = (absolute_chanID >> 0)  & 0x3F; // 0 to 63

	uint8_t chanID_febS = 64*(asicID%2) + channelID + 1; // 1 is because the FEB_S board connector labels begin from 1

	return chanID_febS;
}
		
uint8_t TOF_TreeData::getConnIdOnFebD( uint32_t absolute_chanID )
{ 
	//uint8_t portID    = (absolute_chanID >> 17) & 0x1F; 
  //uint8_t slaveID   = (absolute_chanID >> 12) & 0x1F;
  uint8_t asicID    = (absolute_chanID >> 6)  & 0x3F;
	uint8_t connID    = asicID/2 + 1;
  uint8_t channelID = (absolute_chanID >> 0)  & 0x3F; // 0 to 63

	return connID;
}

std::vector<uint8_t> TOF_TreeData::getConnIdOnFebD_and_ChannelIdOnFebS (uint32_t absolute_chanID )
{
	//uint8_t portID    = (absolute_chanID >> 17) & 0x1F; 
  //uint8_t slaveID   = (absolute_chanID >> 12) & 0x1F;
  uint8_t asicID    = (absolute_chanID >> 6)  & 0x3F;
	uint8_t connID    = asicID/2 + 1;
  uint8_t channelID = (absolute_chanID >> 0)  & 0x3F; // 0 to 63
	
	uint8_t chanID_febS = 64*(asicID%2) + channelID + 1; // 1 is because the FEB_S board connector labels begin from 1

	std::vector<uint8_t> reval={ connID, chanID_febS };
	return reval;
};

