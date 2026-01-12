/// This code assumes that the number of 
void tofCoinEvt_20260111()
{
	/// PETSYS setup
  double frequency = 200E6; // 200 MHz - doublecheck it
  //double frequency = 160E6; // 160 MHz - based on PETSYS's JINST paper
  double Tsec = 1/frequency; // second
	double Tns  = Tsec/pow(10,-9); // nsec

	/// Class setup
	TOF_CoincidenceEvents* theCoin = new TOF_CoincidenceEvents(); //::getInstance();
  auto theChanConv = TOF_ChannelConversion::getInstance();
	auto theFit      = TOF_Fitting::getInstance();
  auto theAttrib   = TOF_Attributes::getInstance();

  auto theCalib    = TOF_TdcQdcCalibration::getInstance();
	//theCalib->readCalibrationFiles( "../calibration/20260109" ); 
	theCalib->readCalibrationFiles( "../calibration/20251217" ); 

	const char* dirName = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20251217_DistrBoard/20251218";
	//const char* dirName = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20260109_MergedSoftwareCheck/tofdata/stg1";

	const char* finName = "test_qdc_raw.root";
	//const char* finName = "run_2026-01-09_23-48-34.104Z.stg1.root";
	const char* finPath = Form( "%s/%s", dirName, finName );

	const char* pdf_suffix = "_20251218_test";

	int qdcmode = 1;
	cout << "qdcmode: " << qdcmode << endl;

	const char* mode_suffix = qdcmode==1? "_qdc":"_tot";
	cout << "[0]" << qdcmode << "," << mode_suffix << endl;
  //TString pdfName= Form("output/tofCoinEvt%s%s.pdf", mode_suffix, pdf_suffix );
  TString pdfName= Form("output/tofCoinEvt%s.pdf", pdf_suffix );

	cout << "[1]" << pdfName << endl;
	
	
	TFile* fin = new TFile( finPath, "read" );
	if( !fin->IsOpen() ) {
		printf("[ERR] FILE NOT FOUND: %s", finPath);
		return;
	}

	TOF_TreeData* tD = (TOF_TreeData*) fin->Get("data");
	if( !tD ) return;
	//tD->setBranchAddress();
	
 
	/// ROI channel list
	uint8_t febD_connID = 4;
	std::vector<uint8_t>  smaChannels = { 1, 2, 65, 66 }; // SMA connector IDs
	std::vector<uint32_t> activeChannels;
	for( auto chan: smaChannels )
	{
		auto achanID = theChanConv->getAbsoluteChannelID( febD_connID, chan );
		activeChannels.push_back( achanID );
	}

	std::sort( activeChannels.begin(), activeChannels.end() );
	for( auto chan: activeChannels ) cout << "channel: " << chan << endl;

	TFile *fout = new TFile( Form("output/tofCoinEvt%s.root", pdf_suffix), "recreate" );

	/// class setup
	auto ok = theCoin->setTreeData( tD ); cout << ok << endl;
	theCoin->setActiveChannels( activeChannels );
	//theCoin->setQdcCalibMethod( TOF_QdcCalibMethod::fGetEnergy );
	auto tC = theCoin->getCoincidenceEventsTree();

	tC->Print();
	tC->Write();

	//std::vector<TOF_CoincidenceChannelInfo>* vBranch = nullptr;
	//tC->SetBranchAddress( "coinEvt", &vBranch );

	theCoin->generateHistoForQA();

	theCoin->fHisto_dT     ->Write();
	theCoin->fHisto_NbOfEvt->Write();
	theCoin->fHisto_TvsQcal->Write();

	cout << "=================================" << endl;
	cout << "ouput: " << fout->GetName() << endl;
	cout << "=================================" << endl;

	fout->Close();
	


}



