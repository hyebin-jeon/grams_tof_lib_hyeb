auto theParamScan = TOF_ParameterScan::getInstance();
auto theChanConv  = TOF_ChannelConversion::getInstance();
auto theAttrib    = TOF_Attributes::getInstance();

void plotScan22()
{
	/// Parameter table info
	//const char* pTable = "../calibration/20250815_GSFC/param_table_vthT1.txt";
	const char* pTable = "../calibration/20250820/param_table_discLSBt1.txt";
	theParamScan->readParamScanTable( pTable );
	int param0_nb = theParamScan->getNbOfParameter(0);
	int param1_nb = theParamScan->getNbOfParameter(1);
	auto param0_name = theParamScan->getParameterName(0);
	auto param1_name = theParamScan->getParameterName(1);
	auto params0 = theParamScan->getParameterValueList(0);
	auto params1 = theParamScan->getParameterValueList(1);

	//theParamScan->printParamScanTable();

	/// active channel list
	uint8_t febD_connID = 3;
  uint8_t activeChannel[4] = { 1, 2, 65, 66 }; // SMA connector IDs
	const int chanN = 4;

	/// file path info
	const char* dirName = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20250820";
  const char* finName = "test_qdc_raw.root";

  const char* finPath = Form("%s/%s", dirName, finName); 
	TFile* fin = new TFile( finPath , "read" );
	if( !fin->IsOpen() ) {
		printf("[ERR] FILE NOT FOUND: %s", finPath);
		//delete fin;
		return;
	}

	TOF_TreeData* t = (TOF_TreeData*) fin->Get("data");
	if( !t ) return;
	t->setBranchAddress();
	int entries = t->GetEntries();

	/// Histogram initialization
	std::vector<std::vector<std::vector<TH1D*>>> vvvhQ; //( chanN, std::vector<std::vector<TH1D*>>
			                                            //( param0_nb, std::vector<TH1D*> 
																								  //( param1_nb, nullptr ) ) );
	uint32_t activeAbsChanID[chanN];
	for( int chIdx=0; chIdx<4; chIdx++ )
	{
		activeAbsChanID[chIdx] = theChanConv->getAbsoluteChannelID( febD_connID, activeChannel[chIdx] );

		std::vector< std::vector<TH1D*> > vvhQ;
		vvhQ.clear();
		for( int pIdx0=0; pIdx0<param0_nb; pIdx0++ )
		{
			std::vector<TH1D*> vhQ;
			vhQ.clear();
		  for( int pIdx1=0; pIdx1<param1_nb; pIdx1++ )
		  {
			  const char* hname = Form( "hQ_ch%d_%s_%d_%s_%d", 
						                      activeChannel[chIdx], 
																  param0_name.Data(), (int) params0[pIdx0],
																  param1_name.Data(), (int) params1[pIdx1] );
				
			  TH1D* htemp = new TH1D( hname, "", 80, 0, 320 );
				vhQ.push_back( htemp );
			}
			vvhQ.push_back( vhQ );
		}
		vvvhQ.push_back( vvhQ );
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

		//cout << Form("[%04d] chIdx: %2d, pIdx0: %d, pIdx1: %d, step0: %d, step1: %2d, efine: %5d", i, chIdx, pIdx0, pIdx1, step1, step2, efine) << endl;
		vvvhQ[chIdx][pIdx0][pIdx1]->Fill( efine );

		//if( i>100 ) break;
	}

	TFile* fout = new TFile("output/plotScan2.root", "recreate");
	for( int ch=0; ch<chanN; ch++ )
	{
		for( int pidx0=0; pidx0<param0_nb; pidx0++ )
		{
		  for( int pidx1=0; pidx1<param1_nb; pidx1++ )
			{
				theAttrib->attribHist( vvvhQ[ch][pidx0][pidx1] );
				vvvhQ[ch][pidx0][pidx1]->SetTitle( Form("%s=%.0f, %s=%.0f;efine (QDC in ADC);Counts", param0_name.Data(), params0[pidx0], param1_name.Data(), params1[pidx1]) );
				vvvhQ[ch][pidx0][pidx1]->Write();
			}
		}
	}

	TH2D* hScan_entr[chanN];
	TH2D* hScan_mean[chanN];
	std::vector<std::vector<TGraphErrors*>> vvgErr_entr;
	std::vector<std::vector<TGraphErrors*>> vvgErr_mean;
	TMultiGraph* gEntr[chanN];
	TMultiGraph* gMean[chanN];
	for( int ch=0; ch<chanN; ch++ )
	{
		auto chanID = activeAbsChanID[ch];
		hScan_entr[ch] = new TH2D( Form("hScan_entr_ch%03d", chanID), "", param0_nb, 0, param0_nb, param1_nb, 0, param1_nb );
		hScan_mean[ch] = new TH2D( Form("hScan_mean_ch%03d", chanID), "", param0_nb, 0, param0_nb, param1_nb, 0, param1_nb );
		
		gEntr[ch] = new TMultiGraph();
		gEntr[ch]->SetName ( Form( "gEntr_ch%03d", chanID ) );
		gMean[ch] = new TMultiGraph();
		gMean[ch]->SetName( Form( "gEntr_ch%03d", chanID ) );

	  TCanvas* c1 = new TCanvas( Form("c1_ch%03d", chanID), "", 1400, 1200);
	  c1->Divide( param0_nb, param1_nb, 0.002, 0.002 );

		std::vector<TGraphErrors*> vgErr_entr;
		std::vector<TGraphErrors*> vgErr_mean;
		vgErr_entr.clear();
		vgErr_mean.clear();
	  for( int pidx1=0; pidx1<param1_nb; pidx1++ )
	  {
			auto gtempE = new TGraphErrors();
			gtempE->SetName( Form("gEntr_ch%03d_%s_%.0f", chanID, param1_name.Data(), params1[pidx1]) );
			//gtempE->SetTitle( Form("ch%03d_%s_%.0f", chanID, param1_name.Data(), params1[pidx1]) );
			
			auto gtempM = new TGraphErrors();
			gtempM->SetName( Form("gMean_ch%03d_%s_%.0f", chanID, param1_name.Data(), params1[pidx1]) );
			//gtempM->SetTitle( Form("ch%03d_%s_%.0f", chanID, param1_name.Data(), params1[pidx1]) );

	    for( int pidx0=0; pidx0<param0_nb; pidx0++ )
	  	{
	  		int canvIdx = pidx1*param0_nb + pidx0 + 1;
	  		
				c1->cd( canvIdx );
				vvvhQ[ch][pidx0][pidx1]->Draw();
				//hQ[ch][pidx0][pidx1]->DrawNormalized();
				//hQ[ch][pidx0][pidx1]->GetYaxis()->SetTitle( "Normalized counts" );
	  	
	  		gPad->Update();
	  		gPad->Modified();

	  		auto ps = (TPaveStats*) gPad->GetPrimitive("stats");
	  		theAttrib->moveStatBox( ps, 0.6, 0.65, 0.95, 0.89 );

	  		auto counts = vvvhQ[ch][pidx0][pidx1]->GetEntries();
				auto mean   = vvvhQ[ch][pidx0][pidx1]->GetMean(); 

				hScan_entr[ch]->Fill( Form("%.0f", params0[pidx0]), Form("%.0f", params1[pidx1]), counts );
				hScan_mean[ch]->Fill( Form("%.0f", params0[pidx0]), Form("%.0f", params1[pidx1]), mean   );

				gtempE->SetPoint( pidx0, params0[pidx0], counts );
				gtempM->SetPoint( pidx0, params0[pidx0], mean   );
			}

			vgErr_entr.push_back( gtempE );
			vgErr_mean.push_back( gtempM );
		}

		vvgErr_entr.push_back( vgErr_entr );
		vvgErr_mean.push_back( vgErr_mean );

		c1->Modified();
		c1->Update();
		c1->Write();
	}

	for( int ch=0; ch<chanN; ch++ )
	{
		for( int pidx1=0; pidx1< param1_nb; pidx1++ )
		{
			//vvgErr_entr[ch][pidx1]->SetTitle( Form("%s_%2.0f", param1_name.Data(), params1[pidx1]) );
			gEntr[ch]->Add( vvgErr_entr[ch][pidx1] );
			gMean[ch]->Add( vvgErr_mean[ch][pidx1] );
		}
	}

	gStyle->SetOptStat(0);
	//gStyle->SetPalette(kLightTemperature);
	TCanvas* c2 = new TCanvas("c2", "c2", 550, 500 );
	for( int i=0; i<chanN; i++ ) {
		c2->SetName( Form("hScan_entr_ch%03d", activeAbsChanID[i]) );
		hScan_entr[i]->SetTitle( Form("Entries;%s;%s",param0_name.Data(), param1_name.Data()) );
		hScan_entr[i]->GetZaxis()->SetLabelSize(0.02);
		hScan_entr[i]->Draw("colz");
		hScan_entr[i]->Write();
		//c2->Write();
		
		hScan_mean[i]->SetTitle( Form("Mean;%s;%s",param0_name.Data(), param1_name.Data()) );
		hScan_mean[i]->GetZaxis()->SetLabelSize(0.02);
		hScan_mean[i]->Draw("colz");
		hScan_mean[i]->Write();
	}


	TCanvas* c3 = new TCanvas("c3", "c3", 1000, 900 );
	c3->Divide( 2,2 );
	for( int i=0; i<4; i++ )
	{
		c3->cd(i+1);
		gEntr[i]->Draw("apl plc pmc");
		//gEntr[i]->GetHistogram()->SetTitle( Form( "ch%03d;%s;Entries", activeAbsChanID[i], param0_name.Data()) );
		gEntr[i]->Write();
	}
	c3->Write();
	for( int i=0; i<4; i++ )
	{
		c3->cd(i+1);
		gMean[i]->Draw("apl plc pmc");
		//gMean[i]->GetHistogram()->SetTitle( Form( "ch%03d;%s;Mean", activeAbsChanID[i], param0_name.Data()) );
		gMean[i]->Write();
	}
	c3->Write();

	fout->Close();
	//delete fout;

	//// 히스토그램 저장 후 해제
  //for (int ch = 0; ch < chanN; ch++) {
  //  for (int pidx0 = 0; pidx0 < param0_nb; pidx0++) {
  //    for (int pidx1 = 0; pidx1 < param1_nb; pidx1++) {
  //      vvvhQ[ch][pidx0][pidx1]->Write();
  //      delete vvvhQ[ch][pidx0][pidx1]; // 반드시 추가
	//		}
  //  }
	//}

	//for (int i = 0; i < chanN; i++) {
  //  delete hScan_entr[i];
  //  delete hScan_mean[i];
  //  delete gEntr[i];
  //  delete gMean[i];
  //}
}

