auto theParamScan = TOF_ParameterScan::getInstance();
auto theChanConv  = TOF_ChannelConversion::getInstance();
auto theAttrib    = TOF_Attributes::getInstance();

void plotScan2()
{
	/// Parameter table info
	//const char* pTable = "../calibration/20250815_GSFC/param_table_vthT1.txt";
	const char* pTable = "../calibration/20250820_GSFC/param_table_discLSBt1.txt";
	theParamScan->readParamScanTable( pTable );
	//theParamScan->printParamScanTable();
	auto param0_nb = theParamScan->getNbOfParameter(0);
	auto param1_nb = theParamScan->getNbOfParameter(1);
	auto param0_name = theParamScan->getParameterName(0);
	auto param1_name = theParamScan->getParameterName(1);
	auto params0 = theParamScan->getParameterValueList(0);
	auto params1 = theParamScan->getParameterValueList(1);

	/// active channel list
	uint8_t febD_connID = 3;
  uint8_t activeChannel[4] = { 1, 2, 65, 66 }; // SMA connector IDs
	int chanN = 4;

	/// file path info
	const char* dirName = "~/work/grams/20250820_newPaddle_and_paramScan2";
  const char* finName = "test_qdc_raw.root";

  const char* finPath = Form("%s/%s", dirName, finName); 
	TFile* fin = new TFile( finPath , "read" );
	if( !fin->IsOpen() ) {
		printf("[ERR] FILE NOT FOUND: %s", finPath);
		return;
	}

	TOF_TreeData* t = (TOF_TreeData*) fin->Get("data");
	if( !t ) return;
	t->setBranchAddress();
	int entries = t->GetEntries();

	/// Histogram initialization
	TH1D* hQ[4][param0_nb][param1_nb];
	uint32_t activeAbsChanID[4];
	for( int chIdx=0; chIdx<4; chIdx++ )
	{
		activeAbsChanID[chIdx] = theChanConv->getAbsoluteChannelID( febD_connID, activeChannel[chIdx] );

		for( int pIdx0=0; pIdx0<param0_nb; pIdx0++ )
		{
		  for( int pIdx1=0; pIdx1<param1_nb; pIdx1++ )
		  {
			  const char* hname = Form( "hQ_ch%d_%s_%d_%s_%d", 
						                      activeChannel[chIdx], 
																  param0_name.Data(), (int) params0[pIdx0],
																  param1_name.Data(), (int) params1[pIdx1] );
			  hQ[chIdx][pIdx0][pIdx1] = new TH1D( hname, "", 80, 0, 320 );
			}
		}
	}

	/// Fill the histogram
	for( int i=0; i<entries; i++ )
	{
		t->GetEntry(i);
		
		int chIdx=-1;
		for( int idx=0; idx<4; idx++ )
		{
		  if( activeAbsChanID[idx] == t->getChannelID() ) chIdx = idx;
		}

		if( chIdx<0 ) continue;

		auto efine = (int) t->getEFine();
		auto step1 = (int) t->getStep1();
		auto step2 = (int) t->getStep2();

		int pIdx0(-1), pIdx1(-1);
		for( int idx=0; idx<param0_nb; idx++ )
		{
			if( step1==params0[idx] ) {
				pIdx0=idx;
				break;
			}
		}
		for( int idx=0; idx<param0_nb; idx++ )
		{
			if( step2==params1[idx] ) {
				pIdx1=idx;
				break;
			}
		}

		if( pIdx0<0 || pIdx0>param0_nb ) { cout << "Wrong Param Idx :(" << endl; continue; }
		if( pIdx1<0 || pIdx1>param1_nb ) { cout << "Wrong Param Idx :(" << endl; continue; }

		//cout << Form("[%04d] chIdx: %2d, stepIdx: %d, step1: %2d, efine: %5d", i, chIdx, stepIdx, step1, efine) << endl;
		hQ[chIdx][pIdx0][pIdx1]->Fill( efine );

		//if( i>100 ) break;
	}

	TFile* fout = new TFile("output/plotScan2.root", "recreate");
	for( int ch=0; ch<chanN; ch++ )
	{
		for( int pidx0=0; pidx0<param0_nb; pidx0++ )
		{
		  for( int pidx1=0; pidx1<param1_nb; pidx1++ )
			{
				theAttrib->attribHist( hQ[ch][pidx0][pidx1] );
				hQ[ch][pidx0][pidx1]->SetTitle( Form("%s=%.0f, %s=%.0f;efine (QDC in ADC);Counts", param0_name.Data(), params0[pidx0], param1_name.Data(), params1[pidx1]) );
				hQ[ch][pidx0][pidx1]->Write();
			}
		}
	}

	TH2D* hScan[4];
	for( int ch=0; ch<chanN; ch++ )
	{
		hScan[ch] = new TH2D( Form("hScan_ch%03d", activeAbsChanID[ch]), "", param0_nb, 0, param0_nb, param1_nb, 0, param1_nb );

	  TCanvas* c1 = new TCanvas( Form("c1_ch%03d",activeAbsChanID[ch]), "", 1400, 1200);
	  c1->Divide( param0_nb, param1_nb, 0.002, 0.002 );
	  std::vector<std::tuple<int, int, int, int>> vEntries;
	  for( int pidx0=0; pidx0<param0_nb; pidx0++ )
	  {
	    for( int pidx1=0; pidx1<param1_nb; pidx1++ )
	  	{
	  		int canvIdx = pidx1*param0_nb + pidx0 + 1;
	  		
				c1->cd( canvIdx );
				//hQ[ch][pidx0][pidx1]->Draw();
				hQ[ch][pidx0][pidx1]->DrawNormalized();
				hQ[ch][pidx0][pidx1]->GetYaxis()->SetTitle( "Normalized counts" );
	  	
	  		gPad->Update();
	  		gPad->Modified();

	  		auto ps = (TPaveStats*) gPad->GetPrimitive("stats");
	  		theAttrib->moveStatBox( ps, 0.6, 0.65, 0.95, 0.89 );

	  		int counts = hQ[ch][pidx0][pidx1]->GetEntries();
	  		vEntries.push_back( std::make_tuple(ch, pidx0, pidx1, counts) );

				hScan[ch]->Fill( Form("%.0f", params0[pidx0]), Form("%.0f", params1[pidx1]), counts );
			}
		}

		c1->Modified();
		c1->Update();
		c1->Write();
	}


	gStyle->SetOptStat(0);
	TCanvas* c2 = new TCanvas("c2", "c2", 550, 500 );
	for( int i=0; i<chanN; i++ ) {
		c2->SetName( Form("hScan_ch%03d", activeAbsChanID[i]) );
		hScan[i]->SetTitle( Form(";%s;%s",param0_name.Data(), param1_name.Data()) );
		hScan[i]->GetZaxis()->SetLabelSize(0.02);
	  gStyle->SetPalette(kLightTemperature);
		hScan[i]->Draw("colz");
		hScan[i]->Write();
		//c2->Write();
	}


	/*
	TGraph* gCh[4];
	TGraph* gSum = new TGraph();
	gSum->SetNameTitle( "gSum", ";vth_t1 (DAC);Counts" );
	for( int i=0; i<4; i++ )
	{
		gCh[i] = new TGraph();
		gCh[i]->SetName( Form("gCh%d", activeChannel[i]) );
		gCh[i]->SetTitle( Form("Ch%d;vth_t1 (DAC);Counts", activeChannel[i]) );
		gCh[i]->SetMarkerStyle(20 + i);
	}
	for( auto ele: vEntries )
	{
		auto chIdx   = std::get<0>(ele);
		auto stepIdx = std::get<1>(ele);
		auto entries = std::get<2>(ele);

		gCh[chIdx]->AddPoint( stepIdx*10, entries );
	}

	TMultiGraph* mg = new TMultiGraph();
	mg->SetName("mg");
	mg->SetTitle(";vth_t1 (DAC);Counts");
	for( int i=0; i<4; i++ ) mg->Add( gCh[i] );

	TCanvas* c2 = new TCanvas("c2", "c2");
	mg->Draw("apl plc pmc");
	mg->GetHistogram()->SetMaximum(570);
	mg->GetHistogram()->SetMinimum(300);
	gPad->BuildLegend( 0.82, 0.7, 0.9, 0.9 );
*/
}

