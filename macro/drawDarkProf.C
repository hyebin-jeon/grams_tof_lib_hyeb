
void drawDarkProf()
{
	auto theChanConv = TOF_ChannelConversion::getInstance();

	const char* fname = "/Users/hjeon7/work/grams/software/grams_tof_lib_hyeb/macro/output/darkCount_2026-01-28_22-56-58.17Z.root";
	TFile* fin = new TFile( fname );

	std::vector<TProfile*> vProf;
	std::vector<uint32_t> vChannel = { 452, 461, 450, 459, 463,	455 };
	std::sort( vChannel.begin(), vChannel.end() );

	TCanvas* c00 = new TCanvas("c00", "c00");
	TText txt;
  
	int febD_connID = 4;
	uint32_t achanID = theChanConv->getAbsoluteChannelID( febD_connID, 77 );
	cout << "pulse channel: " << achanID << endl;
	auto profP = (TProfile*) fin->Get( Form("vth_t1/pf_ch%03d_vth_t1", achanID) );
	profP->SetLineWidth(2);
	profP->SetLineColor(2);
	profP->SetMarkerColor(2);
	profP->Draw();
	profP->GetYaxis()->SetRangeUser(0,400);
	gPad->Modified();
	gPad->Update();

	int i=0;
  int color;
	 for( auto achan: vChannel )
	{
	   auto febSID = theChanConv->getConnIdOnFebS(achan);

		 auto prof = (TProfile*) fin->Get( Form("vth_t1/pf_ch%03d_vth_t1", achan) );
		 
		 if( achan == achanID ) color = 2;
		 else color = 3+i;
		 cout << "channel: " << achan << ", color: " << color << endl;

		 c00->cd();
		 prof->SetLineWidth(2);
		 prof->SetLineColorAlpha  ( color, 0.7 );
		 prof->SetMarkerColorAlpha( color, 0.7 );

		 txt.SetTextColor( color );
		 txt.DrawTextNDC( 0.12, 0.85-0.05*i, Form("Ch%03d (J%03d)", achan, theChanConv->getConnIdOnFebS(achan)) );

		 prof->Draw("same");

		 i++;
	}

	profP->Draw("same");

	gPad->Modified();
	gPad->Update();
}
