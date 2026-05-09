///
/// Draw S-curve
///
#include <filesystem>
namespace fs = std::filesystem;

auto theChanConv = TOF_ChannelConversion::getInstance();
auto theAttrib   = TOF_Attributes::getInstance();
auto theCalib    = TOF_DiscriminatorCalibration::getInstance();
	
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

void getPathList( std::vector< kFilePath > *vPath );
void breakName( TString kRootName, kHistNameID *rnameID, TString format );

vector<double> asic0T =  {29.55, 32.07, 34.75, 37.27, 39.50, 41.89, 44.34, 42.89, 40.78, 38.21, 35.87, 33.47, 31.06};
vector<double> asic1T =  {31.28, 33.80, 36.43, 38.49, 41.22, 43.45, 45.95, 44.61, 42.50, 40.00, 37.66, 35.20, 32.85};

///==============================================================================///
void tempDep_disc(bool isAsicTemp=kTRUE, TString svg0 = "disc_calibration_00_00_00.svg" )
{
	gStyle->SetOptStat(0);
	gROOT->ForceStyle();
	gStyle->SetPalette(55);
	auto cols = TColor::GetPalette();
	std::vector<int> marker = {24, 25, 26, 32, 27, 28, 30, 35, 37, 40, 42, 46, 49, 20, 21, 22, 29, 33, 34};

	TString calRoot= "disc_calibration.root";
	const char* fitFunc = "fSigmoid";
	
	kHistNameID snameID;
	breakName( svg0, &snameID, "svg" );

	auto portID = snameID.portID;
	auto slaveID = snameID.slaveID;
	auto chipID0 = snameID.chipID;
	auto chipID1 = chipID0 + 1;
	int  channelID = 0; // as an example

	int chipInterest = chipID0;
	//int chipInterest = chipID1;

	TString thrNameList[3] = {"vth_t1", "vth_t2", "vth_e"};

	std::map< std::tuple<int, int, TString, int, int>, TProfile* > vProfNoise;
	std::map< std::tuple<int, int, TString, int, int>, TProfile* > vProfDark ;
	std::map< std::tuple<int, int, TString, int, int>, double > vZero ;
	std::map< std::tuple<int, int, TString, int, int>, double > vNoise;

	std::vector<kFilePath> vPath;
	getPathList( &vPath );

	int col =1;
	double mkSize = 0.7;
	auto leg = new TLegend(0.15, 0.17, 0.37, 0.87, "");
	leg->SetFillStyle( 0 );
  leg->SetBorderSize(0);

	TString tempStr = isAsicTemp? "asic temp" : "set temp";
	
	std::vector<TGraphErrors*> gZ_tsv; // filled from disc_calibration.tsv
	for( int i=0; i<3; i++ )
	{
		auto thrType = (TString) thrNameList[i](thrNameList[i].Index("_")+1,5);

		TGraphErrors* g = new TGraphErrors();
		g->SetName (Form( "gZtsv_chip%02d_chan%02d_%s" , chipInterest, channelID, thrType.Data() ));
		g->SetTitle(Form( "[Zero] chip%02d_chan%02d_%s;Temp (C);Zero", chipInterest, channelID, thrType.Data() ));
		theAttrib->attribGraph( g );
		gZ_tsv.push_back( g );
	}

	std::vector<TGraphErrors*> gBL; // filled from disc_calibration.tsv, baseline
	for( int i=0; i<2; i++ )
	{
		TString brName = (i== (int) TOF_Branch::fBranchT)? "T":"E";
		TGraphErrors* gB = new TGraphErrors();
		gB->SetName (Form( "gBL_chip%02d_chan%02d_%s" , chipInterest, channelID, brName.Data() ));
		gB->SetTitle(Form( "[Baseline] chip%02d_chan%02d_%s;Temp (C);Baseline", chipInterest, channelID, brName.Data() ));
		theAttrib->attribGraph( gB );
		gBL.push_back( gB );
	}

	int temp0;
	int addLegEntry = 0;
	for( auto path: vPath )
	{
		auto idx = path.idx;
		auto temp = path.temp;
		auto fdir = path.fpath;

		if( idx==0 ) temp0= temp;

		TString fPath = Form( "%s/%s", fdir.Data(), calRoot.Data() );
		if( gSystem->AccessPathName( fPath.Data() ) ) {
			cout << "File Not Found: " << fPath << endl;
			continue;
		}

		theCalib->readCalibFromDir( fdir.Data() );
		cout << Form("[%02d]", idx) << endl;

		for( auto chipID : {chipID0, chipID1} )
		{
			auto absChanID = theChanConv->getAbsoluteChannelID( portID, slaveID, chipID, channelID );
			theCalib->printCalibTable( absChanID );

		  for( auto thresholdName: thrNameList ) 
		  {
		  	//for( int channelID = 0; channelID<64; channelID++ )
		  	//{
		  		TString hnameN = Form("hNoise_%02d_%02d_%02d_%02d_%s" ,portID, slaveID, chipID, channelID, thresholdName.Data());
		  		TString hnameD = Form("hDark_%02d_%02d_%02d_%02d_%s"  ,portID, slaveID, chipID, channelID, thresholdName.Data());
		      auto hN = (TProfile*) (new TFile(fPath.Data()))->Get(hnameN);
		      auto hD = (TProfile*) (new TFile(fPath.Data()))->Get(hnameD);
		
					if( !hN ) {
	       		cout << "[ERR] " << fPath << ": [hNoise] " << hN->GetName() << endl;
	       		continue;
	       	}
					if( !hD ) {
	       		cout << "[ERR] " << fPath << ": [hDark]  " << hD->GetName() << endl;
	       		continue;
	       	}

					auto thrType = (TString) thresholdName(thresholdName.Index("_")+1,5);
		      hN->SetName( Form("hN_chip%02d_chan%02d_%s_%02d_%02dC", chipID, channelID, thrType.Data(), idx, temp) );
		      hD->SetName( Form("hD_chip%02d_chan%02d_%s_%02d_%02dC", chipID, channelID, thrType.Data(), idx, temp) );
		      
					hN->SetTitle( Form("[Noise] chip%02d_chan%02d_%s;%s (DAC);Count/Max Count", chipID, channelID, thrType.Data(), thresholdName.Data()) );
					hD->SetTitle( Form("[Dark] chip%02d_chan%02d_%s;%s (DAC);Event frequency (Hz)", chipID, channelID, thrType.Data(), thresholdName.Data()) );

		      //cout << fPath << " => hN->GetEntries()= " << hN->GetEntries() << "hN name: " << hN->GetName() << endl;
					//cout << fPath << " => hD->GetEntries()= " << hD->GetEntries() << "hD name: " << hD->GetName() << endl;

					hN->SetMarkerSize( mkSize );
					hD->SetMarkerSize( mkSize );
					hN->SetMarkerStyle( marker.at(0 + (temp-temp0)/3) );
					hD->SetMarkerStyle( marker.at(0 + (temp-temp0)/3) );
		      theAttrib->attribProf( hN, cols.At( col*15 ) );
		      theAttrib->attribProf( hD, cols.At( col*15 ) );

					auto fitN = hN->GetFunction(fitFunc);
					auto zero(-99.), noise(-99.);
					if( fitN ) {
						fitN->SetLineColor( cols.At( col*15 ) );
						zero  = fitN->GetParameter(2);
						noise = fitN->GetParameter(1);

						//if( chipID == chipInterest ) cout << Form("par0: %4.2f, par1: %4.2f, par2: %4.2f", fitN->GetParameter(0), noise, zero) << endl;
					}
					
					auto mapIdx = std::tuple( chipID, channelID, thrType, idx, temp );
					vProfNoise[ mapIdx ] = hN;
					vProfDark [ mapIdx ] = hD;

					if( zero != -99 ) vZero [ mapIdx ] = zero ;
					if( noise!= -99 ) vNoise[ mapIdx ] = noise;
				//}

					//////////
					/// use the discriminator calib class ///
					if( chipID != chipInterest ) continue;
		      int ThrIdx = -1;
		      if     (thrType == "t1") ThrIdx = 0;
		      else if(thrType == "t2") ThrIdx = 1;
		      else if(thrType == "e" ) ThrIdx = 2;
					auto params = theCalib->getDiscrParams( absChanID, (TOF_Discriminator) ThrIdx );
					auto parB = params.at(0); // baseline
					auto parZ = params.at(1); // zero
					auto parN = params.at(2); // noise


		      auto gTemp = chipInterest==chipID0? asic0T.at(idx) : chipInterest==chipID1? asic1T.at(idx) : temp; 
		      if( !(chipInterest==chipID0 || chipInterest==chipID1) ) cout << "Use [SetTemp] as X-values of TGraphErrors" << endl;
		      gZ_tsv[ThrIdx]->SetPoint( idx, gTemp, parZ );
		      gZ_tsv[ThrIdx]->SetPointError( idx, 0, parN );

					if( ThrIdx==1 ) continue;
					if( ThrIdx==0 ) gBL[0]->SetPoint( idx, gTemp, parB ); // branchT
					if( ThrIdx==2 ) gBL[1]->SetPoint( idx, gTemp, parB ); // branchE
			}
		}
		col++;
	}


	TCanvas* cN = new TCanvas("cN", "cNoise", 1800, 500 ); 
	TCanvas* cD = new TCanvas("cD", "cDark" , 1800, 500 ); 
	cN->Divide(3,1);
	cD->Divide(3,1);

	int count[3] = {0, 0, 0};

	for( const auto [tuple, prof] : vProfNoise )
	{
		auto chipID  = std::get<0>(tuple);
		auto chanID  = std::get<1>(tuple);
		auto thrName = std::get<2>(tuple);
		auto dirIdx  = std::get<3>(tuple);
		auto dirTemp = std::get<4>(tuple);
		if( chipID!=chipInterest ) continue;

		int idx = -1;
		if     (thrName == "t1") idx = 0;
		else if(thrName == "t2") idx = 1;
		else if(thrName == "e" ) idx = 2;
		else continue;

		//cout << Form( "[Noise] chipID: %02d, channelID: %02d, thresholdName: %5s, Index: %02d, temp: %02d, prof entries: ", chipID, chanID, thrName.Data(), dirIdx, dirTemp ) << prof->GetEntries() << endl;

		cN->cd(idx+1);
		if( count[idx]==0 ) prof->Draw();
		else                prof->Draw("same");
					
		if( chipID==chipInterest && idx==0 ) leg->AddEntry( prof, Form("%02d_%02dC", dirIdx, dirTemp), "lp");

		count[idx]++;
	}
	cN->cd(1); leg->Draw();
	cN->cd(2); leg->Draw();
	cN->cd(3); leg->Draw();

	for( int i=0; i<3; i++ ) count[i]=0;
	for( const auto [tuple, prof] : vProfDark )
	{
		auto chipID  = std::get<0>(tuple);
		auto chanID  = std::get<1>(tuple);
		auto thrName = std::get<2>(tuple);
		auto dirIdx  = std::get<3>(tuple);
		auto dirTemp = std::get<4>(tuple);
		
		if( chipID!=chipInterest ) continue;

		int idx = -1;
		if     (thrName == "t1") idx = 0;
		else if(thrName == "t2") idx = 1;
		else if(thrName == "e" ) idx = 2;
		else continue;

		//cout << Form( "[Dark] chipID: %02d, channelID: %02d, thresholdName: %5s, Index: %02d, temp: %02d, prof entries: ", chipID, chanID, thrName.Data(), dirIdx, dirTemp ) << prof->GetEntries() << endl;

		if( idx==0 ) prof->SetMaximum( 1800*1E3 );
		if( idx==1 ) prof->SetMaximum( 1800*1E3 );

		cD->cd(idx+1);
		if( count[idx]==0 ) prof->Draw();
		else                prof->Draw("same");

		count[idx]++;
	}
	cD->cd(1); leg->Draw();
	cD->cd(2); leg->Draw();
	cD->cd(3); leg->Draw();

	TString pdf = Form("output/tempDep_disc_chip%02d_chan%02d.pdf", chipInterest, channelID );
	cN->Print( Form( "%s(", pdf.Data()) );

	cN->cd();
	for( int i=0; i<3; i++ ) count[i]=0;
	for( const auto [tuple, prof] : vProfNoise )
	{
		auto chipID  = std::get<0>(tuple);
		auto chanID  = std::get<1>(tuple);
		auto thrName = std::get<2>(tuple);
		auto dirIdx  = std::get<3>(tuple);
		auto dirTemp = std::get<4>(tuple);
		if( chipID!=chipInterest ) continue;

		int idx = -1;
		if     (thrName == "t1") idx = 0;
		else if(thrName == "t2") idx = 1;
		else if(thrName == "e" ) idx = 2;
		else continue;

		if( idx==0 ) prof->GetXaxis()->SetRangeUser( 43, 53 );
		if( idx==1 ) prof->GetXaxis()->SetRangeUser( 55, 64 );
		if( idx==2 ) prof->GetXaxis()->SetRangeUser( 55, 64 );

		cN->cd(idx+1);
		if( count[idx]==0 ) prof->Draw();
		else                prof->Draw("same");
					
		count[idx]++;
	}
	cN->cd(1); leg->Draw();
	cN->cd(2); leg->Draw();
	cN->cd(3); leg->Draw();

	cN->Print( Form( "%s", pdf.Data()) );
	cD->Print( Form( "%s", pdf.Data()) );
	

	/// temp vs zero, noise ///
	std::vector<TGraphErrors*> gZ; // filled from S curve fitting params
	for( int i=0; i<3; i++ )
	{
		TGraphErrors* g = new TGraphErrors();
		auto thrType = (TString) thrNameList[i](thrNameList[i].Index("_")+1,5);
		g->SetName (Form( "gZero_chip%02d_chan%02d_%s" , chipInterest, channelID, thrType.Data() ));
		g->SetTitle(Form( "[Zero] chip%02d_chan%02d_%s", chipInterest, channelID, thrType.Data() ));
		theAttrib->attribGraph( g );

		gZ.push_back( g );
	}
	
	for( const auto [tuple, zero] : vZero )
	{
		auto chipID  = std::get<0>(tuple);
		auto chanID  = std::get<1>(tuple);
		auto thrName = std::get<2>(tuple);
		auto dirIdx  = std::get<3>(tuple);
		auto dirTemp = std::get<4>(tuple);
		if( chipID!=chipInterest ) continue;

		int idx = -1;
		if     (thrName == "t1") idx = 0;
		else if(thrName == "t2") idx = 1;
		else if(thrName == "e" ) idx = 2;
		else continue;
	
		auto gTemp = chipInterest==chipID0? asic0T.at(dirIdx) : chipInterest==chipID1? asic1T.at(dirIdx) : dirTemp; 
		if( !(chipInterest==chipID0 || chipInterest==chipID1) ) cout << "Use [SetTemp] as X-values of TGraphErrors" << endl;

		gZ[idx]->SetPoint( dirIdx, gTemp, zero );
		gZ[idx]->SetPointError( dirIdx, 0, vNoise[tuple] );
	}

	TText txt;
	txt.SetTextSize(0.05);
	TMarker mk;
	mk.SetMarkerStyle(20);
	mk.SetMarkerSize(1.5);
	//TCanvas* c3 = new TCanvas("c3", "c3" , 1800, 500 ); 
	//c3->Divide(3,1);
	//for( int i=0; i<3; i++ )
	//{
	//  c3->cd(i+1);
	//  gZ[i]->Draw("al");
	//	
	//	for( int j=0; j<gZ[i]->GetN(); j++ )
	//	{
	//		auto x = gZ[i]->GetPointX(j);
	//		auto y = gZ[i]->GetPointY(j);

	//		mk.SetMarkerColorAlpha(  cols.At(j*15), 0.7 );
	//		mk.DrawMarker(x, y);

	//		if( j< 6 ) txt.SetTextAlign(13);
	//		else txt.SetTextAlign(11);
	//		txt.SetTextColorAlpha( cols.At(j*15), 0.7 );
	//		txt.DrawText( x, y, Form("%02d", j) );
	//	}
	//}

	TCanvas* c3_tsv = new TCanvas("c3_tsv", "c3_tsv" , 1800, 500 ); 
	c3_tsv->Divide(3,1);
	for( int i=0; i<3; i++ )
	{
	  c3_tsv->cd(i+1);
	  gZ_tsv[i]->Draw("al");
		
		for( int j=0; j<gZ_tsv[i]->GetN(); j++ )
		{
			auto x = gZ_tsv[i]->GetPointX(j);
			auto y = gZ_tsv[i]->GetPointY(j);

			mk.SetMarkerColorAlpha(  cols.At(j*15), 0.7 );
			mk.DrawMarker(x, y);

			if( j< 6 ) txt.SetTextAlign(13);
			else txt.SetTextAlign(11);
			txt.SetTextColorAlpha( cols.At(j*15), 0.7 );
			txt.DrawText( x, y, Form("%02d", j) );
		}
	}
	c3_tsv->Print( Form( "%s", pdf.Data()) );

	TCanvas* c4 = new TCanvas("c4", "c4" , 1200, 500 ); 
	c4->Divide(2,1);
	for( int i=0; i<2; i++ )
	{
	  c4->cd(i+1);
	  gBL[i]->Draw("al");
		
		for( int j=0; j<gBL[i]->GetN(); j++ )
		{
			auto x = gBL[i]->GetPointX(j);
			auto y = gBL[i]->GetPointY(j);

			mk.SetMarkerColorAlpha(  cols.At(j*15), 0.7 );
			mk.DrawMarker(x, y);

			if( j< 6 ) txt.SetTextAlign(13);
			else txt.SetTextAlign(11);
			txt.SetTextColorAlpha( cols.At(j*15), 0.7 );
			txt.DrawText( x, y, Form("%02d", j) );
		}
	}
	c4->Print( Form( "%s)", pdf.Data()) );

	return;

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

void breakName( TString kRootName, kHistNameID *rnameID, TString format )
{
	if( !format.BeginsWith(".") ) format = "." + format;
	int  chipLen = 2;
	auto chipIdx = kRootName.Index( format.Data() ) - chipLen;
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

