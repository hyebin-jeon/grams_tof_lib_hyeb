
std::vector<TString> vParName_tdc = {"t0", "a0", "a1", "a2"};
void getFitFn_TDC( const char* fname, std::vector<TF1*> *vFitFn );
void getFitFn_QDC( const char* fname, std::vector<TF1*> *vFitFn );

void tempDep_calParams_fit()
{
	const char* fname_tdc = "output/calib_par_temp_TDC.root" ;
	const char* fname_qdc = "output/calib_par_temp_QDC.root" ;

	//TFile *fin_tdc = new TFile( fname_tdc );
	//TFile *fin_qdc = new TFile( fname_qdc );

	std::vector< TF1* > vFitFn_tdc;
	std::vector< TF1* > vFitFn_qdc;

	getFitFn_TDC( fname_tdc, &vFitFn_tdc );
	getFitFn_QDC( fname_qdc, &vFitFn_qdc );

	for( auto fit: vFitFn_tdc )
	{
		double p0 = fit->GetParameter(0);
		double p1 = fit->GetParameter(1);
		cout << fit->GetName() << Form(" --> p0: %6.2f, p1: %6.2f", p0, p1) << endl;
	}

	for( auto fit: vFitFn_qdc )
	{
		double p0 = fit->GetParameter(0);
		double p1 = fit->GetParameter(1);
		cout << fit->GetName() << Form(" --> p0: %6.2f, p1: %6.2f", p0, p1) << endl;
	}

}


void getFitFn_TDC( const char* fname, std::vector<TF1*> *vFitFn )
{
	TFile *fin = new TFile( fname, "read" ); 
	fin->cd();
	TIter next(fin->GetListOfKeys());
  TKey *key;

	TCanvas* c1 = new TCanvas("c1_tdc", "tdc", 600, 600);
	c1->Divide(2,2);
	int idx = 0;
	while( (key = (TKey *)next()) )
	{
		idx++;
    TGraph *gr = (TGraph*) fin->Get( key->GetName() );
    //cout << " Name " << obj->GetName() << endl;
    //cout << " Title " << obj->GetTitle() << endl;

		c1->cd( idx );
		TFitResultPtr* r = gr->Fit( "pol1", "s" );
		gr->Draw("alp");

		TF1* fit = (TF1*) gPad->GetPrimitive("pol1");
		//fit->SetName( Form("%s_fit", key->GetName()) );
		vFitFn->push_back( fit );
  }

	return;
}

void getFitFn_QDC( const char* fname, std::vector<TF1*> *vFitFn )
{
	TFile *fin = new TFile( fname, "read" ); 
	fin->cd();
	TIter next(fin->GetListOfKeys());
  TKey *key;

	TCanvas* c1 = new TCanvas("c1_qdc", "qdc", 1200, 600);
	c1->Divide(5,2);
	int idx = 0;
	while( (key = (TKey *)next()) )
	{
		idx++;
    TGraph *gr = (TGraph*) fin->Get( key->GetName() );

		c1->cd( idx );
		gr->Fit( "pol1" );
		gr->Draw("alp");

		TF1* fit = (TF1*) gPad->GetPrimitive("pol1");
		//fit->SetName( Form("%s_fit", key->GetName()) );
		vFitFn->push_back( fit );
  }

	return;
}
