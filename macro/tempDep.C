///
/// Draw hResolution* or hCount*
///
#include <filesystem>
namespace fs = std::filesystem;

auto theCalib    = TOF_TdcQdcCalibration::getInstance();
auto theChanConv = TOF_ChannelConversion::getInstance();
auto theAttrib   = TOF_Attributes::getInstance();
	
struct kFilePath{
	int idx;
	int temp;
	TString fpath;
};
struct kHistNameID{
	int portID;
	int slaveID;
	int chipID;
	int chanID;
	int tacID;
	char br;
	TString suffix;
};

void readCalibParams();
void getPathList( std::vector< kFilePath > *vPath );
void breakRootName( TString kRootName, kHistNameID *rnameID );

vector<double> asic0T =  {29.55, 32.07, 34.75, 37.27, 39.50, 41.89, 44.34, 42.89, 40.78, 38.21, 35.87, 33.47, 31.06};
vector<double> asic1T =  {31.28, 33.80, 36.43, 38.49, 41.22, 43.45, 45.95, 44.61, 42.50, 40.00, 37.66, 35.20, 32.85};

///==============================================================================///
void tempDep(bool isTDC=kTRUE, bool isAsicTemp=kTRUE, const char* hname= "hResolution_T") // TDC T branch
//void tempDep(bool isTDC=kTRUE, bool isAsicTemp=kTRUE, const char* hname= "hCount_T") // TDC T branch
//void tempDep(bool isTDC=kFALSE, bool isAsicTemp=kTRUE, const char* hname= "hResolution" ) // QDC
{
	gStyle->SetOptStat(0);
	gROOT->ForceStyle();
	gStyle->SetPalette(55);
	auto cols = TColor::GetPalette();
	std::vector<int> marker = {24, 25, 26, 32, 27, 28, 30, 35, 37, 40, 42, 46, 49, 20, 21, 22, 29, 33, 34};

	//const char* tdcName0= "tdc_calibration_00_00_00.root";
	//const char* tdcName1= "tdc_calibration_00_00_01.root";
	TString tdcName0= Form("%s_calibration_00_00_00.root", isTDC? "tdc":"qdc" );
	TString tdcName1= Form("%s_calibration_00_00_01.root", isTDC? "tdc":"qdc" );
	kHistNameID rnameID0;
	kHistNameID rnameID1;
	breakRootName( tdcName0, &rnameID0 );
	breakRootName( tdcName1, &rnameID1 );

	bool isResol = 0;
	if( ((TString) hname).Contains("Resol") ) isResol=1;

	std::vector<TH1S*> vTdcResol0;
	std::vector<TH1S*> vTdcResol1;

	std::vector<kFilePath> vPath;
	getPathList( &vPath );

	int col =1;
	double mkSize = isResol? 1.3:0.3;
	auto leg = new TLegend(0.67, 0.15, 0.89, 0.85, "");

	TString tempStr = isAsicTemp? "asic temp" : "set temp";

	TGraphErrors* gr0 = new TGraphErrors();
	TGraphErrors* gr1 = new TGraphErrors();
	gr0->SetName( Form("gResol_chip%02d_%s", rnameID0.chipID, isTDC? "TDC":"QDC") );
	gr1->SetName( Form("gResol_chip%02d_%s", rnameID1.chipID, isTDC? "TDC":"QDC") );
	gr0->SetTitle( Form("[%s] chip%02d;%s (C);%s resolution", isTDC? "TDC":"QDC", rnameID0.chipID, tempStr.Data(), isTDC? "TDC":"QDC") );
	gr1->SetTitle( Form("[%s] chip%02d;%s (C);%s resolution", isTDC? "TDC":"QDC", rnameID1.chipID, tempStr.Data(), isTDC? "TDC":"QDC") );

	leg->SetFillStyle( 0 );
  leg->SetBorderSize(0);
	int temp0;
	for( auto path: vPath )
	{
		auto idx = path.idx;
		auto temp = path.temp;
		auto fdir = path.fpath;

		if( idx==0 ) temp0= temp;

		TString fPath0 = Form( "%s/%s", fdir.Data(), tdcName0.Data() );
		TString fPath1 = Form( "%s/%s", fdir.Data(), tdcName1.Data() );

		auto h0 = (TH1S*) (new TFile(fPath0.Data()))->Get(hname);
		auto h1 = (TH1S*) (new TFile(fPath1.Data()))->Get(hname);

		h0->SetName( Form("hR_chip00_%02d_%02dC", idx, temp) );
		h1->SetName( Form("hR_chip01_%02d_%02dC", idx, temp) );

		cout << fPath0 << " => h->GetEntries()= " << h0->GetEntries() << "h name: " << h0->GetName() << endl;
		cout << fPath1 << " => h->GetEntries()= " << h1->GetEntries() << "h name: " << h1->GetName() << endl;
		
		theAttrib->attribHist( h0, cols.At( col*15 ), 0.1 );
		theAttrib->attribHist( h1, cols.At( col*15 ), 0.1 );

		h0->SetMarkerStyle( marker.at(0 + (temp-temp0)/3) );
		h1->SetMarkerStyle( marker.at(0 + (temp-temp0)/3) );

		h0->SetMarkerSize( mkSize );
		h1->SetMarkerSize( mkSize );

		vTdcResol0.push_back( h0 );
		vTdcResol1.push_back( h1 );

		leg->AddEntry( h0, Form("%02d_%02dC", idx, temp), "lp");

		auto gTemp0 = isAsicTemp? asic0T.at(idx) : temp;
		auto gTemp1 = isAsicTemp? asic1T.at(idx) : temp;
		gr0->AddPointError( gTemp0, h0->GetMean(), 0, h0->GetRMS() );
		gr1->AddPointError( gTemp1, h1->GetMean(), 0, h1->GetRMS() );

		col++;
	}

	THStack* hR0 = new THStack("chip00", "");
	THStack* hR1 = new THStack("chip01", "");
	for( auto h: vTdcResol0 ) hR0->Add( h );
	for( auto h: vTdcResol1 ) hR1->Add( h );

	TString pdf = Form("output/tempDep_%s_%s.pdf", hname, isTDC? "TDC":"QDC");

	TCanvas* c1 = new TCanvas("c1", "c1", 1200, 500);
	c1->Divide(2,1);
	c1->cd(1);
	hR0->SetTitle( Form("Chip00;%s Resolution;", isTDC? "TDC":"QDC"));
	if( isResol ) {
		hR0->Draw("nostack");
	  hR0->Draw("nostack p same");
	} else {
	  hR0->Draw("nostack p");
	}
	auto x0 = hR0->GetXaxis();
	if( ((TString) hname).Contains("Resol") ) {
		if( isTDC ) x0->SetRange(x0->FindBin(0.002), x0->FindBin(0.012));
		else        x0->SetRange(x0->FindBin(0.), x0->FindBin(2.));
	}
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	c1->cd(2);
	hR1->SetTitle( Form("Chip01;%s Resolution;", isTDC? "TDC":"QDC"));
	if( isResol ) {
		hR1->Draw("nostack");
	  hR1->Draw("nostack p same");
	} else {
	  hR1->Draw("nostack p");
	}

	auto x1 = hR1->GetXaxis();
	if( ((TString) hname).Contains("Resol") ) {
		if( isTDC ) x1->SetRange(x1->FindBin(0.002), x1->FindBin(0.012));
		else        x1->SetRange(x1->FindBin(0.), x1->FindBin(2.));
	}
	leg->Draw();
	gPad->Modified();
	gPad->Update();

	c1->Print(Form("%s(", pdf.Data()) );

	TCanvas* c2 = new TCanvas("c2", "c2", 1200, 500);
	c2->Divide(2,1);

	c2->cd(1); 
	gr0->Draw("al ||");

	TText txt;
	txt.SetTextSize(0.05);
	TMarker mk;
	mk.SetMarkerStyle(20);
	mk.SetMarkerSize(1.5);
	TLine line;
	line.SetLineWidth(2);
	for( int j=0; j<gr0->GetN(); j++ )
	{
	  auto x = gr0->GetPointX(j);
	  auto y = gr0->GetPointY(j);
	  auto yerrH = gr0->GetErrorYhigh(j);
	  auto yerrL = gr0->GetErrorYlow (j);
			
		line.SetLineColorAlpha( cols.At(j*15), 0.7 );
		line.DrawLine( x, y-yerrL, x, y+yerrH );

		mk.SetMarkerColorAlpha( cols.At(j*15), 0.7 );
		mk.DrawMarker(x, y);
			
		if( j< 6 ) txt.SetTextAlign(13);
		else txt.SetTextAlign(11);
		txt.SetTextColorAlpha( cols.At(j*15), 0.7 );
		txt.DrawText( x, y, Form("%02d", j) );
	}

	c2->cd(2); 
	gr1->Draw("al ||");
	for( int j=0; j<gr1->GetN(); j++ )
	{
	  auto x = gr1->GetPointX(j);
	  auto y = gr1->GetPointY(j);
	  auto yerrH = gr1->GetErrorYhigh(j);
	  auto yerrL = gr1->GetErrorYlow (j);
			
		line.SetLineColorAlpha( cols.At(j*15), 0.7 );
		line.DrawLine( x, y-yerrL, x, y+yerrH );

		mk.SetMarkerColorAlpha( cols.At(j*15), 0.7 );
		mk.DrawMarker(x, y);
			
		if( j< 6 ) txt.SetTextAlign(13);
		else txt.SetTextAlign(11);
		txt.SetTextColorAlpha( cols.At(j*15), 0.7 );
		txt.DrawText( x, y, Form("%02d", j) );
	}
	
	c2->Print(Form("%s)", pdf.Data()) );

}
///==============================================================================///

void getPathList( std::vector< kFilePath > *vPath )
{

	std::string home = std::getenv("HOME");
	std::string dirName = home+"/Box/GRAMS/TOF_MPD/0_TestOutputs/20251002_UChicago_AsicTempTest/20251001_Peltier";

	kFilePath fpath;

	for( const auto& entry: fs::directory_iterator(dirName) )
	{
		if( entry.is_directory())
		{
			TString subdir = (TString) entry.path().filename();
	  
			int tidx0 = subdir.Last('_');
			int tidx1 = subdir.Last('C');
	    int len   = 2;

	    int temp = ((TString) subdir(tidx1-len, len)).Atoi();
	    int idx  = ((TString) subdir(tidx0-len, len)).Atoi();

			TString fPath = Form( "%s/%s", dirName.c_str(), subdir.Data() );
			//cout << idx << ", " << fPath << endl;
			
			fpath.idx = idx;
			fpath.temp = temp;
			fpath.fpath = fPath;

			vPath->push_back( fpath );

		}
	}
	std::sort( vPath->begin(), vPath->end(), [](const kFilePath& a, const kFilePath& b) { return a.idx < b.idx; });

};
///==============================================================================///

void breakRootName( TString kRootName, kHistNameID *rnameID)
{
	int  chipLen = 2;
	auto chipIdx = kRootName.Index(".root") - chipLen;
	auto chipIDstr = (TString) kRootName( chipIdx, chipLen);
	int  chipID = chipIDstr.Atoi();
	//cout << "chip ID: " << chipIDstr << " => (int) " << chipID  << endl;

	int  slaveLen = 2;
	auto slaveIdx = chipIdx - slaveLen - 1;
	auto slaveIDstr = (TString) kRootName( slaveIdx, slaveLen);
	int  slaveID = slaveIDstr.Atoi();
	//cout << "slave ID: " << slaveIDstr << " => (int) " << slaveID  << endl;

	int  portLen = 2;
	auto portIdx = slaveIdx - slaveLen - 1;
	auto portIDstr = (TString) kRootName( portIdx, portLen);
	int  portID = portIDstr.Atoi();
	//cout << "port ID: " << portIDstr << " => (int) " << portID  << endl;

	rnameID->portID  = portID ;
	rnameID->slaveID = slaveID;
	rnameID->chipID  = chipID ;
	rnameID->chanID  = -1     ; // dummy
	rnameID->tacID   = -1     ; // dummy
	rnameID->br      = 'T'    ; // dummy

	return;
}



void readCalibParams()
{
  
	/// get the file paths
	std::string home = std::getenv("HOME");
	std::string dirName = home+"/Box/GRAMS/TOF_MPD/0_TestOutputs/20251002_UChicago_AsicTempTest/20251001_Peltier";
	const char* tdcName = "tdc_calibration.tsv";


	int testN =0;
	for( const auto& entry: fs::directory_iterator(dirName) )
	{
		if( entry.is_directory())
		{
			TString subdir = (TString) entry.path().filename();
	  
			int tidx0 = subdir.Last('_');
			int tidx1 = subdir.Last('C');
	    int len   = 2;

	    double temp = ((TString) subdir(tidx1-len, len)).Atof();
	    double idx  = ((TString) subdir(tidx0-len, len)).Atof();

			//cout << subdir;
			//cout << "==> temp: " << temp << ", idx: " << idx << endl;

		  TString fPath = Form( "%s/%s/%s", dirName.c_str(), subdir.Data(), tdcName );
			cout << fPath << endl;

	    theCalib->readTdcCalib( fPath.Data() );  
			
			uint32_t achanID = 0;
			theCalib->printTdcCalibTable( achanID );

			uint8_t tac = 0;
			auto vParam = theCalib->getTdcParams_T( achanID, tac );

			testN++;

			if( testN > 0 ) break;

		}
	}
}	
///==============================================================================///
