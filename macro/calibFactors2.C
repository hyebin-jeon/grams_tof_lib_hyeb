
auto theAttrib = TOF_Attributes::getInstance();
auto theCalib  = TOF_TdcQdcCalibration::getInstance();
auto theChanConv = TOF_ChannelConversion::getInstance();
	
std::vector< int > absChannelList;
std::map< std::tuple<uint32_t, uint8_t>, TH2S* > fDataHistList;
std::map< std::tuple<uint32_t, uint8_t>, std::tuple<double,double> > fFitParams;


void getData( const char* finPath );
TH2S* getDataOfChannel( uint32_t achanID, uint8_t tacID );

void calibFactors2()
{
	const char* dirName   = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20250903_QDC_vs_ToT";
  const char* finName   = "test_qdc_raw.root";
	const char* finPath   = Form("%s/%s", dirName, finName  );
	//TFile* fin   = new TFile( finPath  , "read" );
	//if( !fin->IsOpen() ) {
	//	printf("[ERR] FILE NOT FOUND: %s", finPath);
	//	return;
	//}

	std::vector<int> chipID = {4, 5};
	std::map< int, TFile* > mfQcal;
	for( auto chip: chipID ) 
	{
	  const char* fQcalName = Form("qdc_calibration_00_00_%02d.root", chip);
	  const char* fQcalPath = Form("%s/%s", dirName, fQcalName);
	  TFile* fQcal = new TFile( fQcalPath, "read" );
	  if( !fQcal->IsOpen() ) {
	  	printf("[ERR] FILE NOT FOUND: %s", fQcalPath);
	  	return;
	  }

		mfQcal[chip] = fQcal;
	}

	//auto achanID = theChanConv->getAbsoluteChannelID( 0, 0, 4, 0 ); // from hQcal

	//std::vector< uint8_t > absChannelList;
	for( auto chip: chipID )
	{
	  for( int ch=0; ch<=1; ch++ )
	  {
			auto achanID = theChanConv->getAbsoluteChannelID( 0, 0, chip, ch ); // from hQcal
			//cout << "absChannelList: " << (int) achanID << endl;
			absChannelList.push_back( (int) achanID );
		}
	}

	/// This must be located after filling absChannelList
	getData( finPath );

	const int tacN = 4; // number of TAC for each channel
	const int polN = 9;

	double fitXmin= 45;
	double fitXmax= 85;
	double ymin = -150;
	double ymax =  450;

	/// fitting range highlighter box
	TBox box;
	box.SetFillColorAlpha( kYellow, 0.3 );
	box.SetX1( fitXmin );
	box.SetX2( fitXmax );
	box.SetY1( ymin );
	box.SetY2( ymax );

	TCanvas* c1 = new TCanvas("c1", "c1");
  c1->Print("output/calibFactors2.pdf[");
	c1->Divide(2,2);
	
	TCanvas* c2 = new TCanvas("c2", "c2");
  c2->Print("output/calibFactors2_raw.pdf[");
	c2->Divide(2,2);
	
	gStyle->SetOptStat(111111);

	for( auto chip: chipID )
	{
	  for( int ch=0; ch<=1; ch++ )
	  {
			auto achanID = theChanConv->getAbsoluteChannelID( 0, 0, chip, ch ); // from hQcal
	    
	    for(int i=0; i<tacN; i++ )
	    {
	    	c1->cd(i+1);

	    	TProfile* pRef = (TProfile*) mfQcal[chip]->Get(Form("c_00_00_%02d_%02d_%d_pFine", chip, ch, i));
				pRef->GetXaxis()->SetTitle("tEnd-ti (clocks)");

	      /// x axis range
	      double xmax = pRef->GetXaxis()->GetXmax();
	      double xmin = pRef->GetXaxis()->GetXmin();

	    	auto params = theCalib->getQdcParams( achanID, i );
	    	TF1* fRef = new TF1( Form("fRef_tac%d",i), "pol9", xmin, xmax );
	      fRef->SetLineColor(kGreen);
	      fRef->SetLineWidth(2);
	      for( int i=0; i<polN+1; i++ ) fRef->SetParameter(i, params.at(i) );

	      auto xmaxBin = pRef->GetXaxis()->FindBin(240);
	      pRef->GetXaxis()->SetRange(0, xmaxBin);
	      pRef->SetMaximum( ymax );
	      pRef->SetMinimum( ymin );
	      pRef->Draw();

	  		box.Draw();

	      fRef->Draw("same");
	    
	    	TF1* fLin = new TF1( Form("fLin_tac%d", i), "pol1", xmin, xmax);
	      fLin->SetLineColor(kRed);
	      fLin->SetLineWidth(2);

	    	pRef->Fit( Form("fLin_tac%d",i), "q", "same goff", fitXmin, fitXmax);
      	fLin->Draw("same");
	      
				//cout << Form("The liner function fitted on (%.1f, %.1f): ", fitXmin, fitXmax ) << endl;
	      cout << Form("[ch.%03d][tac.%d] slope, intercept: (%.2f, %.2f)", achanID, i, fLin->GetParameter(1), fLin->GetParameter(0)) << endl;
				
				auto idx = std::tuple( ch, i );
				fFitParams[idx] = std::tuple( fLin->GetParameter(0), fLin->GetParameter(1) );
			

			  auto hData = getDataOfChannel( achanID, i );
				if( hData==nullptr ) continue;
	      auto pData = hData->ProfileX( Form("p_ch%03d_t%d",achanID,i), 1, -1, "s"); 
				pData->SetMarkerSize(2);
				pData->SetMarkerStyle(2);
				pData->SetMarkerColor(kMagenta);
				pData->SetLineColor(kMagenta);
				pData->Draw("same");

				auto leg = new TLegend(0.67, 0.11, 0.89, 0.40, "");
				leg->AddEntry( pRef, "calib data", "l" );
				leg->AddEntry( fRef, "calib fit (pol9)", "l" );
				leg->AddEntry( fLin, "linear fit", "l" );
				leg->AddEntry( pData, "muon data" "p");
				//leg->AddEntry( box, "linFit range", "f");
				leg->Draw();
	    	
				c2->cd(i+1);
				hData->Draw();
				pData->Draw("same");

	    }
	    c1->Print("output/calibFactors2.pdf");
	    c2->Print("output/calibFactors2_raw.pdf");
	  }
	}
	c1->Print("output/calibFactors2.pdf]");
	c2->Print("output/calibFactors2_raw.pdf]");
}

void getData( const char* finPath )
{
	TFile* fin   = new TFile( finPath  , "read" );
	
	if( !fin->IsOpen() ) {
		printf("[ERR] FILE NOT FOUND: %s", finPath);
		return;
	}

	TOF_TreeData* t = (TOF_TreeData*) fin->Get("data");
	if( !t ) return;
	t->setBranchAddress();

	TH2S* hData{nullptr};
	for( auto ch: absChannelList )
	{
		for( int tac=0; tac<4; tac++ )
		{
	    //hData = new TH2S(Form("hData_ch%03d_tac%d", ch, tac), "", 100, 50, 70, 100, 100, 140);
	    //hData = new TH2S("hData", "", 1024, 0, 1024, 500, 0, 500);
	    hData = new TH2S("hData", "", 150, 0, 150, 500, 0, 500);
	    hData->SetTitle(";tEnd-ti (clocks);Qcal");

			auto idx = std::tuple( ch, tac );
		  fDataHistList[idx] = hData;
		}
	}

	theCalib->readCalibrationFiles( "../calibration/20250903" );
	//theCalib->printQdcCalibTable( achanID );

	int entries = t->GetEntries();
	for( int i=0; i<entries; i++ )
	{
		t->GetEntry(i);

		auto achanID   = t->getChannelID(); // absolute channel ID

		bool roi = 0;
		for( auto chanRef: absChannelList ) 
		{
			if( chanRef != achanID ) continue;
			roi = 1;
		}

		auto connID    = t->getConnIdOnFebD   ( achanID );
		auto asicID    = t->getAsicID         ( achanID );
		auto channelID = t->getChannelIdOnFebS( achanID );  // 1-128
		auto tacID     = t->getTacID();

		auto frameID = t->getFrameID();
		auto tCoarse = t->getTCoarse();
		auto eCoarse = t->getECoarse();
		auto tFine   = t->getTFine()  ;
		auto eFine   = t->getEFine()  ;

		auto time0 = theCalib->getCalibratedTime( TOF_Mode::fQdc, achanID, tacID, frameID, tCoarse, tFine );
		auto time1 = double((frameID*1024+eCoarse));
		auto tintg = time1 - time0;
		auto qcal = theCalib->getCalibratedQDC( achanID, tacID, frameID, eCoarse, eFine, tCoarse, time0  );

		//hData->Fill( tintg, qcal );

		//cout << Form( "[%03d] achanID= %3d, tacID= %d, tintg= %.1f, efine= %.1hu, qcal= %.1f", i, achanID, tacID, tintg, eFine, qcal ) << endl;
		auto idx = std::tuple( (uint32_t) achanID, (uint8_t) tacID );
		if( fDataHistList[idx] == nullptr ) {
		  //if( i>500 ) break;
			continue;
		}
		fDataHistList[idx]->Fill( tintg, eFine );
		//cout << Form( "[%03d] pair.first= %03d, pair.second= %d", i, std::get<0>(idx), std::get<1>(idx) ) << endl;

		//if( i>500 ) break;
	}

	cout << "[getData] Completed. fDataHistList.size()= " << fDataHistList.size() << endl;

	return;
}

TH2S* getDataOfChannel( uint32_t achanID, uint8_t tacID )
{
	if( fDataHistList.size() == 0 ) {
		cout << "[getDataOfChannel] fDataHistList is EMPTY" << endl;
		return nullptr;
	}

	auto idx = std::tuple( (uint32_t) achanID, (uint8_t) tacID );
	if( fDataHistList[idx] == nullptr ) {
		cout << Form("fDataHistList.at( uint32_t achanID= %03d, uint8_t tacID= %d ) is NULL", achanID, tacID) << endl;
		return nullptr;
	}

	return fDataHistList[idx];
}


