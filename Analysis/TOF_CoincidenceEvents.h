/// v2

#pragma once

#include "TObject.h"
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "TOF_TdcQdcCalibration.h"
#include "TOF_TreeData.h"
#include "TOF_CoincidenceChannelInfo.h"

enum class TOF_QdcCalibMethod {
	fGetEnergy = 0,
	fLinear = 1,
	fTiming = 2,
};

class TOF_CoincidenceEvents : public TObject
{
	public:
    inline static TOF_CoincidenceEvents * theCoin{nullptr}; 
  	static TOF_CoincidenceEvents *getInstance() {
  		if( theCoin == nullptr ) {
  			theCoin = new TOF_CoincidenceEvents;
  		}

  		return theCoin;
  	}
  
  	//~TOF_CoincidenceEvents() = default;
  	~TOF_CoincidenceEvents();
		
		//TOF_TdcQdcCalibration* theCalib{nullptr};
	
	private:
		TTree* fTreeCoin{nullptr};
	  std::vector<TOF_CoincidenceChannelInfo> vBranch;
		std::vector<std::vector<TOF_CoincidenceChannelInfo>> vTree;

	private:
		TOF_TreeData* fTree{nullptr};
		uint32_t fTrigChannelID{0}; // absolute channel ID of a trigger channelj
		double   fCoinTimeWindow{1};
		TOF_QdcCalibMethod fQdcCalibMethod{ TOF_QdcCalibMethod::fGetEnergy };

	  std::map< uint32_t, TOF_CoincidenceChannelInfo> fChannelDataList; // {nullptr};
		std::vector< uint32_t > fActiveChannelList;

	public:
		int  setTreeData( TOF_TreeData* tr );// { fTree = tr; };
		void setTriggerChannel( uint32_t trigCh ) { fTrigChannelID = trigCh; }; // coincidence evt using trigger channel is not prepared yet
		void setActiveChannels( std::vector<uint32_t> chanList );
		void setCoincidenceTimeWindowInClk( double twindow ) { fCoinTimeWindow = twindow; }; // in clock

		TTree* getCoincidenceEventsTree();
		std::vector<std::vector<TOF_CoincidenceChannelInfo>> getCoincidenceEvents();

		void createCoinTree();

		void reset();


	public:
		void setQdcCalibMethod( TOF_QdcCalibMethod calMethod ) { fQdcCalibMethod = calMethod; };

	ClassDef(TOF_CoincidenceEvents, 1)

};





