auto theAttrib = TOF_Attributes::getInstance();
auto theCalib  = TOF_TdcQdcCalibration::getInstance();
auto theChanConv = TOF_ChannelConversion::getInstance();

void plotQdcCalib()
{

	const char* dirName   = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20250903_QDC_vs_ToT";
  const char* finName   = "test_qdc_raw.root";
	const char* fQcalName = "qdc_calibration_00_00_04.root";

	const char* fQcalPath = Form("%s/%s", dirName, fQcalName);
	const char* finPath   = Form("%s/%s", dirName, finName  );

	TFile* fQcal = new TFile( fQcalPath, "read" );
	TFile* fin   = new TFile( finPath  , "read" );
	
	if( !fQcal->IsOpen() ) {
		printf("[ERR] FILE NOT FOUND: %s", fQcalPath);
		return;
	}
	if( !fin->IsOpen() ) {
		printf("[ERR] FILE NOT FOUND: %s", finPath);
		return;
	}

	theCalib->readCalibrationFiles( "../calibration/20250903" );

	/// reference histo
	auto pRef = (TProfile*) fQcal->Get("c_00_00_04_00_0_pFine"); // c_portID_slaveID_chipID_chanID_tacID


	TOF_TreeData* t = (TOF_TreeData*) fin->Get("data");
	if( !t ) return;
	t->setBranchAddress();

	TH2S* hData = new TH2S("hData", "", 100, 50, 70, 100, 100, 140);
	//TH2S* hData = new TH2S("hData", "", 1024, 0, 1024, 500, 0, 500);
	hData->SetTitle(";tEnd-ti (clocks);Qcal");

	auto chanRef = theChanConv->getAbsoluteChannelID( 0, 0, 4, 0 ); // from hQcal

	int entries = t->GetEntries();
	for( int i=0; i<entries; i++ )
	{
		t->GetEntry(i);

		auto achanID   = t->getChannelID(); // absolute channel ID

		if( achanID != chanRef ) continue;

		auto connID    = t->getConnIdOnFebD   ( achanID );
		auto asicID    = t->getAsicID         ( achanID );
		auto channelID = t->getChannelIdOnFebS( achanID );  // 1-128
		auto tacID     = t->getTacID();

		auto frameID = t->getFrameID();
		auto tCoarse = t->getTCoarse();
		auto eCoarse = t->getECoarse();
		auto tFine   = t->getTFine()  ;
		auto eFine   = t->getEFine()  ;

		/// current time in clock
		//auto currTime = CLOCKS_IN_A_FRAME * frameID + tCoarse;

		auto time0 = theCalib->getCalibratedTime( TOF_Mode::fQdc, achanID, tacID, frameID, tCoarse, tFine );
		auto time1 = double((frameID*1024+eCoarse));
		auto tintg = time1 - time0;
		auto qcal = theCalib->getCalibratedQDC( achanID, tacID, frameID, eCoarse, eFine, tCoarse, time0  );

		hData->Fill( tintg, qcal );
	}

	gStyle->SetOptStat(111111);
	TCanvas* c1 = new TCanvas("c1", "c1", 1500, 500 );
	c1->Divide(3,1);
	
	c1->cd(1);
	hData->GetZaxis()->SetLabelSize(0.03);
	hData->Draw("colz");
	gPad->Modified(); gPad->Update();
	auto ps1 = (TPaveStats*) gPad->GetPrimitive("stats");
	theAttrib->moveStatBoxNDC( ps1, 0.7, 0.11, 0.9, 0.45);

	c1->cd(2);
	auto pData = hData->ProfileX("profData", 1, -1, "s"); 
	pData->SetMarkerColor(kMagenta);
	pData->SetLineColor(kMagenta);
	pData->SetMarkerStyle(20);
	pData->SetMarkerSize(0.5);

	pRef->SetName("pRef");
	pRef->SetTitle(";tEnd-ti (clocks);");
	pRef->GetXaxis()->SetRange(0, 300);
	pRef->Draw();
	pData->Draw("same");
	
	gPad->Modified(); gPad->Update();
	auto ps2_ref = (TPaveStats*) pRef ->GetListOfFunctions()->FindObject("stats");
	theAttrib->moveStatBoxNDC( ps2_ref, 0.49, 0.11, 0.69, 0.45);

	c1->cd(3);
	pData->Draw();
	pData->SetMaximum(170);
	pData->SetMinimum(70);
	pRef->Draw("same");

	gPad->Modified(); gPad->Update();
	auto ps2_dat = (TPaveStats*) pData->GetListOfFunctions()->FindObject("stats");
	theAttrib->moveStatBoxNDC( ps2_dat, 0.70, 0.11, 0.90, 0.45);

	c1->Modified();
	c1->Update();
}
