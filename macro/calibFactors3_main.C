auto theAttrib = TOF_Attributes::getInstance();
auto theCalib  = TOF_TdcQdcCalibration::getInstance();
auto theChanConv = TOF_ChannelConversion::getInstance();
auto theFit      = TOF_Fitting::getInstance();
	
//std::vector< int > absChannelList;
//std::map< std::tuple<uint32_t, uint8_t>, TH2S* > fDataHistList;
//std::map< std::tuple<uint32_t, uint8_t>, std::tuple<double,double> > fFitParams;

#include "calibFactors3.C"
#include "plotCoincidence_as_fn.C"


void calibFactors3_main()
{
	TString dirName   = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20250903_QDC_vs_ToT";
  TString finName   = "test_qdc_raw.root";
	TString finPath   = Form("%s/%s", dirName.Data(), finName.Data() );

	TFile* fin   = new TFile( finPath.Data(), "read" );
	if( !fin->IsOpen() ) {
		printf("[ERR] FILE NOT FOUND: %s", finPath.Data());
		return;
	}
	TOF_TreeData* t = (TOF_TreeData*) fin->Get("data");
	if( !t ) return;
	t->setBranchAddress();

	//theCalib->readCalibrationFiles( "../calibration/20250903" ); /// done in each *.C code
  //theCalib->printQdcCalibTable( achanID );

	uint8_t febD_connID = 3;
	chipID.push_back( theChanConv->getAsicID(3,  1) );
	chipID.push_back( theChanConv->getAsicID(3, 65) );

	calibFactors3( dirName, t);

	auto absChanList = getAbsChannelList();
	auto fitParams = getLinFitParams();
	setFitParamsC( fitParams );

	plotCoincidence_as_fn( t, absChanList );


}

