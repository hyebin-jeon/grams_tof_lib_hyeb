#pragma once

#ifndef _TOF_TREEDATA_H
#define _TOF_TREEDATA_H

#include "TTree.h"
#include "TObject.h"

class TOF_TreeData : public TTree
{
  /// How to use:
	/// TOF_TreeData t1;          // do nothing
  /// TOF_TreeData t2(true);    // setBranchAddress() excuted 
  /// 
	/// example:
  ///	TOF_TreeData* t = (TOF_TreeData*) fin->Get("data");
  ///	t->setBranchAddress();
  ///	int entries = t->GetEntries();
  ///	for( int i=0; i<entries; i++ )
  ///	{
  ///		t->GetEntry(i);
  ///		...
  ///	}
	///

	public: 
	  TOF_TreeData() = default;

		explicit TOF_TreeData(bool init) { 
        if (init) setBranchAddress();
    }

    ~TOF_TreeData() = default;

	public: 
		void setBranchAddress();
	
	private: 
    float           step1;
    float           step2;
    long long       stepBegin;
    long long       stepEnd;
    long long       frameID;
    unsigned int    channelID;
    unsigned short  tacID;
    unsigned short  tCoarse;
    unsigned short  eCoarse;
    unsigned short  tFine;
    unsigned short  eFine;

    //float		           step1;
    //float		           step2;
    //uint64_t           stepBegin;
    //uint64_t           stepEnd;
    //unsigned long long frameID;
    //uint32_t           channelID;
    //unsigned short     tacID;
    //uint16_t           tCoarse;
    //uint16_t           eCoarse;
    //uint16_t           tFine;
    //uint16_t           eFine;
	public:
    float		 getStep1()    { return step1;     };
    float		 getStep2()    { return step2;     };
    uint64_t getStepBgin() { return stepBegin; };
    uint64_t getStepEnd()  { return stepEnd;   };
    uint64_t getFrameID()  { return frameID;   };
    uint32_t getChannelID(){ return channelID; };
    uint8_t  getTacID()    { return tacID;     };
    uint16_t getTCoarse()  { return tCoarse;   };
    uint16_t getECoarse()  { return eCoarse;   };
    uint16_t getTFine()    { return tFine;     };
    uint16_t getEFine()    { return eFine;     };

	public:
    inline uint8_t getPortID    ( uint32_t absolute_chanID ) { return (absolute_chanID >> 17) & 0x1F; };
    inline uint8_t getSlaveID   ( uint32_t absolute_chanID ) { return (absolute_chanID >> 12) & 0x1F; };
    inline uint8_t getAsicID    ( uint32_t absolute_chanID ) { return (absolute_chanID >> 6 ) & 0x3F; }; // 0-15
    inline uint8_t getChannelID ( uint32_t absolute_chanID ) { return (absolute_chanID >> 0) & 0x3F; }; // 0-63

		uint32_t getAbsoluteChannelID( uint8_t febD_connID, uint8_t febS_chanID );
		uint8_t  getChannelIdOnFebS( uint32_t absolute_chanID ); // ch.001-128
		uint8_t  getConnIdOnFebD( uint32_t absolute_chanID ); // F1-8
		std::vector<uint8_t> getConnIdOnFebD_and_ChannelIdOnFebS (uint32_t absolute_chanID );

	ClassDef(TOF_TreeData, 1)

};

#endif
