/// Created by Hyebin Jeon
/// based on `plotraw3.C` wriiten by Makoto

/// This macro is an old one ==> use tofCoinevt*.C, instead

auto theFit      = TOF_Fitting::getInstance();
auto theAttrib   = TOF_Attributes::getInstance();
auto theCalib    = TOF_TdcQdcCalibration::getInstance();
auto theChanConv = TOF_ChannelConversion::getInstance();

int setClasses( const char* calib_path );

void plotCoincidence( uint8_t febD_connID = 3 )
{
	const int coincidenceT = 1; // clock window for coincidence events

	/// active channel list
	//uint8_t febD_connID = 3;
  uint8_t  activeChannel[4]    = { 1, 2, 65, 66 }; // SMA connector IDs

	uint8_t chan_trig = activeChannel[0];

	/// input file info
	//const char* dirName = "~/work/grams/20250815_newPaddle_and_paramScan";
	const char* dirName = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20250903_QDC_vs_ToT";
	const char* finName = "test_tot_raw.root";
	const char* finPath = Form( "%s/%s", dirName, finName );

	int qdcmode = -1;
	if( ((TString) finName).Contains("qdc") ) qdcmode = 1; //qdc
	else if( ((TString) finName).Contains("tot") ) qdcmode = 0; //tot
	else return;

	bool useGetEnergy = 1; // keep this to be 1 

  const char* pdfName{};
	if( qdcmode==1 ) pdfName= Form("output/plotCoincidence_qdc_%dclocks%s_0903.pdf", coincidenceT, useGetEnergy? "":"_newQcal");
	else pdfName= Form("output/plotCoincidence_tot_%dclocks%s_0903_2.pdf", coincidenceT, useGetEnergy? "":"_newQcal");
	
	if( setClasses( "../calibration/20251217" ) < 0 ) return;

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

	std::vector<uint32_t> activeChannelAbs; // absolute channel IDs of the actiave channels
	bool hitChannel[4];
	for( int i=0; i<4; i++ )
	{
		//activeChannelAbs[i] = theChanConv->getAbsoluteChannelID( febD_connID, activeChannel[i] );
		auto achanID = theChanConv->getAbsoluteChannelID( febD_connID, activeChannel[i] );
		activeChannelAbs.push_back( achanID );
		cout << "active channel (absolute) ID: " << activeChannelAbs[i] << endl;
		hitChannel[i] = 0; // initialization
	}
	std::sort( activeChannelAbs.begin(), activeChannelAbs.end() );

	/// PETSYS setup
  double frequency = 200E6; // 200 MHz - doublecheck it
  //double frequency = 160E6; // 160 MHz - based on PETSYS's JINST paper
  double Tsec = 1/frequency; // second
	double Tns  = Tsec/pow(10,-9); // nsec

	TH1D* hQDC[4];
	TH1D* hQDC_Calib[4];
	TH1D* hTdiff[2]; // time diff between two sides of a paddle
	TH1D* hQdiff[2]; // charge diff between two sides of a paddle
	TH1D* hTratio[2]; // time asym ratio between two sides of a paddle
	TH1D* hQratio[2]; // charge asym ratio between two sides of a paddle
	TH1D* hQCalratio[2]; // calibarated QDC's ratio
	//TH1D* h_dT = new TH1D("h_dT", "Time Diff between Paddles", 300, -15000, 15000 ); // psec
	TH1D* h_dT = new TH1D("h_dT", "Time Diff between Paddles;Asym. time ratio;", 300, -15, 15 ); // nsec
	TH1D* h_dQ[4]; // = new TH1D("h_dQ", ";Qexpected - efine;", 100, -40, 10 );

	TH1D* h_scale = new TH1D("h_scale", "", 100, -10, 10 );

	TH2D* hQvsQcal = useGetEnergy? 
		               new TH2D("hQvsQcal", "All Channels;QDC;Calibrated QDC", 200, qdcmode? 100:180,  qdcmode? 300:400, 100, qdcmode? 0:-50, qdcmode? 30:120 ):
									 new TH2D("hQvsQcal", "All Channels;QDC;Calibrated QDC", 150, 100, 250, 100, 85, 185 );
	TH2D* hTvsQraw = useGetEnergy?
		               new TH2D("hTvsQraw", ""     , 300, -3, 3, qdcmode? 250:200, qdcmode? -0.25:-1, qdcmode? 0:1):
									 new TH2D("hTvsQraw", ""     , 300, -3, 3, 100, 0, 0.25);
	TH2D* hTvsQcal = useGetEnergy?
		               new TH2D("hTvsQcal", ";Time diff in clock;Asym. ratio of calib'ed Q", 300, -3, 3, qdcmode? 120:200, qdcmode? -0.6:-1, qdcmode? 0.6:1):
									 new TH2D("hTvsQcal", ";Time diff in clock;Asym. ratio of calib'ed Q", 300, -3, 3, 100, 0, 0.25);

	hTvsQraw->GetXaxis()->SetTitle("dT btw two ends of a paddle");
	hTvsQcal->GetXaxis()->SetTitle("dT btw two ends of a paddle");

	if(qdcmode) {
	  hQvsQcal->GetXaxis()->SetTitle( "Raw QDC (=efine)" );
	  hQvsQcal->GetYaxis()->SetTitle( useGetEnergy? "Calib QDC (=getEnergy)":"Calib QDC using g(t)" );
	  hTvsQraw->GetYaxis()->SetTitle( "Asym. ratio of QDC (efine)" );
	  hTvsQcal->GetYaxis()->SetTitle( useGetEnergy? "Asym. ratio of calib QDC (getEnergy)":"Asym. ratio of calib QDC using g(t)" );
	} else {
	  hQvsQcal->GetXaxis()->SetTitle( "Time_end (=efine)" );
	  hQvsQcal->GetYaxis()->SetTitle( "Time of charge integration" );
	  hTvsQraw->GetYaxis()->SetTitle( "Time_end (=efine)" );
	  hTvsQcal->GetYaxis()->SetTitle( "Asym. ratio of charge integ time" );
	}


	for( int i=0; i<4; i++ )
	{
		hQDC[i]  = new TH1D( Form("hEfine_ch%03d" , activeChannelAbs[i]), "", 200, qdcmode? 100:180,  qdcmode? 300:400 ); // QDC max = 400 LSB
		hQDC_Calib[i]= useGetEnergy? 
			             new TH1D( Form("hEfine_calib_ch%03d" , activeChannelAbs[i]), "", 100, qdcmode? -2:-50, qdcmode? 32:200 ): // QDC max = 400 LSB
									 new TH1D( Form("hEfine_calib_ch%03d" , activeChannelAbs[i]), "", 100, 80, 180 ); // QDC max = 400 LSB
	
		h_dQ[i] = new TH1D( Form("h_dQ_ch%03d", activeChannelAbs[i]), "", qdcmode? 70:150, qdcmode? -50:-420, qdcmode? 20:-120 );
		theAttrib->attribHist( h_dQ[i], 1+i, 0.3 );

		if( qdcmode ) {
			hQDC[i]      ->GetXaxis()->SetTitle( "QDC (=efine)" );
			hQDC_Calib[i]->GetXaxis()->SetTitle( "Calib QDC (=getEnergy)" );
			h_dQ[i]      ->GetXaxis()->SetTitle( "calib QDC - raw QDC" );
		} else {
			hQDC[i]      ->GetXaxis()->SetTitle( "Time_end (=efine)" );
			hQDC_Calib[i]->GetXaxis()->SetTitle( "Time of charge integration" );
			h_dQ[i]      ->GetXaxis()->SetTitle( "t_integ - t_end" );
		}
	}
	for( int i=0; i<2; i++ )
	{
		int idx0 = i*2;
		int idx1 = idx0+1;
		
		hTdiff [i]    = new TH1D( Form("hTdiff_ch%03d-ch%03d"    , activeChannelAbs[idx0], activeChannelAbs[idx1]), "", 100, 0, 0 ); 
		hQdiff [i]    = new TH1D( Form("hQdiff_ch%03d-ch%03d"    , activeChannelAbs[idx0], activeChannelAbs[idx1]), "", 100, qdcmode? -100:-200, qdcmode? 100:200 );
		hTratio[i]    = new TH1D( Form("hTratio_ch%03d-ch%03d"   , activeChannelAbs[idx0], activeChannelAbs[idx1]), "", 100, -1, 1 ); 
		hQratio[i]    = new TH1D( Form("hQratio_ch%03d-ch%03d"   , activeChannelAbs[idx0], activeChannelAbs[idx1]), "", 100, -1, 1 );
		hQCalratio[i] = new TH1D( Form("hQCalRatio_ch%03d-ch%03d", activeChannelAbs[idx0], activeChannelAbs[idx1]), "", 100, -1, 1 );
		  
		hTdiff [i]->GetXaxis()->SetTitle( "time diff btw two ends of a paddle (in clocks)" );
		hTratio[i]   ->GetXaxis()->SetTitle( "(t0-t1)/(t0+t1)" );
	
		if( qdcmode ) {
		  hQdiff [i]   ->GetXaxis()->SetTitle( "QDC (efine) diff btw two ends of a paddle (ADC)" );
		  hQratio[i]   ->GetXaxis()->SetTitle( "(q0-q1)/(q0+q1)" );
		  hQCalratio[i]->GetXaxis()->SetTitle( "(q0-q1)/(q0+q1) using calibrated QDC");
		} else {
		  hQdiff [i]   ->GetXaxis()->SetTitle( "t_end (efine) diff btw two ends of a paddle (ADC)" );
		  hQratio[i]   ->GetXaxis()->SetTitle( "(t_end0-t_end1)/(t_end0+t_end1)" );
		  hQCalratio[i]->GetXaxis()->SetTitle( "(t_int0-t_int1)/(t_int0+t_int1), t_int=integ. time");
		}
	}

	long long currTime= -1; // 64-bit signed int

	/// the first frame ID
	t->GetEntry(0);
	long long frameID0= t->getFrameID(); 
	long long lastTime= CLOCKS_IN_A_FRAME * frameID0 + t->getTCoarse(); 	

	cout << "initial time: " << lastTime << endl;

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

		//cout << Form("[Every] currTime: %10lld, frameID= %10lld, tCoarse= %10hu, tFine= %10hu,channelID= %3d", currTime, frameID, t->getTCoarse(), t->getTFine(), channelID ) << endl;
			

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

				auto hit_time     = theCalib->getCalibratedTime( TOF_Branch::fBranchT, hit_channel, hit_tacID, hit_frameID, hit_tCoarse, hit_tFine );
				auto hit_time_end = theCalib->getCalibratedTime( TOF_Branch::fBranchE, hit_channel, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine );
				auto hit_qdc_cal = useGetEnergy? 
													 theCalib->getEnergy( hit_channel, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_time  ):
					                 theCalib->getCalibratedQDC( hit_channel, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_tCoarse, hit_time  );

				if( qdcmode ) hit_qdc_cal = hit_qdc_cal;
				else hit_qdc_cal = hit_time_end - hit_time;

				//cout << Form( "QDC= %5d --> QDC_calibrated= %6d", hit_eFine, hit_qdc_cal ) << endl;
		    //cout << Form("[Coinc] currTime: %10lld, frameID= %10lld, tCoarse= %10hu, tFine= %10hu, channelID= %3d, time= %8.1f, eFine= %d, QDC_calib= %6.1f", currTime, hit_frameID, hit_tCoarse, hit_tFine, hit_channel, hit_time, hit_eFine, hit_qdc_cal  ) << endl;

				qdc[i]     = hit_eFine;
				time[i]    = hit_time;
				qdc_cal[i] = hit_qdc_cal;
				
	      hQDC[i]      ->Fill( hit_eFine );
				hQDC_Calib[i]->Fill( hit_qdc_cal );
				h_dQ[i]      ->Fill( hit_qdc_cal - hit_eFine );
				hQvsQcal     ->Fill( hit_eFine, hit_qdc_cal );
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
				
				hTvsQraw->Fill( tdiff, qratio );
				hTvsQcal->Fill( tdiff, qratio_cal );
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
	h_dT->Draw();
	gPad->Modified();
	gPad->Update();
	auto ps = (TPaveStats*) gPad->GetPrimitive("stats");
	if( h_dT->GetMean() > 0 ) 
	  theAttrib->moveStatBoxNDC( ps, 0.12, 0.55, 0.40, 0.90 );
	else
	  theAttrib->moveStatBoxNDC( ps, 0.62, 0.55, 0.90, 0.90 );

	c01->Print( Form("%s(", pdfName) );
	c00->Print( pdfName );
	c02->Print( pdfName );
	c03->Print( pdfName );
	c04->Print( pdfName );

	gStyle->SetOptStat(0);
	TCanvas* c05 = new TCanvas( "c05", "c05" );
	hQvsQcal->Draw("colz");

	TCanvas* c06 = new TCanvas( "c06", "c06" );
	hTvsQcal->Draw("colz");
	gPad->SetGrid();
	auto correl = hTvsQcal->GetCorrelationFactor();
	theAttrib->drawTextNDC( 0.05, 12, kBlack, 0.15, 0.85, Form("correl= %.2f", correl) );

	TCanvas* c07 = new TCanvas( "c07", "c07" );
	hTvsQraw->Draw("colz");
	gPad->SetGrid();
	auto correlR = hTvsQraw->GetCorrelationFactor();
	theAttrib->drawTextNDC( 0.05, 12, kBlack, 0.15, 0.85, Form("correl= %.2f", correlR) );

	TCanvas* c08 = new TCanvas( "c08", "c08" );
	double h_dQ_max = 0;
	for( int i=0; i<4; i++ ) 
	{
		auto max = h_dQ[i]->GetMaximum();
		if( h_dQ_max < max ) h_dQ_max = max;
	}
	h_dQ[0]->SetMaximum( h_dQ_max + 20);
	h_dQ[0]->Draw();
	h_dQ[1]->Draw("same");
	h_dQ[2]->Draw("same");
	h_dQ[3]->Draw("same");
	auto leg = gPad->BuildLegend( 0.7, 0.65, 0.9, 0.9);
	theAttrib->setAttribLegendEntries( leg, "line");

	

	c05->Print( pdfName );
	c06->Print( pdfName );
	c07->Print( pdfName );
	c08->Print( Form("%s)", pdfName) );

	cout << "=================================" << endl;
	cout << "ouput: " << pdfName << endl;
	cout << "=================================" << endl;

}

int setClasses( const char* calib_path )
{
	//const char* calib_path = "../calibration/20250815_GSFC";
	const char* tdc_calib = Form( "%s/tdc_calibration.tsv", calib_path );
	const char* qdc_calib = Form( "%s/qdc_calibration.tsv", calib_path );
	auto ok = theCalib->readCalibrationFiles( tdc_calib, qdc_calib );

	return ok;
}

