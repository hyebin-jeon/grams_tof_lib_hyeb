
void pulseTest()
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

	const char* dirName    = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20260122_PulseTests/tofdata/"; 
	const char* subdirName = "stg1/ampl_1.5V";
	const char* finName    = "run_2026-01-22_22-22-37.717Z.stg1.root";


	//theCalib->readCalibrationFiles( "../calibration/20251217" ); 
	const char* tdc_suffix = "_2026-01-22_21-03-10.916Z";
	const char* qdc_suffix = "_2026-01-22_21-07-30.19Z" ;
	TString tdcPath = Form("%s/tdc/tdc_calibration%s.tsv", dirName, tdc_suffix);
	TString qdcPath = Form("%s/qdc/qdc_calibration%s.tsv", dirName, qdc_suffix);
  theCalib->readTdcCalib(tdcPath);
  theCalib->readQdcCalib(qdcPath);


	const char* pdf_suffix = "_20260122_1.5V";
  TString pdfName= Form("output/tofCoinEvt%s.pdf", pdf_suffix );
	
	const char* finPath = Form( "%s/%s/%s", dirName, subdirName, finName );
	TFile* fin = new TFile( finPath, "read" );
	if( !fin->IsOpen() ) {
		printf("[ERR] FILE NOT FOUND: %s\n", finPath);
		return;
	}
	
	/////////////////////
	/// ROI channel list
	uint8_t febD_connID = 4;
	std::vector<uint8_t>  smaChannels = { 1, 2, 65, 66, 20 }; // SMA connector IDs
	std::vector<uint32_t> activeChannels;
	for( auto chan: smaChannels )
	{
		auto achanID = theChanConv->getAbsoluteChannelID( febD_connID, chan );
		activeChannels.push_back( achanID );
	}

	std::sort( activeChannels.begin(), activeChannels.end() );
	for( auto chan: activeChannels ) cout << "channel: " << chan << endl;
	/////////////////////


	TOF_TreeData* tD = (TOF_TreeData*) fin->Get("data");
	if( !tD ) return;
		 
	tD->setBranchAddress();
	tD->Print();

	TH1D* hQf = new TH1D("hQf", "raw QDC (efine)", 100, 0, 0 );
	TH1D* hCh = new TH1D("hCh", "channel", 128, 128*(febD_connID-1), 128*febD_connID );
	TH1D* hTc = new TH1D("hTc", "Tcoarse", 100, 0, 0 );

	for( int i=0; i<tD->GetEntries(); i++ )
	{
		tD->GetEntry(i);

		//if( tD->getChannelID() != 442 ) continue; 

		hQf->Fill( tD->getEFine() );
		hCh->Fill( tD->getChannelID() );
		hTc->Fill( tD->getTCoarse() );

	}

	TCanvas* c1 = new TCanvas("c1","c1");
	hQf->Draw();
	TCanvas* c2 = new TCanvas("c2","c2");
	hCh->Draw();

	delete tD;

	

	//TFile *fout = new TFile( Form("output/tofCoinEvt%s.root", pdf_suffix), "recreate" );

}




