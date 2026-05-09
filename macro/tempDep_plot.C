///
/// Draw a calibration scan and its fitting function
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
void initializeHistNameStr( kHistNameID* hnameID );
void breakHistName( TString kHistName, kHistNameID *hnameID, bool isTDC=kTRUE);
void breakRootName( TString kRootName, kHistNameID *rnameID );

///==============================================================================///
void tempDep_plot( bool isTDC=kTRUE, TString kHistName= "c_00_00_00_00_0_E_pFine_X" ) // TDC
//void tempDep_plot( bool isTDC=kFALSE, TString kHistName= "c_00_00_00_00_0_pFine" ) // QDC
{
	gStyle->SetOptStat(0);
	gROOT->ForceStyle();
	gStyle->SetPalette(55);
	auto cols = TColor::GetPalette();
	std::vector<int> marker = {24, 25, 26, 32, 27, 28, 30, 35, 37, 40, 42, 46, 49, 20, 21, 22, 29, 33, 34};

	TString tdcName0= Form("%s_calibration_00_00_00.root", isTDC? "tdc":"qdc" );
	TString tdcName1= Form("%s_calibration_00_00_01.root", isTDC? "tdc":"qdc" );
	
	kHistNameID hnameID;
	breakHistName( kHistName, &hnameID, isTDC );
	
	kHistNameID rnameID0;
	kHistNameID rnameID1;
	breakRootName( tdcName0, &rnameID0 );
	breakRootName( tdcName1, &rnameID1 );

	const char* psuffix = hnameID.suffix.Data();
	const char* hsuffix = "control_E";

	const char* fitFunc = isTDC? "periodicF2" : "pol8" ;


	if( hnameID.portID  != rnameID0.portID  ) cout << "BAAAAD 0. portID"  << endl;
	if( hnameID.slaveID != rnameID0.slaveID ) cout << "BAAAAD 0. slaveID" << endl;
	if( hnameID.chipID  != rnameID0.chipID  ) cout << "BAAAAD 0. chipID"  << endl;
	if( hnameID.portID  != rnameID1.portID  ) cout << "BAAAAD 1. portID"  << endl;
	if( hnameID.slaveID != rnameID1.slaveID ) cout << "BAAAAD 1. slaveID" << endl;
	if( hnameID.chipID+1  != rnameID1.chipID  ) cout << "BAAAAD 1. chipID"  << endl;

	const char* brStr =  hnameID.br=='\0'? Form("%s", "") : Form("%c_",hnameID.br);

	TString pname0 = Form("c_%02d_%02d_%02d_%02d_%01d_%s%s", hnameID.portID, hnameID.slaveID, rnameID0.chipID, hnameID.chanID, hnameID.tacID, brStr, psuffix);
	TString pname1 = Form("c_%02d_%02d_%02d_%02d_%01d_%s%s", hnameID.portID, hnameID.slaveID, rnameID1.chipID, hnameID.chanID, hnameID.tacID, brStr, psuffix);
	TString hname0 = Form("c_%02d_%02d_%02d_%02d_%01d_%s%s", hnameID.portID, hnameID.slaveID, rnameID0.chipID, hnameID.chanID, hnameID.tacID, brStr, hsuffix);
	TString hname1 = Form("c_%02d_%02d_%02d_%02d_%01d_%s%s", hnameID.portID, hnameID.slaveID, rnameID1.chipID, hnameID.chanID, hnameID.tacID, brStr, hsuffix);

	std::vector<kFilePath> vPath;
	getPathList( &vPath );

	std::vector<TProfile*> vProf;
	std::vector<TH1S*> vHist0;
	std::vector<TH1S*> vHist1;

	int col =1;
	int mk=0;
	double mkSize = 0.6;

	double leg_x0 = 0.75;
	double leg_x1 = 0.92;
	double leg_y0 = isTDC? 0.26: 0.13;
	double leg_y1 = isTDC? 0.88: 0.75;
	auto leg = new TLegend( leg_x0, leg_y0, leg_x1, leg_y1, "");

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

		if( gSystem->AccessPathName( fPath0.Data() ) ) {
			cout << "File Not Found: " << fPath0 << endl;
			continue;
		}
		if( gSystem->AccessPathName( fPath1.Data() ) ) {
			cout << "File Not Found: " << fPath1 << endl;
			continue;
		}

		auto p0 = (TProfile*) (new TFile(fPath0.Data()))->Get(pname0.Data());
		auto p1 = (TProfile*) (new TFile(fPath1.Data()))->Get(pname1.Data());
		auto h0 = (TH1S*) (new TFile(fPath0.Data()))->Get(hname0.Data());
		auto h1 = (TH1S*) (new TFile(fPath1.Data()))->Get(hname1.Data());

		if( !p0 ) {
			cout << "[ERR] " << fPath0 << ": [0] " << p0->GetName() << endl;
			continue;
		}
		if( !p1 ) {
			cout << "[ERR] " << fPath1 << ": [1] " << p1->GetName() << endl;
			continue;
		}
		if( !h0 ) {
			cout << "[ERR] " << fPath0 << ": [0] " << h0->GetName() << endl;
			continue;
		}
		if( !h1 ) {
			cout << "[ERR] " << fPath1 << ": [1] " << h1->GetName() << endl;
			continue;
		}
		
		//cout << fPath0 << " => p0->GetEntries()= " << p0->GetEntries() << " | p0 name: " << p0->GetName() << endl;
		//cout << fPath1 << " => p1->GetEntries()= " << p1->GetEntries() << " | p1 name: " << p1->GetName() << endl;

		p0->SetName( Form("%s_chip%02d_ch%02d_tac%d_%s%02d_%02dC", psuffix, rnameID0.chipID, hnameID.chanID, hnameID.tacID, brStr, idx, temp) );
		p1->SetName( Form("%s_chip%02d_ch%02d_tac%d_%s%02d_%02dC", psuffix, rnameID1.chipID, hnameID.chanID, hnameID.tacID, brStr, idx, temp) );
		h0->SetName( Form("%s_chip%02d_ch%02d_tac%d_%s%02d_%02dC", hsuffix, rnameID0.chipID, hnameID.chanID, hnameID.tacID, brStr, idx, temp) );
		h1->SetName( Form("%s_chip%02d_ch%02d_tac%d_%s%02d_%02dC", hsuffix, rnameID1.chipID, hnameID.chanID, hnameID.tacID, brStr, idx, temp) );

		cout << fPath0 << " => p0->GetEntries()= " << p0->GetEntries() << " | p0 name: " << p0->GetName() << endl;
		cout << fPath1 << " => p1->GetEntries()= " << p1->GetEntries() << " | p1 name: " << p1->GetName() << endl;
		cout << fPath0 << " => h0->GetEntries()= " << h0->GetEntries() << " | h0 name: " << h0->GetName() << endl;
		cout << fPath1 << " => h1->GetEntries()= " << h1->GetEntries() << " | h1 name: " << h1->GetName() << endl;

		p0->SetMarkerSize( mkSize );
		p1->SetMarkerSize( mkSize );
		p0->SetMarkerStyle( marker.at(0 + (temp-temp0)/3) );
		p1->SetMarkerStyle( marker.at(0 + (temp-temp0)/3) );
		h0->SetMarkerSize( mkSize );
		h1->SetMarkerSize( mkSize );
		h0->SetMarkerStyle( marker.at(0 + (temp-temp0)/3) );
		h1->SetMarkerStyle( marker.at(0 + (temp-temp0)/3) );

		theAttrib->attribProf( p0, cols.At( col*15 ) );
		theAttrib->attribProf( p1, cols.At( col*15 ) );

		auto fit0 = p0->GetFunction(fitFunc);
		auto fit1 = p1->GetFunction(fitFunc);

		if( fit0 ) {
		  fit0->SetLineColor( cols.At(col*15) );
		  fit0->SetLineStyle( 1 );
		}
		if( fit1 ) {
		  fit1->SetLineColor( cols.At(col*15) );
		  fit1->SetLineStyle( 2 );
		}
		//(p0->GetFunction(fitFunc))->SetLineColor( cols.At(col*15) );
		//(p1->GetFunction(fitFunc))->SetLineColor( cols.At(col*15) );
		//(p0->GetFunction(fitFunc))->SetLineStyle( 1 );
		//(p1->GetFunction(fitFunc))->SetLineStyle( 2 );
		
		theAttrib->attribHist( h0, cols.At( col*15 ), 0.3, 0 );
		theAttrib->attribHist( h1, cols.At( col*15 ), 0.3, 0 );
		(h0->GetFunction("gaus"))->SetLineColor( cols.At(col*15) );
		(h1->GetFunction("gaus"))->SetLineColor( cols.At(col*15) );
		(h0->GetFunction("gaus"))->SetLineStyle( 1 );
		(h1->GetFunction("gaus"))->SetLineStyle( 2 );

		vProf.push_back( p0 );
		vProf.push_back( p1 );
		vHist0.push_back( h0 );
		vHist1.push_back( h1 );

		leg->AddEntry( p0, Form("%02d_%02dC", idx, temp), "lp");

		col++;
		mk++;

		//if(mk>0 ) break;
	}

	TCanvas* c1 = new TCanvas("c1", "c1");
	int idx=0;
	TString opt="";
	for( auto h: vProf ) 
	{
	  if(isTDC) h->SetTitle( Form("[%s] chip%02d&%02d_chan%02d_tac%1d_%c;phase;fine", psuffix, rnameID0.chipID, rnameID1.chipID, hnameID.chanID, hnameID.tacID, hnameID.br) );
		else      h->SetTitle( Form("[%s] chip%02d&%02d_chan%02d_tac%1d;tEnd-ti;fine", psuffix, rnameID0.chipID, rnameID1.chipID, hnameID.chanID, hnameID.tacID) );
		
		c1->cd();
		h->Draw( Form("%s", opt.Data()) );

		if( idx==0 ) opt="same";
		idx++;
		//if( idx>0 ) break;
	}

	leg->Draw();

	c1->Print(Form("output/tempDep_%s%s_%s.pdf(", isTDC? Form("br%c_",hnameID.br):"", psuffix, isTDC? "TDC":"QDC") );

	double prof_x0 = isTDC? 2.0 : 50 ;
	double prof_x1 = isTDC? 2.2 : 120;
	vProf.at(0)->GetXaxis()->SetRangeUser( prof_x0, prof_x1);
	c1->Modified();
	c1->Update();

	if( !isTDC ) 
	{
	  c1->Print(Form("output/tempDep_%s%s_%s.pdf", isTDC? Form("br%c_",hnameID.br):"", psuffix, isTDC? "TDC":"QDC") );
	  vProf.at(0)->GetXaxis()->SetRangeUser( 100, prof_x1);
		vProf.at(0)->SetMaximum( 400 );
		vProf.at(0)->SetMinimum( 300 );
	  c1->Modified();
	  c1->Update();
	}

	c1->Print(Form("output/tempDep_%s%s_%s.pdf)", isTDC? Form("br%c_",hnameID.br):"", psuffix, isTDC? "TDC":"QDC") );
	
	TCanvas* c2 = new TCanvas("c2", "c2", 1200, 500);
	c2->Divide(2,1);
	THStack* st0 = new THStack();
	THStack* st1 = new THStack();
	if( isTDC ) {
		st0->SetTitle( Form("[%s] chip%02d_chan%02d_tac%1d_%c;tError;", hsuffix, rnameID0.chipID, hnameID.chanID, hnameID.tacID, hnameID.br) );
	  st1->SetTitle( Form("[%s] chip%02d_chan%02d_tac%1d_%c;tError;", hsuffix, rnameID1.chipID, hnameID.chanID, hnameID.tacID, hnameID.br) );
	} else {
		st0->SetTitle( Form("[%s] chip%02d_chan%02d_tac%1d;qError;", hsuffix, rnameID0.chipID, hnameID.chanID, hnameID.tacID) );
	  st1->SetTitle( Form("[%s] chip%02d_chan%02d_tac%1d;qError;", hsuffix, rnameID1.chipID, hnameID.chanID, hnameID.tacID) );
	}

	for( auto h: vHist0 ) st0->Add( h );
	for( auto h: vHist1 ) st1->Add( h );

	c2->cd(1);
	st0->Draw("nostack");
	st0->Draw("nostack p same");
	if( isTDC ) st0->GetXaxis()->SetRangeUser( -0.1, 0.1 );
	leg->Draw();
	gPad->Modified();

	c2->cd(2);
	st1->Draw("nostack");
	st1->Draw("nostack p same");
	if( isTDC ) st1->GetXaxis()->SetRangeUser( -0.1, 0.1 );
	leg->Draw();
	gPad->Modified();

	c2->Print(Form("output/tempDep_%s%s_%s.pdf)", isTDC? Form("br%c_",hnameID.br):"", hsuffix, isTDC? "TDC":"QDC") );
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
void initializeHistNameStr( kHistNameID* hnameID )
{
	hnameID->portID  = -1;
	hnameID->slaveID = -1;
	hnameID->chipID  = -1;
	hnameID->chanID  = -1;
	hnameID->tacID   = -1;
	hnameID->br      = 'N';
	hnameID->suffix  = "";

	return;
}



//std::tuple<int, int, int, int, int, char> breakHistName( TString kHistName )
void breakHistName( TString kHistName, kHistNameID* hnameID, bool isTDC=kTRUE )
{
	bool kVerbose = 0;

	/// extract chipID, tacID, channelID from the file name and hname
	auto portIdx = kHistName.Index('_') + 1;
	int  portLen = 2;
	auto portIDstr = (TString) kHistName( portIdx, portLen );
	int  portID  = portIDstr.Atoi();
	if( kVerbose ) cout << "port ID: " << portIDstr << " => (int) " << portID << endl;
	
	auto slaveIdx = portIdx + portLen + 1;
	int  slaveLen = 2;
	auto slaveIDstr = (TString) kHistName( slaveIdx, slaveLen );
	int  slaveID  = slaveIDstr.Atoi();
	if( kVerbose ) cout << "slave ID: " << slaveIDstr << " => (int) " << slaveID << endl;

	auto chipIdx = slaveIdx + slaveLen + 1;
	int  chipLen = 2;
	auto chipIDstr = (TString) kHistName( chipIdx, chipLen );
	int  chipID  = chipIDstr.Atoi();
	if( kVerbose ) cout << "chip ID: " << chipIDstr << " => (int) " << chipID << endl;

	auto chanIdx = chipIdx + chipLen + 1;
	int  chanLen = 2;
	auto chanIDstr = (TString) kHistName( chanIdx, chanLen );
	int  chanID  = chanIDstr.Atoi();
	if( kVerbose ) cout << "chan ID: " << chanIDstr << " => (int) " << chanID << endl;

	auto tacIdx = chanIdx + chanLen + 1;
	int  tacLen = 1;
	auto tacIDstr = (TString) kHistName( tacIdx, tacLen );
	int  tacID  = tacIDstr.Atoi();
	if( kVerbose ) cout << "tac ID: " << tacIDstr << " => (int) " << tacID << endl;

	auto brIdx = tacIdx + tacLen + 1;
	int  brLen = isTDC? 1:0;
	auto brIDstr = (TString) kHistName( brIdx, brLen );
	char brID  = isTDC? (brIDstr=='T'? 'T':'E') : '\0'; //TOF_Branch::fBranchT : TOF_Branch::fBranchE;
	if( kVerbose ) cout << "br ID: " << brIDstr  << endl;

	auto nameIdx = isTDC? brIdx + brLen + 1 : brIdx+brLen;
	int  nameLen = 30; // dummy. large enough number
	auto nameIDstr = (TString) kHistName( nameIdx, nameLen );
	if( kVerbose ) cout << "suffix ID: " << nameIDstr << endl;

	hnameID->portID  = portID ;
	hnameID->slaveID = slaveID;
	hnameID->chipID  = chipID ;
	hnameID->chanID  = chanID ;
	hnameID->tacID   = tacID  ;
	hnameID->br      = brID   ;
	hnameID->suffix  = nameIDstr;

	return;
}


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



///****************************************************************************************///
///****************************************************************************************///
///****************************************************************************************///



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
