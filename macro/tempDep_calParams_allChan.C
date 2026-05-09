///
/// Draw graphs of temperature vs. calib param
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
void breakHistName( TString kHistName, kHistNameID *hnameID);
void breakRootName( TString kRootName, kHistNameID *rnameID);

///==============================================================================///
const int NbOfTdcCalibParams = 4;
const int NbOfQdcCalibParams = 10;
vector<double> asic0T =  {29.55, 32.07, 34.75, 37.27, 39.50, 41.89, 44.34, 42.89, 40.78, 38.21, 35.87, 33.47, 31.06};
vector<double> asic1T =  {31.28, 33.80, 36.43, 38.49, 41.22, 43.45, 45.95, 44.61, 42.50, 40.00, 37.66, 35.20, 32.85};

std::string home = std::getenv("HOME");
std::string dirName = home+"/Box/GRAMS/TOF_MPD/0_TestOutputs/20251002_UChicago_AsicTempTest/20251001_Peltier";

std::vector<TGraph*> tempDep_calParams_TDC( uint8_t portID, uint8_t slaveID, uint8_t chipID, uint8_t channelID, uint8_t tacID, TString suffix );
std::vector<TGraph*> tempDep_calParams_QDC( uint8_t portID, uint8_t slaveID, uint8_t chipID, uint8_t channelID, uint8_t tacID, TString suffix );

void tempDep_calParams_allChan()
{
	uint8_t portID=0;
	uint8_t slaveID=0;
	//uint8_t chipID = 0;
	//uint8_t channelID = 0;
	uint8_t tacID = 0;
	
	//TString suffix = chipID==0? "asic0Temp":"asic1Temp";

	std::vector< std::vector<TGraph*> > vvGr_tdc;
	std::vector< std::vector<TGraph*> > vvGr_qdc;

	for( int chipID =0; chipID<2; chipID++ )
	{
		TString suffix = chipID==0? "asic0Temp":"asic1Temp";
	  for( int chanID = 0; chanID<64; chanID++ )
	  {
	    auto vGr_tdc = tempDep_calParams_TDC( portID, slaveID, chipID, chanID, tacID, suffix );
	    auto vGr_qdc = tempDep_calParams_QDC( portID, slaveID, chipID, chanID, tacID, suffix );
	  	vvGr_tdc.push_back( vGr_tdc );
	  	vvGr_qdc.push_back( vGr_qdc );
	  }
	}

	TFile* fout = new TFile("output/calib_par_temp_all.root", "recreate" );
	auto dirT = fout->mkdir( "tdc" );
	auto dirQ = fout->mkdir( "qdc" );

	dirT->cd();
	for( auto vGr: vvGr_tdc )
	{
	  for( auto gr: vGr )
	  {
	  	gr->SetMarkerStyle(20);
	  	gr->Write();

			gr->GetFunction("pol1");
	  }
	}

	dirQ->cd();
	for( auto vGr: vvGr_qdc )
	{
	  for( auto gr: vGr )
	  {
	  	gr->SetMarkerStyle(20);
	  	gr->Write();
	  }
	}
	
	fout->Close();
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
void breakHistName( TString kHistName, kHistNameID* hnameID )
{
	/// extract chipID, tacID, channelID from the file name and hname
	auto portIdx = kHistName.Index('_') + 1;
	int  portLen = 2;
	auto portIDstr = (TString) kHistName( portIdx, portLen );
	int  portID  = portIDstr.Atoi();
	//cout << "port ID: " << portIDstr << " => (int) " << portID << endl;
	
	auto slaveIdx = portIdx + portLen + 1;
	int  slaveLen = 2;
	auto slaveIDstr = (TString) kHistName( slaveIdx, slaveLen );
	int  slaveID  = slaveIDstr.Atoi();
	//cout << "slave ID: " << slaveIDstr << " => (int) " << slaveID << endl;

	auto chipIdx = slaveIdx + slaveLen + 1;
	int  chipLen = 2;
	auto chipIDstr = (TString) kHistName( chipIdx, chipLen );
	int  chipID  = chipIDstr.Atoi();
	//cout << "chip ID: " << chipIDstr << " => (int) " << chipID << endl;

	auto chanIdx = chipIdx + chipLen + 1;
	int  chanLen = 2;
	auto chanIDstr = (TString) kHistName( chanIdx, chanLen );
	int  chanID  = chanIDstr.Atoi();
	//cout << "chan ID: " << chanIDstr << " => (int) " << chanID << endl;

	auto tacIdx = chanIdx + chanLen + 1;
	int  tacLen = 1;
	auto tacIDstr = (TString) kHistName( tacIdx, tacLen );
	int  tacID  = tacIDstr.Atoi();
	//cout << "tac ID: " << tacIDstr << " => (int) " << tacID << endl;

	auto brIdx = tacIdx + tacLen + 1;
	int  brLen = 1;
	auto brIDstr = (TString) kHistName( brIdx, brLen );
	char brID  = brIDstr=='T'? 'T':'E'; //TOF_Branch::fBranchT : TOF_Branch::fBranchE;
	//cout << "br ID: " << brIDstr  << endl;

	auto nameIdx = brIdx + brLen + 1;
	int  nameLen = 30; // dummy. large enough number
	auto nameIDstr = (TString) kHistName( nameIdx, nameLen );
	//cout << "suffix ID: " << nameIDstr << endl;

	hnameID->portID  = portID ;
	hnameID->slaveID = slaveID;
	hnameID->chipID  = chipID ;
	hnameID->chanID  = chanID ;
	hnameID->tacID   = tacID  ;
	hnameID->br      = brID   ;
	hnameID->suffix  = nameIDstr;

	return;
}
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


///****************************************************************************************///
///****************************************************************************************///
///****************************************************************************************///


std::vector< TGraph* > tempDep_calParams_TDC( uint8_t portID, uint8_t slaveID, uint8_t chipID, uint8_t channelID, uint8_t tacID, TString suffix )
{
	gStyle->SetPalette(55);
	auto cols = TColor::GetPalette();
  
	/// get the file paths
	//std::string home = std::getenv("HOME");
	//std::string dirName = home+"/Box/GRAMS/TOF_MPD/0_TestOutputs/20251002_UChicago_AsicTempTest/20251001_Peltier";
	const char* tdcName = "tdc_calibration.tsv";

	std::vector<kFilePath> vPath;
	getPathList( &vPath );

	std::vector<TGraph*> vGr; 
	std::vector<TString> vParName = {"t0", "a0", "a1", "a2"};
	for( int i=0; i< NbOfTdcCalibParams; i++ )
	{
		TGraph* g = new TGraph();
		g->SetName( Form("g_chip%1d_ch%02d_%s", chipID, channelID, vParName.at(i).Data()) );
		g->SetTitle( Form("%s;temp;%s", vParName.at(i).Data(), vParName.at(i).Data()) );
		vGr.push_back( g );
	}

	bool usePelT = 0;
	std::vector<double> tempList;
	if( suffix.Contains("peltTemp") ) usePelT = 1;
	else {
		if( suffix.Contains("asic0Temp") ) tempList = asic0T;
		else if( suffix.Contains("asic1Temp") ) tempList = asic1T;
		else 
			cout << "[ERR] suffix is wrong" << endl;
	}

	int idx=0;
	int temp0;
	for( auto path: vPath )
	{
		auto idx = path.idx;
		auto temp = path.temp;
		auto fdir = path.fpath;

		if( idx==0 ) temp0= temp;

		TString fPath = Form( "%s/%s", fdir.Data(), tdcName );

		if( gSystem->AccessPathName( fPath.Data() ) ) {
			cout << "File Not Found: " << fPath << endl;
			continue;
		}

		theCalib->readTdcCalib( fPath.Data() );  

		uint32_t achanID = theChanConv->getAbsoluteChannelID( portID, slaveID, chipID, channelID );
		
		//theCalib->printTdcCalibTable( achanID );

		auto vParam = theCalib->getTdcParams_T( achanID, tacID );

		for( int i=0; i< NbOfTdcCalibParams; i++ )
		{
			auto tempX = usePelT? temp : tempList.at(idx) ;
			//vGr.at(i)->SetPoint( idx, temp, vParam.at(i) );
			//vGr.at(i)->SetPoint( idx, asic0T.at(idx), vParam.at(i) );
			vGr.at(i)->SetPoint( idx, tempX, vParam.at(i) );
			//cout << vGr.at(i)->GetName() << ": (" << i << ") " << tempX << ", " << vParam.at(i) << endl;
		}

		idx++;
		//if( idx > 0 ) break;
	}

	for( auto gr: vGr ) gr->Fit( "pol1", "q" );

	return vGr;
}	
///==============================================================================///

std::vector<TGraph*> tempDep_calParams_QDC( uint8_t portID, uint8_t slaveID, uint8_t chipID, uint8_t channelID, uint8_t tacID, TString suffix )
{
	gStyle->SetPalette(55);
	auto cols = TColor::GetPalette();
  
	/// get the file paths
	//std::string home = std::getenv("HOME");
	//std::string dirName = home+"/Box/GRAMS/TOF_MPD/0_TestOutputs/20251002_UChicago_AsicTempTest/20251001_Peltier";
	const char* qdcName = "qdc_calibration.tsv";

	std::vector<kFilePath> vPath;
	getPathList( &vPath );

	std::vector<TString> vParName;
	for( int i=0; i< NbOfQdcCalibParams; i++ ) vParName.push_back( Form("p%1d", i) );
	std::vector<TGraph*> vGr; 
	for( int i=0; i< NbOfQdcCalibParams; i++ )
	{
		TGraph* g = new TGraph();
		g->SetName( Form("g_chip%1d_ch%02d_%s", chipID, channelID, vParName.at(i).Data()) );
		g->SetTitle( Form("%s;temp;%s", vParName.at(i).Data(), vParName.at(i).Data()) );
		vGr.push_back( g );
	}

	bool usePelT = 0;
	std::vector<double> tempList;
	if( suffix.Contains("peltTemp") ) usePelT = 1;
	else {
		if( suffix.Contains("asic0Temp") ) tempList = asic0T;
		else if( suffix.Contains("asic1Temp") ) tempList = asic1T;
		else 
			cout << "[ERR] suffix is wrong" << endl;
	}

	int idx=0;
	int temp0;
	for( auto path: vPath )
	{
		auto idx = path.idx;
		auto temp = path.temp;
		auto fdir = path.fpath;

		if( idx==0 ) temp0= temp;

		TString fPath = Form( "%s/%s", fdir.Data(), qdcName );

		if( gSystem->AccessPathName( fPath.Data() ) ) {
			cout << "File Not Found: " << fPath << endl;
			continue;
		}

		theCalib->readQdcCalib( fPath.Data() );  

		uint32_t achanID = theChanConv->getAbsoluteChannelID( portID, slaveID, chipID, channelID );
		
		//theCalib->printQdcCalibTable( achanID );

		auto vParam = theCalib->getQdcParams( achanID, tacID );

		for( int i=0; i< NbOfQdcCalibParams; i++ )
		{
			auto tempX = usePelT? temp : tempList.at(idx) ;
			vGr.at(i)->SetPoint( idx, tempX, vParam.at(i) );
			//cout << vGr.at(i)->GetName() << ": (" << i << ") " << tempX << ", " << vParam.at(i) << endl;
		}

		idx++;
		//if( idx > 0 ) break;
	}

	for( auto gr: vGr ) gr->Fit( "pol1", "q" );

	return vGr;
}	
///==============================================================================///

