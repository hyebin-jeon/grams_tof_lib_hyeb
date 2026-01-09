/// v1

#include <filesystem>
namespace fs = std::filesystem;

void biasScan()
{
	/// Class setup
	//TOF_CoincidenceEvents* theCoin = new TOF_CoincidenceEvents(); //::getInstance();
  auto theChanConv = TOF_ChannelConversion::getInstance();
	auto theFit      = TOF_Fitting::getInstance();
  auto theAttrib   = TOF_Attributes::getInstance();
  auto theCalib    = TOF_TdcQdcCalibration::getInstance();
	theCalib->readCalibrationFiles( "../calibration/20250909" );

	std::string home = std::getenv("HOME");
	//std::string dirName = home+"/Box/GRAMS/TOF_MPD/0_TestOutputs/20250909_diffBiasVolt";
	std::string dirName = "../data/20250909";
	const char* finName = "test_qdc_raw.root";

	/// output pdf name 
	const char* pdf_suffix = "_0909";
	int qdcmode = -1;
	if( ((TString) finName).Contains("qdc") ) qdcmode = 1; //qdc
	else if( ((TString) finName).Contains("tot") ) qdcmode = 0; //tot
	else return;

	const char* mode_suffix = qdcmode==1? "_qdc":"_tot";
  const char* pdfName= Form("output/biasScan%s%s.pdf", mode_suffix, pdf_suffix );
	
	/// ROI channel list
	uint8_t febD_connID = 3;
	std::vector<uint8_t>  smaChannels = { 1, 2, 65, 66 }; // SMA connector IDs
	std::vector<uint32_t> activeChannels;
	for( auto chan: smaChannels )
	{
		auto achanID = theChanConv->getAbsoluteChannelID( febD_connID, chan );
		activeChannels.push_back( achanID );
	}
	std::sort( activeChannels.begin(), activeChannels.end() );

	//TOF_CoincidenceEvents* theCoin = TOF_CoincidenceEvents::getInstance();
	//theCoin->setActiveChannels( activeChannels );

	std::vector<TString> vSubDirName;
	for( const auto& entry: fs::directory_iterator(dirName) )
	{
		if( entry.is_directory())
		{
			TString subdir = (TString) entry.path().filename();
			if( subdir.Contains("Fan") ) continue;

			vSubDirName.push_back(subdir);
		}
	}
	
	std::vector< TFile* > vFile;
	vFile.clear();

	std::vector< TOF_CoincidenceEvents* > vCoinEvt;
	vCoinEvt.clear();
	int nb = vSubDirName.size();
	for( int i=0; i<nb; i++ ) {
		TOF_CoincidenceEvents* theCoin = new TOF_CoincidenceEvents();
		theCoin->SetUniqueID(i);
		theCoin->setActiveChannels( activeChannels );
		vCoinEvt.push_back( theCoin );
	}

	int i=0;
	TFile* fin{nullptr};
	for( auto subdir: vSubDirName )
	//for( int k=0; k<4; k++ )
	{
		//TString subdir = vSubDirName.at(0);
			
	  int vidx0 = subdir.Index("Vsense");
	  int vidx1 = vidx0 + 6;
	  double Vsense = ((TString) subdir(vidx1, 3)).Atof();

	  int measNo = 1;
	  if( subdir.Contains("1st")) measNo=1;
	  if( subdir.Contains("2nd")) measNo=2;
	  if( subdir.Contains("3rd")) measNo=3;
	 
		TString fPath = Form( "%s/%s/%s", dirName.c_str(), subdir.Data(), finName );
		cout << "["<<i<<"]" << fPath<< endl;
	  //cout << Form( "[%02d] %s ==> Vsense = %.1f, meas no.%d", i, subdir.Data(), Vsense, measNo) << endl;
		//cout << "=========================================================" << endl;

		fin = new TFile( fPath.Data(), "read" );
	  if( !fin->IsOpen() ) {
	  	printf("[ERR] FILE NOT FOUND: %s", fPath.Data());
	  	return;
	  }

		vFile.push_back( fin );
		i++;
	}

	TFile* fnew = new TFile("output/biasScan_new.root", "recreate");


	TOF_CoincidenceEvents* theCoin = nullptr;
	TOF_TreeData* tD{nullptr};
	//TTree* tC{nullptr};
	//std::vector<TOF_CoincidenceChannelInfo> vBranch;

	  std::vector<TOF_CoincidenceChannelInfo> vBranch;
	for( int j=0; j< vFile.size(); j++ )
	{
		//if( j>5 ) break;

		cout << "===========================================" << endl;
		cout << "[" << j << "] File: " << vFile.at(j)->GetName() << endl;

	  int vidx0 = ((TString) vFile.at(j)->GetName()).Index("Vsense");
	  int vidx1 = vidx0 + 6;
	  double Vsense = ((TString) ((TString) vFile.at(j)->GetName())(vidx1, 3)).Atof();

	  int measNo = 1;
	  if( ((TString) vFile.at(j)->GetName()).Contains("1st")) measNo=1;
	  if( ((TString) vFile.at(j)->GetName()).Contains("2nd")) measNo=2;
	  if( ((TString) vFile.at(j)->GetName()).Contains("3rd")) measNo=3;
	

		//TFile* ff = new TFile("../data/20250909/bias_Vset161_Vsense163/test_qdc_raw.root");


		cout << "[biasScan] OK0" << endl;
	  tD = (TOF_TreeData*) vFile.at(j)->Get("data");
	  //tD = (TOF_TreeData*) ff->Get("data");
		cout << "[biasScan] OK1" << endl;
	  if( !tD ) {
			delete vFile.at(j);
			continue;
		}

		//tD->SetDirectory( 0 ); // isolate tD from fin
		cout << "[biasScan] OK2" << endl;
		tD->SetName( Form("t_%03.0fV_%d", Vsense, measNo) );
		cout << "[biasScan] OK3" << endl;
	  
		/// class setup
	  //TOF_CoincidenceEvents* theCoin = new TOF_CoincidenceEvents();
	  theCoin = new TOF_CoincidenceEvents();
		cout << "[biasScan] OK4" << endl;
	  theCoin->setTreeData( tD );
		cout << "[biasScan] OK5" << endl;
		theCoin->setActiveChannels( activeChannels );
		cout << "[biasScan] OK6" << endl;
		
		tD->GetEntry(0);
		cout << "[biasScan] OK7" << endl;
		auto frameID      = tD->getFrameID();
		cout << "[biasScan] OK8" << endl;
		auto tCoarse      = tD->getTCoarse();
		cout << "[biasScan] OK9" << endl;
		auto absChannelID = tD->getChannelID(); // absolute channel ID
		cout << "[biasScan] OK10" << endl;
		std::cout << Form("==> frameID= %10lld, tCoarse= %10hu, channelID= %3u", frameID, tCoarse, absChannelID ) << std::endl;
		cout << "[biasScan] OK11" << endl;

	  auto vTree = theCoin->getCoincidenceEvents();
		cout << "[biasScan] OK12" << endl;

		//gDirectory->cd();
		fnew->cd();
		TTree* tC = new TTree(Form("t_%03.0fV_%d", Vsense, measNo), Form("t_%03.0fV_%d", Vsense, measNo) );
		cout << "[biasScan] OK13" << endl;
		vBranch.clear();
		cout << "[biasScan] OK14" << endl;

		tC->Branch("coinEvt",  &vBranch, 6400, 0 );
		cout << "[biasScan] OK15" << endl;

		//for( auto vBr : vTree )
		for( int k=0; k<vTree.size(); k++ )
		{
			vBranch = vTree.at(k);
			//vBranch.insert(vBranch.begin(), vBr.begin(), vBr.end());
			tC->Fill();
			//if( k>10) break;
		}
		tC->Write();

		cout << "[biasScan] OK16" << endl;
		//cout << "==> raw tree entries: " << tD->GetEntries() << ", extracted vector list size: " << vTree.size() << endl; 
		cout << "==> raw tree entries: " << tD->GetEntries() << ", extracted vector list size: " << vTree.size() << " ==> tC->GetEntries()= " << tC->GetEntries() << endl;


		/// clean up memories for the next loop turns
		cout << "[biasScan] OK17" << endl;
		tC=nullptr;
		delete tC;

		cout << "[biasScan] OK18" << endl;
		//delete tD;
		tD=nullptr;

		cout << "[biasScan] OK19" << endl;
		delete theCoin; // tD is deleted here
		theCoin=nullptr;

		//fin->Close();
		//delete fin;

		vBranch.clear();
		cout << "[biasScan] OK20" << endl;
		cout << "[biasScan] OK21" << endl;

		//vFile.at(j)->Close();
		
	}
}
