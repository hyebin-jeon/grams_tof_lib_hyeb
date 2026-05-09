auto theParamScan = TOF_ParameterScan::getInstance();
auto theChanConv  = TOF_ChannelConversion::getInstance();
auto theAttrib    = TOF_Attributes::getInstance();

void plotScan3()
{
	//TH1::AddDirectory(kFALSE);

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
	TString pname0 = (TString) param0_name(0, 3);
	TString pname1 = (TString) param1_name(0, 3);

	//theParamScan->printParamScanTable();

	/// active channel list
	uint8_t febD_connID = 3;
  uint8_t activeChannel[4] = { 1, 2, 65, 66 }; // SMA connector IDs
	const int chanN = 4;

	/// file path info
	const char* dirName = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20250820";
  const char* finName = "test_qdc_raw.root";

	const char* foutName = "output/plotScan3_20250820.root";

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

	cout << "========A=====" << endl;
	/// Initialization ///
	uint32_t activeAbsChanID[chanN];
	std::map<std::tuple<int, int, int>, TH1D*> mhQ; 
	TH1D* htemp{nullptr};
	for( int chIdx=0; chIdx<4; chIdx++ )
	{
		auto achanID = theChanConv->getAbsoluteChannelID( febD_connID, activeChannel[chIdx] );
		activeAbsChanID[chIdx] = achanID;
		for( auto pp0: params0 )
		{
			int p0 = (int) pp0;
		  for( auto pp1: params1 )
		  {
				int p1 = (int) pp1;
			  TString hname = Form( "hQ_ch%d_%s%02d_%s%02d", achanID, pname0.Data(), p0, pname1.Data(), p1 );
			  htemp = new TH1D( hname.Data(), "", 80, 0, 320 );

				theAttrib->attribHist( htemp );
				htemp->SetTitle( Form("%s%d_%s%d", pname0.Data(), p0, pname1.Data(), p1) );
				
				auto idx = std::tuple( achanID, p0, p1 );
		    mhQ[idx] = htemp;
			}
		}
	}

	///// check the initialized histo
	////int i=0;
	//for( const auto [tuple,h] : mhQ )
	//{
	//	auto ch   = std::get<0>(tuple);
	//	auto pidx0= std::get<1>(tuple);
	//	auto pidx1= std::get<2>(tuple);
	//	//cout << Form("%02d: [%03d][%d][%d]  %s",i++, ch, pidx0, pidx1, h->GetName()) << endl;
	//} 

	cout << "========B=====" << endl;
	/// Fill the histogram
	for( int i=0; i<entries; i++ )
	{
		t->GetEntry(i);
		
		int  achID = t->getChannelID(); // absolute channel ID
		auto efine = (int) t->getEFine();
		int  step1 = (int) t->getStep1();
		int  step2 = (int) t->getStep2();

		//cout << Form("[%04d] chIdx: %2d, pIdx0: %d, pIdx1: %d, step0: %d, step1: %2d, efine: %5d", i, chIdx, pIdx0, pIdx1, step1, step2, efine) << endl;
		auto idx = std::tuple( achID, step1, step2 );
		mhQ[idx]->Fill( efine );

		//if( i>100 ) break;
	}

	//cout << "Completed filling histograms from every branch element" << endl;

	TFile* fout = new TFile(foutName, "recreate");
	fout->mkdir( "histo" );
	fout->mkdir( "entries" );
	fout->mkdir( "mean" );
	
	cout << "========C=====" << endl;

	fout->cd("histo");
	for( const auto [tp, h]: mhQ )
	{
		//auto achID = std::get<0>(tp);
		//auto par0  = std::get<1>(tp);
		//auto par1  = std::get<2>(tp);
		h->Write();
		cout << h->GetName() << endl;
		//cout << h->GetTitle() << endl;
	}


	cout << "========D=====" << endl;

	/// Plots to see the relations between parameters ///
	std::map< int, TH2D* > mhScan_entr;
	std::map< int, TH2D* > mhScan_mean;
	std::map< int, TMultiGraph* > mGrEntr;
	std::map< int, TMultiGraph* > mGrMean;

	cout << "========E=====" << endl;
	TH2D* hentr{nullptr};
	TH2D* hmean{nullptr};
	TGraphErrors* gentr{nullptr};
	TGraphErrors* gmean{nullptr};
	TMultiGraph* mgentr{nullptr};
	TMultiGraph* mgmean{nullptr};
	TCanvas* c1{nullptr};
	
	for( auto chanID: activeAbsChanID )
	{
		//auto chanID = activeAbsChanID[ch];
		hentr = new TH2D( Form("hentr_ch%03d", chanID), "", param0_nb, params0[0], params0[-1], param1_nb, params1[0], params1[-1]);
		hmean = new TH2D( Form("hmean_ch%03d", chanID), "", param0_nb, params0[0], params0[-1], param1_nb, params1[0], params1[-1]);
		mhScan_entr[chanID] = hentr; 
		mhScan_mean[chanID] = hmean;
		
		mgentr = new TMultiGraph();
		mgentr->SetName ( Form( "gEntr_ch%03d", chanID ) );
		mgmean = new TMultiGraph();
		mgmean->SetName( Form( "gEntr_ch%03d", chanID ) );

	  c1 = new TCanvas( Form("c1_ch%03d", chanID), "", 1400, 1200);
	  c1->Divide( param0_nb, param1_nb, 0.002, 0.002 );

	  for( int pidx1=0; pidx1<param1_nb; pidx1++ )
	  {
			auto pp1= params1[pidx1];
			int  p1 = (int) pp1;
			gentr = new TGraphErrors();
			gentr->SetName( Form("gEntr_ch%03d_%s_%02d", chanID, pname1.Data(), p1) );
			gentr->SetTitle( Form("%s_%02d", param1_name.Data(), p1) );
			
			gmean = new TGraphErrors();
			gmean->SetName( Form("gMean_ch%03d_%s_%02d", chanID, pname1.Data(), p1) );
			gmean->SetTitle( Form("%s_%02d", param1_name.Data(), p1) );
			
			cout << gentr->GetName() << endl;
			cout << gmean->GetName() << endl;

	    for( int pidx0=0; pidx0<param0_nb; pidx0++ )
	  	{
			  auto pp0= params0[pidx0];
			  int  p0 = (int) pp0;

	  		int  canvIdx = pidx1*param0_nb + pidx0 + 1;
				c1->cd( canvIdx );
				
				auto idx = std::tuple( chanID, p0, p1 );
				if(!mhQ[idx]) {
					cout << "WRONG HISTOGRAMS INDEX" << endl;
					continue;
				}

				mhQ[idx]->Draw();
	  	
	  		gPad->Update();
	  		gPad->Modified();

	  		auto ps = (TPaveStats*) gPad->GetPrimitive("stats");
	  		theAttrib->moveStatBox( ps, 0.6, 0.65, 0.95, 0.89 );

	  		auto counts = mhQ[idx]->GetEntries();
				auto mean   = mhQ[idx]->GetMean(); 
				auto rms   = mhQ[idx]->GetRMS(); 

				//mhScan_entr[chanID]->Fill( p0, p1, counts );
				//mhScan_mean[chanID]->Fill( p0, p1, mean   );
				mhScan_entr[chanID]->Fill( Form("%d", p0), Form("%d", p1), counts );
				mhScan_mean[chanID]->Fill( Form("%d", p0), Form("%d", p1), mean   );

				gentr->SetPoint( pidx0, p0, counts );
				gmean->SetPoint( pidx0, p0, mean   );
				
				gentr->SetPointError( pidx0, 0, TMath::Sqrt(counts) );
				gmean->SetPointError( pidx0, 0, rms   );
			}

			//auto idx_gr = std::tuple( ch, pidx1 );
			//mGrEntr[ idx_gr ] = gentr;
			//mGrMean[ idx_gr ] = gmean;
			mgentr->Add( gentr );
			mgmean->Add( gmean );

		}
		
		mgentr->GetHistogram()->SetTitle( Form("ch%03d;%s;Entries", chanID, param0_name.Data()) );
		mgmean->GetHistogram()->SetTitle( Form("ch%03d;%s;Mean", chanID, param0_name.Data()) );
		mGrEntr[ chanID ] = mgentr;
		mGrMean[ chanID ] = mgmean;

		c1->Write();
		c1->Destructor ();
	}
	cout << "========F=====" << endl;

	fout->cd( "entries" );
	for( auto chanID: activeAbsChanID ) mhScan_entr[chanID]->Write();
	for( auto chanID: activeAbsChanID ) mGrEntr[chanID]->Write();
	fout->cd( "mean" );
	for( auto chanID: activeAbsChanID ) mhScan_mean[chanID]->Write();
	for( auto chanID: activeAbsChanID ) mGrMean[chanID]->Write();

	cout << "========G=====" << endl;
	sleep(2);
	cout << "========H=====" << endl;

	TCanvas* c2 = new TCanvas("c2", "", 1200, 600);
	c2->Divide( chanN, 2 );
	for( int i=0; i< chanN; i++ )
	{
		int chID = activeAbsChanID[i];
		int canv = i+1;
		c2->cd( canv );
		mGrEntr[chID]->Draw( "apl plc pmc" );
		auto leg0 = gPad->BuildLegend();
		theAttrib->setAttribLegendEntries( leg0, "line" ); 
		leg0->Draw();

		c2->cd( chanN+canv );
		mGrMean[chID]->Draw( "apl plc pmc" );
		auto leg1 = gPad->BuildLegend();
		theAttrib->setAttribLegendEntries( leg1, "line" ); 
		leg1->Draw();
	}
	fout->cd();
	c2->Write();


	cout << "========I=====" << endl;
	fout->Close();
	fin->Close();	

	//fout->Delete();
	//fin->Delete();
	cout << "========END=====" << endl;
}

