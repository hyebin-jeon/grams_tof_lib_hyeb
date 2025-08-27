#pragma once

#ifndef _TOF_CHANNELCONVERSION_
#define _TOF_CHANNELCONVERSION_

#include "TObject.h"
class TOF_ChannelConversion : public TObject
{
  public:
    inline static TOF_ChannelConversion * theChanCov{nullptr}; 
  	static TOF_ChannelConversion *getInstance() {
  		if( theChanCov == nullptr ) {
  			theChanCov = new TOF_ChannelConversion;
  		}
  
  		return theChanCov;
  	}
  
  	~TOF_ChannelConversion() = default;

	public:
    uint32_t getAbsoluteChannelID( uint8_t febD_connID, uint8_t febS_chanID );
		
	ClassDef(TOF_ChannelConversion, 1)

};

#endif
