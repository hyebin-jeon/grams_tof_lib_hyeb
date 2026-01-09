/// v3

#include "TOF_CoincidenceEvents.h"

ClassImp( TOF_CoincidenceEvents );
		
void TOF_CoincidenceEvents::setActiveChannels( std::vector<uint32_t> chanList )
{
	fActiveChannelList.clear();

	if( chanList.size() == 0 ) {
		std::cout<< "TOF_QdcCalibMethod::setActiveChannels(). A given channel list is empty." << std::endl;
	}

	std::sort( chanList.begin(), chanList.end() );

	fActiveChannelList = chanList;

	//for( auto chan: fActiveChannelList )
	//	std::cout << Form("[TOF_CoincidenceEvents::setActiveChannels] active channel: %03d", chan) << std::endl;

	return;
}

int TOF_CoincidenceEvents::setTreeData( TOF_TreeData* tr )
{
	if( !tr ) {
		std::cout << "[ERR] TOF_CoincidenceEvents::setTreeDat( TOF_TreeData* tr ), Given 'tr' NOT FOUND" << std::endl;
		return -1;
	}
	tr->SetBranchStatus("*",1);

	fTree= tr;
	fTree->setBranchAddress();

	return 1;
}

std::vector<std::vector<TOF_CoincidenceChannelInfo>> TOF_CoincidenceEvents::getCoincidenceEvents()
{
	if( !fTree ) {
		std::cout << "[ERR] TOF_QdcCalibMethod::getCoincidenceEvents(). fTree is NULL. Exit." << std::endl;
		return vTree;
	}

	if( fTree->GetEntries() == 0 ) {
		std::cout << "[ERR] TOF_QdcCalibMethod::getCoincidenceEvents(). fTree's entries = 0. Exit." << std::endl;
		return vTree;
	}
	fTree->setBranchAddress(); // duplicate

	vTree.clear();

	//std::cout << "ok1" << std::endl;

	std::cout << "ok1.2" << std::endl;
	std::cout << "Address fTree: " << fTree << std::endl;

	fTree->GetEntry(0);
	long long frameID0= fTree->getFrameID(); 
	long long lastTime= CLOCKS_IN_A_FRAME * frameID0 + fTree->getTCoarse(); 	
	long long currTime= -1; // 64-bit signed int

	TOF_CoincidenceChannelInfo channelInfo{};
	TOF_CoincidenceChannelInfo channelBrElement{};
	std::map< uint32_t, TOF_CoincidenceChannelInfo> vChannelData;
	std::map< uint32_t, bool > vHitChannel;

	const int chanSize = fActiveChannelList.size();
	
	std::cout << "ok2. fTree->GetEntries()= " << fTree->GetEntries() << std::endl;
	//fTree->Print();
	  
	std::vector<TOF_CoincidenceChannelInfo> vBranchTemp;
	vBranchTemp.clear();

	for( int i=0; i<fTree->GetEntries(); i++ )
	{
		fTree->GetEntry(i);

		auto frameID      = fTree->getFrameID();
		auto tCoarse      = fTree->getTCoarse();
		auto absChannelID = fTree->getChannelID(); // absolute channel ID

		channelInfo.frameID   = frameID;
		channelInfo.channelID = absChannelID;
		channelInfo.tacID   = fTree->getTacID();
		channelInfo.tCoarse = fTree->getTCoarse();
		channelInfo.eCoarse = fTree->getECoarse();
		channelInfo.tFine   = fTree->getTFine()  ;
		channelInfo.eFine   = fTree->getEFine()  ;

		long long time_trigCh;
		if( absChannelID == fTrigChannelID ) time_trigCh = CLOCKS_IN_A_FRAME * frameID + tCoarse;

		/// current time in clock
		currTime = CLOCKS_IN_A_FRAME * frameID + tCoarse;
		
			
		/// coincidence events within 'coincidenceT' clocks
		if( fabs(currTime - lastTime) > fCoinTimeWindow) 
		{
			vChannelData.clear();
			vHitChannel.clear();
		}

		for( auto activeChanID: fActiveChannelList )
		{
		  if( absChannelID == activeChanID )
		  {
				vHitChannel [absChannelID] = true;
		    vChannelData[absChannelID] = channelInfo;
			}
		}

		if( vChannelData.size() == 1 ) lastTime = currTime;
		
		bool good=1;
		for( auto activeChanID: fActiveChannelList )
		{
			good &= vHitChannel[ activeChanID ];
		}

		if( i==0 )
		std::cout << Form("[%03d] good: %d, currTime: %10lld, frameID= %10lld, tCoarse= %10hu, tFine= %10hu, channelID= %3u", i, good, currTime, frameID, channelInfo.tCoarse, channelInfo.tFine, channelInfo.channelID ) << std::endl;

		if( vChannelData.size() == fActiveChannelList.size() && good )
		{
			vBranchTemp.clear();

			//for( const auto [chan, chanData]: vChannelData )
				//std::cout << Form("coincidence channels: %03d, idx = %03d", chanData.channelID, chan ) << std::endl;

		  for( auto activeChanID: fActiveChannelList )
			{
				auto hit_channelID = activeChanID;
		    auto hit_frameID   = vChannelData[activeChanID].frameID;
		    auto hit_tacID     = vChannelData[activeChanID].tacID  ;
		    auto hit_tCoarse   = vChannelData[activeChanID].tCoarse;
		    auto hit_eCoarse   = vChannelData[activeChanID].eCoarse;
		    auto hit_tFine     = vChannelData[activeChanID].tFine  ;
		    auto hit_eFine     = vChannelData[activeChanID].eFine  ;

				//std::cout << Form("[%03d] [coincidence] channelID: %03d, frameID= %10lld, tacID= %d, tCoarse= %10hu, tFine= %10hu, eCoarse= %10hu, eFine= %10hu,", i, hit_channelID, hit_frameID, hit_tacID, hit_tCoarse, hit_tFine, hit_eCoarse, hit_eFine ) << std::endl;

				auto hit_timeBegin = TOF_TdcQdcCalibration::getInstance()->getCalibratedTime( TOF_Branch::fBranchT, hit_channelID, hit_tacID, hit_frameID, hit_tCoarse, hit_tFine );
				//auto hit_timeBegin = theCalib->getCalibratedTime( TOF_Branch::fBranchT, hit_channelID, hit_tacID, hit_frameID, hit_tCoarse, hit_tFine );
				
				//std::cout << Form("[%03d] [coincidence] channelID: %03d, t_begin: %8.2f", i, hit_channelID, hit_timeBegin ) << std::endl;

				//auto ecoarse = channelBrElement.eCoarse;
				auto ecoarse = hit_eCoarse;
	      if((hit_eCoarse - hit_tCoarse) < -256) ecoarse += 1024;
				auto hit_timeEnd = double((frameID*1024+ ecoarse));
				
				//std::cout << Form("[%03d] [coincidence] channelID: %03d, t_begin: %8.2f, t_end: %8.2f", i, hit_channelID, hit_timeBegin, hit_timeEnd ) << std::endl;

				double hit_qdc_cal = 0;
				if( fQdcCalibMethod == TOF_QdcCalibMethod::fGetEnergy )
					hit_qdc_cal = TOF_TdcQdcCalibration::getInstance()->getEnergy( hit_channelID, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_timeBegin  );
					//hit_qdc_cal = theCalib->getEnergy( hit_channelID, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_timeBegin  );
				else if( fQdcCalibMethod == TOF_QdcCalibMethod::fLinear ) 
					hit_qdc_cal = 0; ////////////////////////////////
				else if( fQdcCalibMethod == TOF_QdcCalibMethod::fTiming ) 
					hit_qdc_cal =	TOF_TdcQdcCalibration::getInstance()->getCalibratedQDC( hit_channelID, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_tCoarse, hit_timeBegin  );
					//hit_qdc_cal =	theCalib->getCalibratedQDC( hit_channelID, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_tCoarse, hit_timeBegin  );
				else hit_qdc_cal = 0;
				
				channelBrElement.channelID = hit_channelID;
		    channelBrElement.frameID   = hit_frameID  ;
		    channelBrElement.tacID     = hit_tacID    ;
		    channelBrElement.tCoarse   = hit_tCoarse  ;
		    channelBrElement.eCoarse   = hit_eCoarse  ;
		    channelBrElement.tFine     = hit_tFine    ;
		    channelBrElement.eFine     = hit_eFine    ;
				channelBrElement.timeBegin = hit_timeBegin;
				channelBrElement.timeEnd   = hit_timeEnd  ;
				channelBrElement.qdc_cal   = hit_qdc_cal  ;

				vBranchTemp.push_back( channelBrElement );
			}

			//fTreeCoin->Fill();
			vTree.push_back( vBranchTemp );
		}
	}

	//fTreeCoin->Print();
	std::cout << "ok3. vTree.size() " << vTree.size() << std::endl;

	std::cout << "[TOF_CoincidenceEvents::getCoincidenceEvents] Completed" << std::endl;

	//return fTreeCoin;
	return vTree;
}


TTree* TOF_CoincidenceEvents::getCoincidenceEventsTree()
{
	if( !fTree ) {
		std::cout << "[ERR] TOF_QdcCalibMethod::getCoincidenceEvents(). fTree is NULL. Exit." << std::endl;
		return nullptr;
	}

	if( fTree->GetEntries() == 0 ) {
		std::cout << "[ERR] TOF_QdcCalibMethod::getCoincidenceEvents(). fTree's entries = 0. Exit." << std::endl;
		return nullptr;
	}

	vBranch.clear();


	/// create fTreeCoin and do Branch()
	fTreeCoin = new TTree( "tCoin", "tCoin" );
	fTreeCoin->SetDirectory(0);
	auto br = fTreeCoin->Branch( "coinEvt", &vBranch, 64000, 0 );

	fTree->GetEntry(0);
	long long frameID0= fTree->getFrameID(); 
	long long lastTime= CLOCKS_IN_A_FRAME * frameID0 + fTree->getTCoarse(); 	
	long long currTime= -1; // 64-bit signed int

	TOF_CoincidenceChannelInfo channelInfo{};
	TOF_CoincidenceChannelInfo channelBrElement{};
	std::map< uint32_t, TOF_CoincidenceChannelInfo> vChannelData;
	std::map< uint32_t, bool > vHitChannel;

	const int chanSize = fActiveChannelList.size();
	
	for( int i=0; i<fTree->GetEntries(); i++ )
	{
		fTree->GetEntry(i);

		auto frameID      = fTree->getFrameID();
		auto tCoarse      = fTree->getTCoarse();
		auto absChannelID = fTree->getChannelID(); // absolute channel ID

		channelInfo.frameID   = frameID;
		channelInfo.channelID = absChannelID;
		channelInfo.tacID   = fTree->getTacID();
		channelInfo.tCoarse = fTree->getTCoarse();
		channelInfo.eCoarse = fTree->getECoarse();
		channelInfo.tFine   = fTree->getTFine()  ;
		channelInfo.eFine   = fTree->getEFine()  ;

		long long time_trigCh;
		if( absChannelID == fTrigChannelID ) time_trigCh = CLOCKS_IN_A_FRAME * frameID + tCoarse;

		/// current time in clock
		currTime = CLOCKS_IN_A_FRAME * frameID + tCoarse;
		
			
		/// coincidence events within 'coincidenceT' clocks
		if( fabs(currTime - lastTime) > fCoinTimeWindow) 
		{
			vChannelData.clear();
			vHitChannel.clear();
		}

		for( auto activeChanID: fActiveChannelList )
		{
		  if( absChannelID == activeChanID )
		  {
				vHitChannel [absChannelID] = true;
		    vChannelData[absChannelID] = channelInfo;
			}
		}

		if( vChannelData.size() == 1 ) lastTime = currTime;
		
		bool good=1;
		for( auto activeChanID: fActiveChannelList )
		{
			good &= vHitChannel[ activeChanID ];
		}

		if( i==0 )
		std::cout << Form("[%03d] good: %d, currTime: %10lld, frameID= %10lld, tCoarse= %10hu, tFine= %10hu, channelID= %3u", i, good, currTime, frameID, channelInfo.tCoarse, channelInfo.tFine, channelInfo.channelID ) << std::endl;

		if( vChannelData.size() == fActiveChannelList.size() && good )
		{
			vBranch.clear();

			//for( const auto [chan, chanData]: vChannelData )
				//std::cout << Form("coincidence channels: %03d, idx = %03d", chanData.channelID, chan ) << std::endl;

		  for( auto activeChanID: fActiveChannelList )
			{
				auto hit_channelID = activeChanID;
		    auto hit_frameID   = vChannelData[activeChanID].frameID;
		    auto hit_tacID     = vChannelData[activeChanID].tacID  ;
		    auto hit_tCoarse   = vChannelData[activeChanID].tCoarse;
		    auto hit_eCoarse   = vChannelData[activeChanID].eCoarse;
		    auto hit_tFine     = vChannelData[activeChanID].tFine  ;
		    auto hit_eFine     = vChannelData[activeChanID].eFine  ;

				//std::cout << Form("[%03d] [coincidence] channelID: %03d, frameID= %10lld, tacID= %d, tCoarse= %10hu, tFine= %10hu, eCoarse= %10hu, eFine= %10hu,", i, hit_channelID, hit_frameID, hit_tacID, hit_tCoarse, hit_tFine, hit_eCoarse, hit_eFine ) << std::endl;

				auto hit_timeBegin = TOF_TdcQdcCalibration::getInstance()->getCalibratedTime( TOF_Branch::fBranchT, hit_channelID, hit_tacID, hit_frameID, hit_tCoarse, hit_tFine );
				//auto hit_timeBegin = theCalib->getCalibratedTime( TOF_Branch::fBranchT, hit_channelID, hit_tacID, hit_frameID, hit_tCoarse, hit_tFine );
				
				//std::cout << Form("[%03d] [coincidence] channelID: %03d, t_begin: %8.2f", i, hit_channelID, hit_timeBegin ) << std::endl;

				//auto ecoarse = channelBrElement.eCoarse;
				auto ecoarse = hit_eCoarse;
	      if((hit_eCoarse - hit_tCoarse) < -256) ecoarse += 1024;
				auto hit_timeEnd = double((frameID*1024+ ecoarse));
				
				//std::cout << Form("[%03d] [coincidence] channelID: %03d, t_begin: %8.2f, t_end: %8.2f", i, hit_channelID, hit_timeBegin, hit_timeEnd ) << std::endl;

				double hit_qdc_cal = 0;
				if( fQdcCalibMethod == TOF_QdcCalibMethod::fGetEnergy )
					hit_qdc_cal = TOF_TdcQdcCalibration::getInstance()->getEnergy( hit_channelID, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_timeBegin  );
					//hit_qdc_cal = theCalib->getEnergy( hit_channelID, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_timeBegin  );
				else if( fQdcCalibMethod == TOF_QdcCalibMethod::fLinear ) 
					hit_qdc_cal = 0; ////////////////////////////////
				else if( fQdcCalibMethod == TOF_QdcCalibMethod::fTiming ) 
					hit_qdc_cal =	TOF_TdcQdcCalibration::getInstance()->getCalibratedQDC( hit_channelID, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_tCoarse, hit_timeBegin  );
					//hit_qdc_cal =	theCalib->getCalibratedQDC( hit_channelID, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_tCoarse, hit_timeBegin  );
				else hit_qdc_cal = 0;
				
				channelBrElement.channelID = hit_channelID;
		    channelBrElement.frameID   = hit_frameID  ;
		    channelBrElement.tacID     = hit_tacID    ;
		    channelBrElement.tCoarse   = hit_tCoarse  ;
		    channelBrElement.eCoarse   = hit_eCoarse  ;
		    channelBrElement.tFine     = hit_tFine    ;
		    channelBrElement.eFine     = hit_eFine    ;
				channelBrElement.timeBegin = hit_timeBegin;
				channelBrElement.timeEnd   = hit_timeEnd  ;
				channelBrElement.qdc_cal   = hit_qdc_cal  ;

				vBranch.push_back( channelBrElement );
			}

			fTreeCoin->Fill();
		}
	}

	std::cout << "[TOF_CoincidenceEvents::getCoincidenceEvents] Completed" << std::endl;

	return fTreeCoin;
}

void TOF_CoincidenceEvents::reset()
{
  if(fTree) 
	{
    // fTree는 CloneTree()로 생성된 객체이므로 삭제해야 함
    fTree->ResetBranchAddresses(); // 이미 Clone된 객체에선 필요 없을 수 있음
    delete fTree;
    fTree=nullptr;
		//std::cout << "[TOF_CoincidenceEvents::reset()] fTree is reset" << std::endl;
  }
  
  vTree.clear();
		
	std::cout << "[TOF_CoincidenceEvents::reset()] Reset Completed" << std::endl;
};


TOF_CoincidenceEvents::~TOF_CoincidenceEvents()
{
	std::cout << "[TOF_CoincidenceEvents] destructor called" << std::endl;
	reset();
	if( this==theCoin ) theCoin=nullptr;
}
		
