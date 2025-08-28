#pragma once

#ifndef _TOF_CHANNELCONVERSION_
#define _TOF_CHANNELCONVERSION_

#include <map>
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
  	};
  
  	~TOF_ChannelConversion() = default;

	private: 
		/// map between connector ID on FEB/S (J1-J128) and channel ID on the software (ch0-ch127)
		std::map<uint8_t, uint8_t> fMap_ConnIdToChannelId;
		std::map<uint8_t, uint8_t> fMap_ChannelIdToConnId;

	public:
		void fillMapConnIdToChannelId();
		void fillMapChannelIdToConnId();
		void fillChannelMaps();

		std::map< uint8_t, uint8_t > getMapConnIdToChannelId();
		std::map< uint8_t, uint8_t > getMapChannelIdToConnId();

		/// Conversion from connector IDs (hardware) to channel IDs (software)
		uint8_t getAsicID( uint8_t febD_connID, uint8_t febS_connID ); // [0-15]
		uint8_t getChannelID_128( uint8_t febS_connID ); // [0-127] within a set of two ASICs
		uint8_t getChannelID_64 ( uint8_t febS_connID ); // [0-63] within an ASIC
    uint32_t getAbsoluteChannelID( uint8_t febD_connID, uint8_t febS_connID );

		/// Break an absolute channel ID into each element
		uint8_t getPortID   ( uint32_t channel );
		uint8_t getSlaveID  ( uint32_t channel );
		uint8_t getAsicID   ( uint32_t channel ); // 0-15
		uint8_t getChannelID( uint32_t channel ); // 0-63

		uint8_t getAsicIdx   ( uint32_t channel ); // 0 or 1
		uint8_t getChannelID_128( uint32_t channel ); // 0-127
		
		/// Conversion from an absolute channel ID (software) to connector IDs (hardware)
		uint8_t getConnIdOnFebD( uint32_t channel ); // 1-8
		uint8_t getConnIdOnFebS( uint32_t channel ); // 1-128
		
	  ClassDef(TOF_ChannelConversion, 1)

};

#endif
