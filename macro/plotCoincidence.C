/// Created by Hyebin Jeon
/// based on `plotraw3.C` wriiten by Makoto

auto theCalib = TOF_TdcQdcCalibration::getInstance();
auto theChanConv = TOF_ChannelConversion::getInstance();
auto theFit = TOF_Fitting::getInstance();

void setClasses();

void plotCoincidence( uint8_t febD_connID = 3 )
{
	setClasses();

	const int coincidenceT = 1; // clock window for coincidence events


	/// active channel list
	//uint8_t febD_connID = 3;
  uint8_t  activeChannel[4]    = { 1, 2, 65, 66 }; // SMA connector IDs

	uint8_t chan_trig = activeChannel[0];

	/// input file info
	const char* dirName = "~/work/grams/20250815_newPaddle_and_paramScan";
	const char* finName = "test_qdc_raw.root";
	const char* finPath = Form( "%s/%s", dirName, finName );

	TFile* fin = new TFile( finPath, "read" );
	if( !fin->IsOpen() ) {
		printf("[ERR] FILE NOT FOUND: %s", finPath);
		return;
	}

	TOF_TreeData* t = (TOF_TreeData*) fin->Get("data");
	if( !t ) return;

	/// SetBranchAddress
	t->setBranchAddress();
	int entries = t->GetEntries();

	cout << "entries: " << entries << endl;

	/// PETSYS setup
  double frequency = 200E6; // 200 MHz - doublecheck it
  //double frequency = 160E6; // 160 MHz - based on PETSYS's JINST paper
  double Tsec = 1/frequency; // second
	double Tns  = Tsec/pow(10,-9); // nsec

	TH1D* hTDC[4];
	TH1D* hTime[4];
	TH1D* hQDC[4];
	TH1D* hQDC_Calib[4];
	TH1D* hTdiff[2]; // time diff between two sides of a paddle
	TH1D* hQdiff[2]; // charge diff between two sides of a paddle
	TH1D* hTratio[2]; // time asym ratio between two sides of a paddle
	TH1D* hQratio[2]; // charge asym ratio between two sides of a paddle
	TH1D* hQCalratio[2]; // calibarated QDC's ratio
	//TH1D* h_dT = new TH1D("h_dT", "Time Diff between Paddles", 300, -15000, 15000 ); // psec
	TH1D* h_dT = new TH1D("h_dT", "Time Diff between Paddles;Asym. time ratio;", 300, -15, 15 ); // nsec

	for( int i=0; i<4; i++ )
	{
		hTDC[i]  = new TH1D( Form("hTDC_ch%03d" , activeChannel[i]), "", 200, 0, 0 ); // TDC max = 400 LSB
		hTime[i] = new TH1D( Form("hTime_ch%03d", activeChannel[i]), "", 500, -15000, 15000 ); // TDC max = 400 LSB
		hQDC[i]  = new TH1D( Form("hQDC_ch%03d" , activeChannel[i]), "", 100, 50,  350 ); // QDC max = 400 LSB
		hQDC_Calib[i]= new TH1D( Form("hQDC_calib_ch%03d" , activeChannel[i]), "", 100, 0,  20 ); // QDC max = 400 LSB
	}
	for( int i=0; i<2; i++ )
	{
		int idx0 = i*2;
		int idx1 = idx0+1;
		
		hTdiff [i]    = new TH1D( Form("hTdiff_ch%03d-ch%03d"    , activeChannel[idx0], activeChannel[idx1]), "", 100, -10, 10 ); 
		hQdiff [i]    = new TH1D( Form("hQdiff_ch%03d-ch%03d"    , activeChannel[idx0], activeChannel[idx1]), "", 100, -50, 50 );
		hTratio[i]    = new TH1D( Form("hTratio_ch%03d-ch%03d"   , activeChannel[idx0], activeChannel[idx1]), "", 200, -1, 1 ); 
		hQratio[i]    = new TH1D( Form("hQratio_ch%03d-ch%03d"   , activeChannel[idx0], activeChannel[idx1]), "", 200, -1, 1 );
		hQCalratio[i] = new TH1D( Form("hQCalRatio_ch%03d-ch%03d", activeChannel[idx0], activeChannel[idx1]), "", 200, -1, 1 );
	}

	long long currTime= -1; // 64-bit signed int

	/// the first frame ID
	t->GetEntry(0);
	long long frameID0= t->getFrameID(); 
	long long lastTime= CLOCKS_IN_A_FRAME * frameID0 + t->getTCoarse(); 	

	cout << "initial time: " << lastTime << endl;

	uint32_t activeChannelAbs[4]; // absolute channel IDs of the actiave channels
	bool hitChannel[4];
	for( int i=0; i<4; i++ )
	{
		activeChannelAbs[i] = theChanConv->getAbsoluteChannelID( febD_connID, activeChannel[i] );
		cout << "active channel (absolute) ID: " << activeChannelAbs[i] << endl;
		hitChannel[i] = 0; // initialization
	}

	TOF_Struct::TOF_TdcQdc channelInfo{};
	std::map< uint32_t, TOF_Struct::TOF_TdcQdc> vChannelData;
	vChannelData.clear();

	/// Fill the histograms
	for( int i=0; i<entries; i++ )
	{
		t->GetEntry(i);

		auto frameID      = t->getFrameID();
		auto tCoarse      = t->getTCoarse();
		auto absChannelID = t->getChannelID(); // absolute channel ID

		auto connID    = t->getConnIdOnFebD   ( absChannelID );
		auto channelID = t->getChannelIdOnFebS( absChannelID );  // 1-128
		auto asicID    = t->getAsicID         ( absChannelID );

		channelInfo.frameID = frameID;
		channelInfo.tacID   = t->getTacID();
		channelInfo.tCoarse = t->getTCoarse();
		channelInfo.eCoarse = t->getECoarse();
		channelInfo.tFine   = t->getTFine()  ;
		channelInfo.eFine   = t->getEFine()  ;


		/// current time in clock
		currTime = CLOCKS_IN_A_FRAME * frameID + tCoarse;
			
		/// coincidence events within 'coincidenceT' clocks
		if( fabs(currTime - lastTime) > coincidenceT) 
		{
			vChannelData.clear();
		  for( int i=0; i<4; i++ ) hitChannel[i] = 0;
		}

		 for( int i=0; i<4; i++ )
		 {
		   if( absChannelID == activeChannelAbs[i] )
		   {
		     hitChannel[i] = true;
		     vChannelData[absChannelID] = channelInfo;
		 	}
		 }

		cout << Form("[Every] currTime: %10lld, frameID= %10lld, tCoarse= %10hu, tFine= %10hu,channelID= %3d", currTime, frameID, t->getTCoarse(), t->getTFine(), channelID ) << endl;
			

		if( vChannelData.size() == 1 ) lastTime = currTime;
		
		bool good = hitChannel[0] && hitChannel[1] && hitChannel[2] && hitChannel[3];
		if( vChannelData.size() == 4 && good )
		{
			double qdc[4]     = {-999, -999, -999, -999};
			double time[4]    = {-999, -999, -999, -999};
			double qdc_cal[4] = {-999, -999, -999, -999};
			for( int i=0; i<4; i++ )
			{
				auto hit_channel = activeChannelAbs[i];
		    auto hit_frameID = vChannelData[activeChannelAbs[i]].frameID;
		    auto hit_tacID   = vChannelData[activeChannelAbs[i]].tacID  ;
		    auto hit_tCoarse = vChannelData[activeChannelAbs[i]].tCoarse;
		    auto hit_eCoarse = vChannelData[activeChannelAbs[i]].eCoarse;
		    auto hit_tFine   = vChannelData[activeChannelAbs[i]].tFine  ;
		    auto hit_eFine   = vChannelData[activeChannelAbs[i]].eFine  ;

				auto hit_time = theCalib->getCalibratedTime( TOF_Mode::fQdc, hit_channel, hit_tacID, hit_frameID, hit_tCoarse, hit_tFine );
				auto hit_qdc_cal = theCalib->getCalibratedQDC( hit_channel, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_time  );

				//hTime[i]->Fill( hit_time*Tsec );
				//hTDC[i] ->Fill( hit_time  );
	      hQDC[i] ->Fill( hit_eFine );
				hQDC_Calib[i]->Fill( hit_qdc_cal );

				//cout << Form( "QDC= %5d --> QDC_calibrated= %6d", hit_eFine, hit_qdc_cal ) << endl;
		    cout << Form("[Coinc] currTime: %10lld, frameID= %10lld, tCoarse= %10hu, tFine= %10hu, channelID= %3d, time= %8.1f, eFine= %d, QDC_calib= %6.1f", currTime, hit_frameID, hit_tCoarse, hit_tFine, hit_channel, hit_time, hit_eFine, hit_qdc_cal  ) << endl;

				qdc[i]     = hit_eFine;
				time[i]    = hit_time;
				qdc_cal[i] = hit_qdc_cal;
			}
	
			for( int i=0; i<2; i++ )
			{
				int idx0 = i*2;
				int idx1 = idx0+1;
				double qratio = (qdc [idx0] - qdc [idx1])/(qdc [idx0] + qdc [idx1]);
				double tratio = (time[idx0] - time[idx1])/(time[idx0] + time[idx1]);
				double qdiff  = qdc [idx0] - qdc [idx1];
				double tdiff  = time[idx0] - time[idx1];

			  hTratio[i]->Fill( tratio ); 
				hQratio[i]->Fill( qratio );
			  hTdiff [i]->Fill( tdiff  ); 
				hQdiff [i]->Fill( qdiff  );

				double qratio_cal = (qdc_cal[idx0] - qdc_cal[idx1])/(qdc_cal[idx0] + qdc_cal[idx1]);
				hQCalratio[i]->Fill( qratio_cal );
			}
	       
			double dT = ((time[0]-time[2]) + (time[1]-time[3]))/2.0 * Tns;
			//double dT = ((etime[0]-etime[2]) + (etime[1]-etime[3]))/2.0;
			h_dT->Fill( dT );
		}
		//if( i>100 ) break;
	}

	/// Individual channels' time and qdc
	TCanvas* c00 = new TCanvas( "c00", "QDC_calib_allChannels", 800, 600 );
	TCanvas* c01 = new TCanvas( "c01", "QDC_allChannels", 800, 600 );
	TCanvas* c02 = new TCanvas( "c02", "Tdiff_A_Paddle" , 800, 600 );
	TCanvas* c03 = new TCanvas( "c03", "Tratio_A_Paddle", 800, 600 );
	c00->Divide( 2,2 );
	c01->Divide( 2,2 );
	c02->Divide( 2,2 );
	c03->Divide( 2,2 );
	gStyle->SetOptStat(111111);
	for( int i=0; i<4; i++ )
	{
	  c01->cd( i+1 );
		if( hQDC[i] ) hQDC[i]->Draw();
	  
		c00->cd( i+1 );
		if( hQDC_Calib[i] ) hQDC_Calib[i]->Draw();

		if( i<2 ) {
	    c02->cd( i+1 );
			if( hTdiff[i%2]  ) hTdiff[i%2]->Draw();
		  c03->cd( i+1 );
			//if( hTratio[i%2] ) hTratio[i%2]->Draw();
			if( hQCalratio[i%2] ) hQCalratio[i%2]->Draw();

		}
		else {
	    c02->cd( i+1 );
			if( hQdiff[i%2]  ) hQdiff[i%2]->Draw();
		  c03->cd( i+1 );
			if( hQratio[i%2] ) hQratio[i%2]->Draw();
		}
	}


	gStyle->SetOptFit(1111);
	TCanvas* c04 = new TCanvas( "c04", "Time Resolution of Two Paddles" );
	theFit->fitGauss( h_dT, 2.5 );
	auto fit = theFit->getFitFunction();
	if( h_dT ) h_dT->Draw();

  const char* pdfName = Form("output/plotCoincidence_%dclocks.pdf", coincidenceT);
	c01->Print( Form("%s(", pdfName) );
	c00->Print( pdfName );
	c02->Print( pdfName );
	c03->Print( pdfName );
	c04->Print( Form("%s)", pdfName) );

}

void setClasses()
{
	const char* calib_path = "../calibration/20250815_GSFC";
	const char* tdc_calib = Form( "%s/tdc_calibration.tsv", calib_path );
	const char* qdc_calib = Form( "%s/qdc_calibration.tsv", calib_path );
	theCalib->readCalibrationFiles( tdc_calib, qdc_calib );
}

