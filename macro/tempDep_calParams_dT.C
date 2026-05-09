///### This macro is written to study uncertainty of time resolution due to temp variation.
/// To do so, first I wanted to see whether the parameter variation due to the temp change behaves in common way for all channels or not

std::vector<TString> vParName = {"t0", "a0", "a1", "a2"};

void tempDep_calParams_dT()
{
	TFile* fin = new TFile("output/calib_par_temp_all.root", "read");


	auto dirT = fin->GetDirectory("tdc");
	auto dirQ = fin->GetDirectory("qdc");

	if( !dirT ) {
		std::cerr << "[ERR] dirT does not exist" << std::endl;
	}
	if( !dirQ ) {
		std::cerr << "[ERR] dirQ does not exist" << std::endl;
	}

	auto listT = dirT->GetListOfKeys();
	auto listQ = dirQ->GetListOfKeys();

	//////////////////////////////////////////////////////////
	/// TDC 
	//////////////////////////////////////////////////////////
	TH1D* hSlopeAll [4];  // slope of fitting
	TH1D* hYInterAll[4]; // y-intercept of fitting
	TH1D* hSlope [2][4];  // slope of fitting
	TH1D* hYInter[2][4]; // y-intercept of fitting
	double xminS[4] = { -0.3, 120, 130, -2.5 };
	double xmaxS[4] = {  0.3, 180, 200,  2.5 };
	double xminI[4] = { -0.002, -0.6, -0.6, -0.08 };
	double xmaxI[4] = {  0.002,  0.2,  0.1,  0.05 };
	for( int chip =0; chip<2; chip++ ) 
	{
	  for( int i=0; i<4; i++)
	  {
	  	hSlope [chip][i] = new TH1D( Form("hSlope_%d_%d" , chip, i), Form("%d_%s_slope"     , chip, vParName.at(i).Data()), 50, xminS[i], xmaxS[i] );
	  	hYInter[chip][i] = new TH1D( Form("hYInter_%d_%d", chip, i), Form("%d_%s_yintercept", chip, vParName.at(i).Data()), 50, xminI[i], xmaxI[i] );

			if( chip>0 ) continue;
	  	hSlopeAll [i] = new TH1D( Form("hSlope_all_%d" , i), Form("all_%s_slope"     , vParName.at(i).Data()), 50, xminS[i], xmaxS[i] );
	  	hYInterAll[i] = new TH1D( Form("hYInter_all_%d", i), Form("all_%s_yintercept", vParName.at(i).Data()), 50, xminI[i], xmaxI[i] );
	  }
	}

	int idx = 0;
	TCanvas* c1 = new TCanvas("c1", "c1", 1000, 800);
	int rowN = 8;
	int colN = 6;
	c1->Divide( rowN, colN );
	
	/// TDC directory
	gStyle->SetTitleFontSize(0.1);
	for( TObject* obj: *listT )
	{
		TString name = obj->GetName(); // ex)  g_chip1_ch63_a2
		TGraph* gr = (TGraph*) dirT->Get( name );

		int idx_chip = name.Index( "chip" ) + 4;
		int idx_chan = name.Last ( '_' ) - 2;
		int idx_par  = name.Last ( '_' ) + 1;

		TString chipID_str  = (TString) name( idx_chip, 1);
		TString chanID_str  = (TString) name( idx_chan, 2);
		TString parName     = (TString) name( idx_par , 2);
		cout << Form("[%03d]", idx) << "Name: " << gr->GetName() << Form("==> chip: %3s, channel: %3s, parName: %3s", chipID_str.Data(), chanID_str.Data(), parName.Data() ) << endl; 

		int chipID = chipID_str.Atoi();
		int chanID = chanID_str.Atoi();
		
		TString parNameID_str = (TString) parName(1,1);
		int parID  = parName.BeginsWith("t")? 0 : parNameID_str.Atoi()+1;
		//cout << "Name: " << gr->GetName() << Form("==> chip: %3d, channel: %3d, parName: %3s, parNameID_str: %2s, parNameID: %2d", chipID, chanID, parName.Data(), parNameID_str.Data(), parID ) << endl; 

		//for( int i=0; i<gr->GetN(); i++ )
		//	cout << Form("[%d_%02d] Point= %02d, x= %3.1f, y= %3.1f", chipID, chanID, idx, gr->GetPointX(i), gr->GetPointY(i)) << endl;

		gr->SetTitle( Form("chip%d_ch%02d_%s", chipID, chanID, parName.Data()) );

		auto fit = gr->GetFunction("pol1");
		if(!fit) std::cerr<< "[ERR] pol1 does not exist" << std::endl;
		else {
		  fit->SetLineColor(kAzure);
			auto slope  = fit->GetParameter(0); 
			auto yinter = fit->GetParameter(1); // y intercept

			hSlope [chipID][parID]->Fill( slope  );
			hYInter[chipID][parID]->Fill( yinter );
			hSlopeAll [parID]->Fill( slope  );
			hYInterAll[parID]->Fill( yinter );
		}
		
		idx++;

		if( idx <= rowN*colN ) { 
			c1->cd(idx); 
			gr->Draw("apl"); 
		}

		//if( idx>10 ) break;
	}

	//gStyle->SetOptStat(111111);
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	TCanvas* c2 = new TCanvas("c2", "c2", 1200, 600);
	c2->Divide(4,2);
	
	TText title; 
	title.SetTextAlign(21);
	title.SetTextSize(0.06);

	TLatex txt; 
	txt.SetTextSize(0.06);
	int color[2] = {kBlack, kRed};
	for( int i=0; i<4; i++)
	{
		double maxS0 = hSlope [0][i]->GetMaximum();
		double maxS1 = hSlope [1][i]->GetMaximum();
		double maxI0 = hYInter[0][i]->GetMaximum();
		double maxI1 = hYInter[1][i]->GetMaximum();
	  if( maxS0 < maxS1 ) hSlope[0][i]->SetMaximum( maxS1*1.2 );
		else hSlope[0][i]->SetMaximum( maxS0*1.2 );
	  if( maxI0 < maxI1 ) hYInter[0][i]->SetMaximum( maxI1*1.2 );
		else hYInter[0][i]->SetMaximum( maxI0*1.2 );

		for( int j=0; j<2; j++ ) 
		{
		  hSlope [j][i]->SetLineColorAlpha( color[j], 0.3 );
		  hYInter[j][i]->SetLineColorAlpha( color[j], 0.3 );
		  hSlope [j][i]->SetFillColorAlpha( color[j], 0.3 );
		  hYInter[j][i]->SetFillColorAlpha( color[j], 0.3 );

			txt.SetTextColorAlpha( color[j], 0.5 );

		  c2->cd(i+1);
		  hSlope[j][i]->Draw( j==0? "":"same" );
			if( j==0 ) title.DrawTextNDC(0.5, 0.92, Form("%s_slope",vParName.at(i).Data()) );
			txt.DrawLatexNDC( 0.15, 0.82-0.07*j, Form("chip%d: %3.2f#pm%3.2f", j, hSlope[j][i]->GetMean(), hSlope[j][i]->GetRMS()) );
		  
		  c2->cd(4+i+1);
		  hYInter[j][i]->Draw( j==0? "":"same" );
			if( j==0 ) title.DrawTextNDC(0.5, 0.92, Form("%s_Yintercept", vParName.at(i).Data()) );
			txt.DrawLatexNDC( 0.15, 0.82-0.07*j, Form("chip%d: %3.2f#pm%3.2f", j, hYInter[j][i]->GetMean(), hYInter[j][i]->GetRMS()) );
		}
		
		cout << Form("[Slope _%s] (underflow, overflow) => chip0: (%5.0f, %5.0f), chip1: (%5.0f, %5.0f)", vParName.at(i).Data(),
				        hSlope[0][i]->GetBinContent(0), hSlope[0][i]->GetBinContent(hSlope[0][i]->GetNbinsX()+1),
				        hSlope[1][i]->GetBinContent(0), hSlope[1][i]->GetBinContent(hSlope[1][i]->GetNbinsX()+1) ) << endl;
		cout << Form("[YInter_%s] (underflow, overflow) => chip0: (%5.0f, %5.0f), chip1: (%5.0f, %5.0f)", vParName.at(i).Data(),
				        hYInter[0][i]->GetBinContent(0), hYInter[0][i]->GetBinContent(hYInter[0][i]->GetNbinsX()+1),
				        hYInter[1][i]->GetBinContent(0), hYInter[1][i]->GetBinContent(hYInter[1][i]->GetNbinsX()+1) ) << endl;
	}
	
	TCanvas* c3 = new TCanvas("c3", "c3", 1200, 600);
	c3->Divide(4,2);
	double factor;
	for( int i=0; i<4; i++)
	{
		hSlopeAll [i]->SetMaximum( hSlopeAll [i]->GetMaximum()*1.2 );
		hYInterAll[i]->SetMaximum( hYInterAll[i]->GetMaximum()*1.2 );

		hSlopeAll [i]->SetLineColorAlpha( kAzure, 0.3 );
		hYInterAll[i]->SetLineColorAlpha( kAzure, 0.3 );
		hSlopeAll [i]->SetFillColorAlpha( kAzure, 0.3 );
		hYInterAll[i]->SetFillColorAlpha( kAzure, 0.3 );

		c3->cd(i+1);
		factor= i==0? 1E2:1;
		hSlopeAll[i]->Draw();
		if( i>0 && i<3 ) hSlopeAll [i]->Fit( "gaus" );
		title.DrawTextNDC(0.5, 0.92, Form("%s_slope",vParName.at(i).Data()) );
		txt.SetTextColorAlpha( color[0], 0.5 );
		txt.DrawLatexNDC( 0.15, 0.82, Form("chip0&1: %s%3.2f#pm%3.2f%s", factor==1? "":"(", hSlopeAll[i]->GetMean()*factor, hSlopeAll[i]->GetRMS()*factor, factor==1? "":")E-2") );
		if( gPad->GetPrimitive("gaus") ) {
			auto gausS = (TF1*) gPad->GetPrimitive("gaus");
			txt.SetTextColorAlpha( color[1], 0.5 );
		  txt.DrawLatexNDC( 0.15, 0.82-0.07, Form("Gaus fit: %s%3.2f#pm%3.2f%s", factor==1? "":"(", gausS->GetParameter(1)*factor, gausS->GetParameter(2)*factor, factor==1? "":")E-2") );
		}

		
		c3->cd(4+i+1);
		factor= i==0? 1E3:1;
		hYInterAll[i]->Draw();
		if( i>0 && i<3 ) hYInterAll[i]->Fit( "gaus" );
		title.DrawTextNDC(0.5, 0.92, Form("%s_Yintercept", vParName.at(i).Data()) );
		txt.SetTextColorAlpha( color[0], 0.5 );
		txt.DrawLatexNDC( 0.15, 0.82, Form("chip0&1: %s%3.2f#pm%3.2f%s", factor==1? "":"(", hYInterAll[i]->GetMean()*factor, hYInterAll[i]->GetRMS()*factor, factor==1? "":")E-3") );
		if( gPad->GetPrimitive("gaus") ) {
			auto gausI = (TF1*) gPad->GetPrimitive("gaus");
			txt.SetTextColorAlpha( color[1], 0.5 );
		  txt.DrawLatexNDC( 0.15, 0.82-0.07, Form("Gaus fit: %s%3.2f#pm%3.2f%s", factor==1? "":"(", gausI->GetParameter(1)*factor, gausI->GetParameter(2)*factor, factor==1? "":")E-3") );
		}
		
		cout << Form("[Slope _%s] (underflow, overflow) => chip0&1: (%5.0f, %5.0f)", vParName.at(i).Data(),
				        hSlopeAll[i]->GetBinContent(0), hSlopeAll[i]->GetBinContent(hSlopeAll[i]->GetNbinsX()+1) ) << endl;
		cout << Form("[YInter_%s] (underflow, overflow) => chip0&1: (%5.0f, %5.0f)", vParName.at(i).Data(),
				        hYInterAll[i]->GetBinContent(0), hYInterAll[i]->GetBinContent(hYInterAll[i]->GetNbinsX()+1) ) << endl;
	}

}

