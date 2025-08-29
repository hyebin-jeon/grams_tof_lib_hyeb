#include "TOF_Fitting.h"

ClassImp( TOF_Fitting );

/// ywant = percentage of the max height
std::array<Double_t,2> TOF_Fitting::GetX_withY( TH1* h, Double_t ywant )
{
	Int_t bin_ymax  = h->GetMaximumBin();
	Double_t ymax		= (Double_t) h->GetBinContent( bin_ymax );

	Double_t cut = ymax * ywant;

	std::vector<Double_t> xlist;
	xlist.clear();
	for( int i=1; i< h->GetNbinsX(); i++ )
	{
		Int_t y = h->GetBinContent(i);

		if( y < cut ) continue;

		Double_t x = h->GetBinCenter(i);
		xlist.push_back( x );
	}
	
	Double_t xmin = xlist.front();
	Double_t xmax = xlist.back();

	return {xmin, xmax};
}

double TOF_Fitting::getXatYmax( TH1* h )
{
	int binIdx_at_ymax = h->GetMaximumBin();
	double x_at_ymax = h->GetXaxis()->GetBinCenter( binIdx_at_ymax );

	return x_at_ymax;
}

//////////////////////////////////////////////////////////////////////////
		
void TOF_Fitting::fitGauss( TH1* h, double Nsigma )
{
	auto xrange =  GetX_withY( h, 0.01 );
	Double_t xmin = xrange[0];
	Double_t xmax = xrange[1];

	/// Guassian fFunction
	fFunc = new TF1("fFunc","gaus", xmin, xmax );
	fFunc->SetLineColor( kRed );
	fFunc->SetLineWidth( 1 );
	
	printf("[Attempt-1] Fit range: %5.1f, %5.1f\n", xmin, xmax );
	auto x_at_ymax = getXatYmax( h );
	fFunc->SetParameter( 1, x_at_ymax ); // mean
	h->Fit(fFunc);
	
	auto mean  = fFunc->GetParameter(1);
	auto sigma = fFunc->GetParameter(2);
	xmin = mean-sigma*Nsigma;
	xmax = mean+sigma*Nsigma;
	printf("[Attempt-2] Fit range: %5.1f, %5.1f\n", xmin, xmax );
	fFunc->SetRange( xmin, xmax );
	h->Fit( fFunc, "r" );

	mean  = fFunc->GetParameter(1);
	sigma = fFunc->GetParameter(2);
	xmin = mean-sigma*Nsigma;
	xmax = mean+sigma*Nsigma;
	printf("[Attempt-3] Fit range: %5.1f, %5.1f\n", xmin, xmax );
	fFunc->SetParameters( fFunc->GetParameters() );
	h->Fit(fFunc, "r");

	return;
}
