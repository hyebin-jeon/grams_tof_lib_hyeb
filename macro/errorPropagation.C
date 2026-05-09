/// 2025.01.02 Hyebin
/// assuming a linear fitting on the TDC calibration data --> t_calib = ( frameID*1024 + t_coarse ) - (t_fine - a0)/a1, where a0 and a1 are functions of temperature, T.
/// a0 = p00 + p01*T, where p00=-0.24+-0.09 and p01 = 146.36+-7.20
/// a1 = p10 + p11*T, where p10=-0.26+-0.05 and p11 = 168.58+-6.82

auto theAttrib   = TOF_Attributes::getInstance();

double calc_errorPropagation( double temp, double tfine );

void errorPropagation()
{
	double tfine = 2.0;

	double tphase_min = 0;
	double tphase_max = 8;
	double tphase_step = 0.5;
	int    tphase_nb = (int) (tphase_max - tphase_min)/tphase_step + 1;

	std::vector<TGraph*> vGr;
	std::vector<TGraph*> vGrT;
	for( int i=0; i<tphase_nb; i++ )
	{
		TGraph* g = new TGraph();
	  g->SetTitle( Form("tphase_%1.1f;ASIC Temp (C);d(TDC)", ((double)i)*tphase_step) );
	  g->SetLineWidth(2);
	  g->SetMarkerStyle(20);
	  //g->SetLineColorAlpha( kBlue, 0.5 );
	  //g->SetMarkerColorAlpha( kBlue, 0.5 );
		vGr.push_back( g );

		TGraph* gT = new TGraph();
	  gT->SetTitle( Form("dT_tphase_%1.1f;ASIC Temp (C);d(TDC)", ((double)i)*tphase_step) );
	  gT->SetLineWidth(2);
	  gT->SetMarkerStyle(20);
		vGrT.push_back( gT );
	}

	for( int j=0; j<tphase_nb; j++ )
	{
		double tfine_ = ((double)j)*tphase_step;

	  for( int i =0; i<60; i++ )
	  {
	  	double temp = (double) i+1;
	  	double err = calc_errorPropagation( temp, tfine_ );
	  	vGr.at(j)->SetPoint( i, temp, err );

			double dt = 4.*err*err/2.;
			vGrT.at(j)->SetPoint(i, temp, dt);
	  }
	}

	TCanvas* c1 = new TCanvas("c1", "error propagation");
	TMultiGraph* mg = new TMultiGraph();
	mg->SetTitle(";ASIC Temp (C);d(TDC)");
	for( int j=0; j<tphase_nb; j++ ) mg->Add( vGr.at(j) );
	mg->Draw("apl plc pmc");
	mg->GetHistogram()->GetYaxis()->SetTitleOffset(1.45);
	auto leg = gPad->BuildLegend( 0.7, 0.15, 0.9, 0.75);
  leg->SetFillStyle (0);
  leg->SetBorderSize(0);
	theAttrib->setAttribLegendEntries(leg, "marker");

	TCanvas* c2 = new TCanvas("c2", "calc err of time resocalc err of a single dT");
	TMultiGraph* mgT = new TMultiGraph();
	mgT->SetTitle(";ASIC Temp (C);d(TDC)");
	for( int j=0; j<tphase_nb; j++ ) mgT->Add( vGrT.at(j) );
	mgT->Draw("apl plc pmc");
	mgT->GetHistogram()->GetYaxis()->SetTitleOffset(1.45);
	auto legT = gPad->BuildLegend( 0.7, 0.15, 0.9, 0.75);
  legT->SetFillStyle (0);
  legT->SetBorderSize(0);
	theAttrib->setAttribLegendEntries(legT, "marker");

	return;
}



//double calc_errorPropagation( double temp = 22., double tfine = 2.0 )
double calc_errorPropagation( double temp, double tfine )
{
	double T = temp;

	double a0_p0  = -0.24  ; // p00
	double a0_dp0 =  0.09  ; 
	double a0_p1  =  146.36; // p01
	double a0_dp1 =  7.20  ;
	double a1_p0  = -0.26  ; // p10
	double a1_dp0 =  0.05  ; 
	double a1_p1  =  168.58; // p11
	double a1_dp1 =  6.82  ;
	
	double a0 = a0_p0 + a0_p1*T;
	double a1 = a1_p0 + a1_p1*T;

	double da0_2 = pow(a0_dp0,2) + pow(a0_dp1,2)*T*T; // (da0)^2
	double da1_2 = pow(a1_dp0,2) + pow(a1_dp1,2)*T*T; // (da1)^2

	/// d(t_calib)^2
	double dt2 = da0_2/pow(a1,2) + da1_2 * pow( (tfine-a0)/a1/a1, 2);

	//cout << Form("At temp = %2.1f, the propagated uncertainty of the calibrated TDC = %3.2f", T, sqrt(dt2)) << endl;

	return sqrt(dt2);
}




