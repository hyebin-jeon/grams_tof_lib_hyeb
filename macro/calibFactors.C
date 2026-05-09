
auto theAttrib = TOF_Attributes::getInstance();
auto theCalib  = TOF_TdcQdcCalibration::getInstance();
auto theChanConv = TOF_ChannelConversion::getInstance();

void calibFactors()
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

	uint8_t tacID = 0;
	auto achanID = theChanConv->getAbsoluteChannelID( 0, 0, 4, 0 ); // from hQcal

	theCalib->readCalibrationFiles( "../calibration/20250903" );
	theCalib->printQdcCalibTable( achanID );
	
	auto pRef = (TProfile*) fQcal->Get("c_00_00_04_00_0_pFine"); // c_portID_slaveID_chipID_chanID_tacID
	double xmin = pRef->GetXaxis()->GetXmin();
	double xmax = pRef->GetXaxis()->GetXmax();
	
	auto params = theCalib->getQdcParams( achanID, tacID );

	int polN = 9;
	TF1* fn0 = new TF1("fn0", Form("pol%d",polN), xmin, xmax );
	fn0->SetLineColor(kGreen);
	fn0->SetLineWidth(2);
	for( int i=0; i<polN+1; i++ )
	  fn0->SetParameter(i, params.at(i) );

	double fitXmin= 50;
	double fitXmax= 90;
	double ymin = -150;
	double ymax =  450;
	TCanvas* c1 = new TCanvas("c1", "c1");
	auto xmaxBin = pRef->GetXaxis()->FindBin(250);
	pRef->GetXaxis()->SetRange(0, xmaxBin);
	pRef->SetMaximum( ymax );
	pRef->SetMinimum( ymin );
	pRef->Draw();
	//gPad->SetGrid();

	/// fitting range highlighter box
	TBox box;
	box.SetFillColorAlpha( kYellow, 0.3 );
	box.DrawBox( fitXmin, ymin, fitXmax, ymax ); 
	
	fn0->Draw("same");

	TF1* fn1 = new TF1("fn1", "pol1", xmin, xmax );
	fn1->SetLineColor(kRed);
	fn1->SetLineWidth(2);

	pRef->Fit("fn1", "", "same goff", fitXmin, fitXmax);
	fn1->Draw("same");

	cout << Form("The liner function fitted on (%.1f, %.1f): ", fitXmin, fitXmax ) << endl;
	cout << Form("slope: %.2f", fn1->GetParameter(1)) << endl;
	cout << Form("intercept: %.2f", fn1->GetParameter(0)) << endl;

	//TLine line;
	//line.SetLineStyle(2);
	//line.DrawLine( 50, 0, 50, fn1->Eval(50) );
	//line.DrawLine( 90, 0, 90, fn1->Eval(90) );

	c1->Print("output/calibFactors.png");
}


