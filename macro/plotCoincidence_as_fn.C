/// Created by Hyebin Jeon
/// based on `plotraw3.C` wriiten by Makoto

std::map< std::tuple<uint32_t, uint8_t>, std::tuple<double,double> > kFitParamsC; // coincidence
void setFitParamsC( std::map< std::tuple<uint32_t, uint8_t>, std::tuple<double,double> > list )
{
	kFitParamsC = list;
};

void plotCoincidence_as_fn( TOF_TreeData*  t, std::vector<int> absChannelList )
{
	if( !t ) { 
		cout << "[plotCoincidence_as_fn] t does NOT exist. Exit" << endl;
		return;
	}

	const int coincidenceT = 1; // clock window for coincidence events

	/// active channel list
	uint8_t febD_connID = 3;
  uint8_t activeChannel[4]    = { 1, 2, 65, 66 }; // SMA connector IDs

  const char* pdfName = "output/calibFactors3_plotCoin.pdf";

	int entries = t->GetEntries();

	cout << "entries: " << entries << endl;

	bool hitChannel[4];
  //std::vector< int > absChannelList;
	for( int i=0; i<4; i++ )
	{
		hitChannel[i] = 0; // initialization
		//absChannelList.push_back( (int) theChanConv->getAbsoluteChannelID(febD_connID, activeChannel[i]) );
		//cout << "active channel (absolute) ID: " << activeChannelAbs[i] << endl;
	}

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
	TH1D* h_dT = new TH1D("h_dT", "Time Diff between Paddles;Asym. time ratio;", 300, -15, 15 ); // nsec
	TH1D* h_dQ[4]; 
	TH1D* h_scale = new TH1D("h_scale", "", 80, 0, 0.4 );
	h_scale->GetXaxis()->SetTitle( "getEnergy()/Linear fit correction" );

	TH2D* hQvsQcal = new TH2D("hQvsQcal", "All Channels;QDC;Calibrated QDC", 200, 100, 300, 170, -20, 150 );
	TH2D* hTvsQraw = new TH2D("hTvsQraw", ";Time diff in clock;Asym. ratio of raw Q"     , 300, -3, 3, 100, -0.25, 0);
	TH2D* hTvsQcal = new TH2D("hTvsQcal", ";Time diff in clock;Asym. ratio of calib'ed Q", 300, -3, 3, 100, -0.6, 0.6);

	for( int i=0; i<4; i++ )
	{
		hTDC[i]  = new TH1D( Form("hTDC_ch%03d"   , absChannelList[i]), "", 200, 0, 0 ); // TDC max = 400 LSB
		hTime[i] = new TH1D( Form("hTime_ch%03d"  , absChannelList[i]), "", 500, 0, 0 ); // TDC max = 400 LSB
		hQDC[i]  = new TH1D( Form("hEfine_ch%03d" , absChannelList[i]), "", 200, 100, 300 ); // QDC max = 400 LSB
		hQDC_Calib[i]= new TH1D( Form("hEfine_calib_ch%03d" , absChannelList[i]), "", 170, -20, 150 ); // QDC max = 400 LSB
		h_dQ[i] = new TH1D( Form("h_dQ_ch%03d", absChannelList[i]), ";Qexpected - efine;", 200, -200, 0 );
		theAttrib->attribHist( h_dQ[i], 1+i, 0.3 );

		hTDC[i] ->GetXaxis()->SetTitle( "TDC" );
		hTime[i]->GetXaxis()->SetTitle( "Time" );
		hQDC[i] ->GetXaxis()->SetTitle( "efine" );
		hQDC_Calib[i]->GetXaxis()->SetTitle( "calibrated efine");
	}
	for( int i=0; i<2; i++ )
	{
		int idx0 = i*2;
		int idx1 = idx0+1;
		
		hTdiff [i]    = new TH1D( Form("hTdiff_ch%03d-ch%03d"    , absChannelList[idx0], absChannelList[idx1]), "", 100,  0, 0 ); 
		hQdiff [i]    = new TH1D( Form("hQdiff_ch%03d-ch%03d"    , absChannelList[idx0], absChannelList[idx1]), "", 100,  -100, 0 );
		hTratio[i]    = new TH1D( Form("hTratio_ch%03d-ch%03d"   , absChannelList[idx0], absChannelList[idx1]), "", 100, -1, 1 ); 
		hQratio[i]    = new TH1D( Form("hQratio_ch%03d-ch%03d"   , absChannelList[idx0], absChannelList[idx1]), "", 100, -1, 1 );
		hQCalratio[i] = new TH1D( Form("hQCalRatio_ch%03d-ch%03d", absChannelList[idx0], absChannelList[idx1]), "", 100, -1, 1 );
		
		hTdiff [i]   ->GetXaxis()->SetTitle( "time diff btw two ends of a paddle (in clocks)" );
		hQdiff [i]   ->GetXaxis()->SetTitle( "QDC diff btw two ends of a paddle (ADC)" );
		hTratio[i]   ->GetXaxis()->SetTitle( "(t0-t1)/(t0+t1)" );
		hQratio[i]   ->GetXaxis()->SetTitle( "(q0-q1)/(q0+q1)" );
		hQCalratio[i]->GetXaxis()->SetTitle( "(q0-q1)/(q0+q1) using calibrated QDC");
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
		   if( absChannelID == absChannelList[i] )
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
				auto hit_channel = absChannelList[i];
		    auto hit_frameID = vChannelData[absChannelList[i]].frameID;
		    auto hit_tacID   = vChannelData[absChannelList[i]].tacID  ;
		    auto hit_tCoarse = vChannelData[absChannelList[i]].tCoarse;
		    auto hit_eCoarse = vChannelData[absChannelList[i]].eCoarse;
		    auto hit_tFine   = vChannelData[absChannelList[i]].tFine  ;
		    auto hit_eFine   = vChannelData[absChannelList[i]].eFine  ;

				auto hit_time = theCalib->getCalibratedTime( TOF_Mode::fQdc, hit_channel, hit_tacID, hit_frameID, hit_tCoarse, hit_tFine );
				//auto hit_qdc_cal = useGetEnergy? 
				//									 theCalib->getEnergy( hit_channel, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_time  ):
				//	                 theCalib->getCalibratedQDC( hit_channel, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_tCoarse, hit_time  );
				auto hit_qdc_cal_getE = theCalib->getEnergy( hit_channel, hit_tacID, hit_frameID, hit_eCoarse, hit_eFine, hit_time );

				auto idx = std::tuple( hit_channel, hit_tacID );
				auto p0 = std::get<0>( kFitParamsC[idx] );
				auto p1 = std::get<1>( kFitParamsC[idx] );

	      if((hit_eCoarse - hit_tCoarse) < -256) hit_eCoarse += 1024;
        double timeEnd = double(hit_frameID*1024 + hit_eCoarse);
        float ti = (timeEnd - hit_time);
				auto hit_qdc_cal = hit_eFine - p1*ti - p0;

				//hTime[i]->Fill( hit_time*Tsec );
				//hTDC[i] ->Fill( hit_time  );
	      hQDC[i] ->Fill( hit_eFine );
				hQDC_Calib[i]->Fill( hit_qdc_cal );

				//cout << Form( "QDC= %5d --> QDC_calibrated= %6d", hit_eFine, hit_qdc_cal ) << endl;
		    //cout << Form("[Coinc] currTime: %10lld, frameID= %10lld, tCoarse= %10hu, tFine= %10hu, channelID= %3d, time= %8.1f, eFine= %d, QDC_calib= %6.1f", currTime, hit_frameID, hit_tCoarse, hit_tFine, hit_channel, hit_time, hit_eFine, hit_qdc_cal  ) << endl;
		    //cout << Form("[Coinc] frameID= %10lld, tCoarse= %5hu, tFine= %5hu, channelID= %3d, time= %8.1f, eFine= %d, QDC_calib= %6.1f, p0= %.1f, p1= %.1f", hit_frameID, hit_tCoarse, hit_tFine, hit_channel, hit_time, hit_eFine, hit_qdc_cal, p0, p1 ) << endl;

				qdc[i]     = hit_eFine;
				time[i]    = hit_time;
				qdc_cal[i] = hit_qdc_cal;
				
				h_dQ[i] ->Fill( hit_qdc_cal - hit_eFine );
				hQvsQcal->Fill( hit_eFine, hit_qdc_cal );

				h_scale->Fill( hit_qdc_cal_getE/hit_qdc_cal );
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
		//if( i>200 ) break;
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

	//gStyle->SetOptStat(1111);
	TCanvas* c08 = new TCanvas( "c08", "c08" );
	double h_dQ_max = 0;
	for( int i=0; i<4; i++ ) 
	{
		auto max = h_dQ[i]->GetMaximum();
		if( h_dQ_max < max ) h_dQ_max = max;
	}
	h_dQ[0]->SetMaximum(h_dQ_max + 20);
	h_dQ[0]->Draw();
	h_dQ[1]->Draw("same");
	h_dQ[2]->Draw("same");
	h_dQ[3]->Draw("same");
	auto leg = gPad->BuildLegend( 0.7, 0.65, 0.9, 0.9);
	theAttrib->setAttribLegendEntries( leg, "line");

	for( int i=0; i<4; i++ )
	{
		double tsize= 0.03;
		double x0 = 0.57+0.15*(i%2);
		double y0 = 0.37-0.1*(i/2);
		theAttrib->drawTextNDC( tsize, 11, h_dQ[i]->GetLineColor(), x0, y0-tsize*0, Form("%-8s=%-5.1f", "entries", h_dQ[i]->GetEntries()) );
		theAttrib->drawTextNDC( tsize, 11, h_dQ[i]->GetLineColor(), x0, y0-tsize*1, Form("%-8s=%-5.1f", "mean"   , h_dQ[i]->GetMean()) );
		theAttrib->drawTextNDC( tsize, 11, h_dQ[i]->GetLineColor(), x0, y0-tsize*2, Form("%-8s=%-5.1f", "sigma"  , h_dQ[i]->GetRMS()) );
	} 

	TCanvas* c09 = new TCanvas( "c09", "c09" );
	h_scale->Draw();

	c05->Print( pdfName );
	c06->Print( pdfName );
	c07->Print( pdfName );
	c08->Print( pdfName );
	c09->Print( Form("%s)", pdfName) );

	cout << "=================================" << endl;
	cout << "ouput: " << pdfName << endl;
	cout << "=================================" << endl;

}

