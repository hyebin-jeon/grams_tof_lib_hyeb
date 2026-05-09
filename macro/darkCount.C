#include <fstream>

struct sChannelDisc{
	uint32_t channelID;
	int baseT;
	int baseE;
	double zeroT1;
	double zeroT2;
	double zeroE;
	double noiseT1;
	double noiseT2;
	double noiseE;
};

	
std::map< uint32_t, sChannelDisc > mChanDisc;
void readDiscriminatorCalibration( const char* fname );

void darkCount()
{
	bool canvOn = true; // whether to save profiles with zero+/- noise red lines


	/// pulse in connector ID on FebS
	uint8_t febD_connID = 4;

	//std::vector<int> vPulseChannelFebS = {77};
	//const char* finDark    = "/Users/hjeon7/Library/CloudStorage/Box-Box/GRAMS/TOF_MPD/0_TestOutputs/20260128_pulse/tofdata/calibration/disc_calibration_2026-01-28_22-56-58.17Z_dark.tsv";
	//const char* finDiscCal = "/Users/hjeon7/Library/CloudStorage/Box-Box/GRAMS/TOF_MPD/0_TestOutputs/20260128_pulse/tofdata/disc/disc_calibration_2026-01-28_22-55-52.986Z.tsv";
	
	std::vector<int> vPulseChannelFebS = {20};
	const char* finDiscCal = "/Users/hjeon7/Library/CloudStorage/Box-Box/GRAMS/TOF_MPD/0_TestOutputs/20260122_PulseTests/tofdata_20260122/disc/disc_calibration_2026-01-22_20-55-11.590Z.tsv";
	//const char* finDark    = "/Users/hjeon7/Library/CloudStorage/Box-Box/GRAMS/TOF_MPD/0_TestOutputs/20260122_PulseTests/tofdata_20260122/calibration/disc_calibration_2026-01-22_20-57-38.114Z_dark.tsv";
	//const char* finDark    = "/Users/hjeon7/Library/CloudStorage/Box-Box/GRAMS/TOF_MPD/0_TestOutputs/20260122_PulseTests/tofdata_20260122/calibration/disc_calibration_2026-01-22_21-00-16.148Z_dark.tsv";
	const char* finDark    = "/Users/hjeon7/Library/CloudStorage/Box-Box/GRAMS/TOF_MPD/0_TestOutputs/20260130_pulse_tofdata/calibration/disc_calibration_2026-01-30_22-52-24.679Z_dark.tsv";

	if( !((TString) finDark).EndsWith("dark.tsv") ) {
		std::cout << "[WARN] Darkcount Input File does NOT end with dark.tsv" << std::endl;
	}

	TString fnameDark= std::filesystem::path(finDark)   .filename().string();
	TString fnameDCal= std::filesystem::path(finDiscCal).filename().string();
	TString dtimeDarkStr = (TString) fnameDark( 17, 23 );
	TString dtimeDCalStr = (TString) fnameDCal( 17, 23 );

	cout << "dtimeDark: " << dtimeDarkStr << endl;
	cout << "dtimeDCal: " << dtimeDCalStr << endl;

  std::ifstream fin( finDark );
	if( ! fin.is_open() ) {
		cout << "No iniput file" << endl;
		return;
	}

  auto theChanConv = TOF_ChannelConversion::getInstance();

  readDiscriminatorCalibration( finDark );

  unsigned short portID, slaveID, chipID, channelID;
	std::string thrName;
	unsigned short threshold;
	double darkcount;
	int nline=0;

	std::map< uint32_t, TProfile*> vProfile_t1;
	std::map< uint32_t, TProfile*> vProfile_t2;
	std::map< uint32_t, TProfile*> vProfile_e ;
	std::vector< uint32_t > vActiveChannel;
	vProfile_t1.clear();
	vProfile_t2.clear();
	vProfile_e.clear();
	vActiveChannel.clear();

		
	theChanConv->fillChannelMaps();

	do {
    if ( fin.eof() ) break;
    fin >> portID >> slaveID >> chipID >> channelID >> thrName >> threshold >> darkcount;
		//cout << Form("%2d, %2d, %2d, %03d, %6s, %2d, %6.4f", portID, slaveID, chipID, channelID, thrName.c_str(), threshold, darkcount) << endl;
		
		uint32_t achanID = theChanConv->getAbsoluteChannelID( portID, slaveID, chipID, channelID );
		auto febSID = theChanConv->getConnIdOnFebS(achanID);

		bool addChan =true;
		std::sort( vActiveChannel.begin(), vActiveChannel.end() );

		for( auto chan0: vActiveChannel ) {
			if( chan0 == achanID ) addChan = false;
		}
		if( addChan )	vActiveChannel.push_back( achanID );

		if( thrName == "vth_t1" ) {
			if( !vProfile_t1[ achanID ] ) {
				vProfile_t1[achanID] = new TProfile( Form("pf_ch%03d_%s", achanID, thrName.c_str()), Form("[%s] CH.%03d (J%03d)", thrName.c_str(),achanID, febSID), 64, 0, 64);
				//cout << "Set TProfile: " << vProfile_t1[achanID]->GetName() << endl;
			}
			vProfile_t1[achanID]->Fill( threshold, darkcount );
			//if( darkcount > 0 ) cout << Form( "Fill channel[%03d] ( %2d, %6.2f )", achanID, threshold, darkcount ) << endl;
		}
		else if( thrName == "vth_t2" ) {
			if( !vProfile_t2[ achanID ] ) {
				vProfile_t2[achanID] = new TProfile( Form("pf_ch%03d_%s", achanID, thrName.c_str()), Form("[%s] CH.%03d", thrName.c_str(),achanID), 64, 0, 64);
			}
			vProfile_t2[achanID]->Fill( threshold, darkcount );
		}
		else if( thrName == "vth_e" ) {
			if( !vProfile_e[ achanID ] ) {
				vProfile_e[achanID] = new TProfile( Form("pf_ch%03d_%s", achanID, thrName.c_str()), Form("[%s] CH.%03d", thrName.c_str(),achanID), 64, 0, 64);
			}
			vProfile_e[achanID]->Fill( threshold, darkcount );
		}
		else {
			cout << "[WARNING] Wrong threshold name" << endl;
		}

    nline++;
  } while( 1 );

	/// info 
  cout << "Channel List: " << endl;
	int i=0;
	for( auto chan: vActiveChannel ) {
		cout << chan << ", ";
		i++;
		if( i%20==0 ) cout << endl;
		if( i == vActiveChannel.size() ) cout << endl;
	}

	int Ntest=0;
	TFile* fout = new TFile( Form("output/darkCount_%s.root", dtimeDarkStr.Data() ), "recreate" );
	auto dir_t1 = fout->mkdir( "vth_t1" );
	auto dir_t2 = fout->mkdir( "vth_t2" );
	auto dir_e  = fout->mkdir( "vth_e"  );
	auto dir_t1_canv  = fout->mkdir( "vth_t1_canv"  );
	cout << Form("[INFO] Output File Generated: %s", fout->GetName()) << endl;

	TLine l; // center
	l.SetLineColor(kRed);
	l.SetLineStyle(2);

	const char* ftsvName = Form( "output/disc_setting_%s.tsv", dtimeDCalStr.Data() );
  std::ofstream ftsv( ftsvName );
	if (!ftsv.is_open()) {
		std::cerr<< "[ERR] Cannot generate an output file: " << ftsvName << std::endl;
		return;
	}
	else cout << Form("[INFO] Output File Generated: %s", ftsvName) << endl;


	ftsv << "#portID\tslaveID\tchipID\tchannelID\tvth_t1\tvth_t2\tvth_e\n";

	TCanvas* c1 = new TCanvas("c1", "c1");
	double vth_t1, vth_t2, vth_e;
	for( uint32_t chanID : vActiveChannel )
	{
		int portID  = theChanConv->getPortID ( chanID );
		int slaveID = theChanConv->getSlaveID( chanID );
		int asicID  = theChanConv->getAsicID ( chanID );
		int chan64  = chanID%64;

		//cout << "portID: " << portID << ", slaveID: " << slaveID << ", asicID: " << asicID << ", chan64: " << chan64  << endl;

		if( vProfile_t1[chanID]->GetEntries() ==0  ) vth_t1 = 0;
		else {
			dir_t1->cd(); 
			vProfile_t1[chanID]->Write(); 

			auto zero  = mChanDisc[chanID].zeroT1 ;
		  auto noise = mChanDisc[chanID].noiseT1;
			vth_t1 = zero - noise; // smaller DAC = higher threshold voltage

			if( canvOn ) {
			  dir_t1_canv->cd();
			  c1->SetName( vProfile_t1[chanID]->GetName() );
			  c1->cd();
			  vProfile_t1[chanID]->Draw();
			  c1->Modified();
			  c1->Update();
			  auto max = gPad->GetUymax();
			  auto min = 0; 
			  //cout << Form("[Ch.%03d] zero, noise = %5.5f, %5.5f", chanID, zero, noise ) << endl;
			  l.DrawLine( zero, 0, zero, max );
			  l.DrawLine( zero+noise, 0, zero+noise, max );
			  l.DrawLine( zero-noise, 0, zero-noise, max );
			  c1->Modified();
			  c1->Update();
			  c1->Write();
			  c1->Clear();
			}
		}

		if( vProfile_t2[chanID]->GetEntries() == 0 ) vth_t2 = 0;
		else {
			dir_t2->cd(); 
			vProfile_t2[chanID]->Write(); 
			auto zero  = mChanDisc[chanID].zeroT2 ;
		  auto noise = mChanDisc[chanID].noiseT2;
			vth_t2 = zero - noise; // smaller DAC = higher threshold voltage
		}
		
		if( vProfile_e[chanID]->GetEntries() == 0 ) vth_e = 0; 
		else {
			dir_e->cd(); 
			vProfile_e[chanID]->Write(); 
			auto zero  = mChanDisc[chanID].zeroE ;
		  auto noise = mChanDisc[chanID].noiseE;
			vth_e = zero - noise; // smaller DAC = higher threshold voltage
			//cout<< Form("[vth_e ] TProfile saved of ch%03d", chanID) << endl;
		}

		ftsv << portID << "\t" << slaveID  << "\t" << asicID  << "\t" << chan64  << "\t" << vth_t1 << "\t" << vth_t2 << "\t" << vth_e << std::endl;


		Ntest++;
		//if( Ntest>10 ) break;
	}


	TCanvas* c2 = new TCanvas("c2","c2");
	c2->cd();
	std::vector< uint32_t > vPulseChannel;
	for ( auto ch: vPulseChannelFebS )
	{
		uint32_t achanID = theChanConv->getAbsoluteChannelID( febD_connID, ch );
		vPulseChannel.push_back( achanID );
	}
  std::sort( vPulseChannel.begin(), vPulseChannel.end() );

	int drawN = 0;
	for( uint32_t chanID : vActiveChannel )
	{
		vProfile_t1[chanID] -> Draw( drawN==0? "":"same" );

		auto firstBin = vProfile_t1[chanID]->FindFirstBinAbove(0);
		if( vProfile_t1[chanID]->GetBinCenter( firstBin ) < 30 )
		{ 
			vProfile_t1[chanID] ->SetLineColor  ( kViolet );
			vProfile_t1[chanID] ->SetMarkerColor( kViolet );
			cout << Form("Channel with contents at thresold < 30 --> %03d (J%03d)", chanID, theChanConv->getConnIdOnFebS(chanID)) << endl;
		}
		drawN++;
	}
	for( uint32_t chanID : vPulseChannel )
	{
		vProfile_t1[chanID] -> SetLineColor(kRed);
		vProfile_t1[chanID] -> SetMarkerColor(kRed);
		vProfile_t1[chanID] -> Draw( "same" );
	}
	fout->cd();
	c2->Write();

	ftsv.close();
	c1->Close();
	c2->Close();
	fout->Close();
}



void readDiscriminatorCalibration( const char* fname )
{
  std::ifstream fin( fname );

	if( ! fin.is_open() ) {
		cout << "No iniput file" << endl;
		return;
	}
	else cout << fname << endl;

  auto theChanConv = TOF_ChannelConversion::getInstance();

  sChannelDisc tempDisc;
	mChanDisc.clear();

  TString head;
  char buf[256];
  unsigned short portID, slaveID, chipID, channelID;
	int baseT, baseE;
	double zeroT1, zeroT2, zeroE;
	double noiseT1, noiseT2, noiseE;
	int nline=0;

  std::string sWord, sLine;
  std::stringstream ssLine;
  int lineN{0}, wordN{0};
  int line0 = 1;
  const char fSep= '\t';

  while( std::getline(fin, sLine) ) {
    if( lineN < line0 ) {
      lineN++;
      continue;
    }
    ssLine.clear();
    ssLine << sLine;

    wordN = 0;
    while( std::getline(ssLine, sWord, fSep) ) {
      if     ( wordN==0  ) portID    = std::atoi(sWord.c_str());
			else if( wordN==1  ) slaveID   = std::atoi(sWord.c_str());
			else if( wordN==2  ) chipID    = std::atoi(sWord.c_str());
			else if( wordN==3  ) channelID = std::atoi(sWord.c_str());
			else if( wordN==4  ) baseT     = std::atoi(sWord.c_str());
			else if( wordN==5  ) baseE     = std::atoi(sWord.c_str());
			else if( wordN==6  ) zeroT1    = std::atof(sWord.c_str());
			else if( wordN==7  ) zeroT2    = std::atof(sWord.c_str());
			else if( wordN==8  ) zeroE     = std::atof(sWord.c_str());
			else if( wordN==9  ) noiseT1   = std::atof(sWord.c_str());
			else if( wordN==10 ) noiseT2   = std::atof(sWord.c_str());
			else if( wordN==11 ) noiseE    = std::atof(sWord.c_str());
      else cout << "[WARNING] Too many words :(" << endl;

      wordN++;
    }
		
		auto achanID = theChanConv->getAbsoluteChannelID( portID, slaveID, chipID, channelID );
		//cout << Form("[%03d] %2u, %2u, %2u, %03hu, %2d, %2d, %3.4f, %3.4f, %3.4f, %3.4f, %3.4f, %3.4f", lineN, portID, slaveID, chipID, channelID, baseT, baseE, zeroT1, zeroT2, zeroE, noiseT1, noiseT2, noiseE ) << endl;
		tempDisc.channelID = achanID;
		tempDisc.baseT   = baseT  ;
		tempDisc.baseE   = baseE  ;
		tempDisc.zeroT1  = zeroT1 ;
		tempDisc.zeroT2  = zeroT2 ;
		tempDisc.zeroE   = zeroE  ;
		tempDisc.noiseT1 = noiseT1;
		tempDisc.noiseT2 = noiseT2;
		tempDisc.noiseE  = noiseE ;

		mChanDisc[achanID] = tempDisc;

		if(lineN>300) break;
    lineN++;
  }

	return;
}





		//TF1* fSigmoid = new TF1("fSigmoid","[0]*ROOT::Math::normal_cdf(x,[1],[2])",0,64); //hyeb: [1]=sigma, [2] = mean
  //fSigmoid->SetParName(0, "C")
  //fSigmoid->SetParName(1, "#sigma")
  //fSigmoid->SetParName(2, "x0")
  //fSigmoid->SetNpx(64)

		//auto binL = vProfile_t1[chanID]->FindFirstBinAbove( 0.1 );
		//auto binH = vProfile_t1[chanID]->FindFirstBinAbove( 0.1 );

		//auto binLW = vProfile_t1[chanID]->GetBinWidth(binL);
		//auto binHW = vProfile_t1[chanID]->GetBinWidth(binH);
		//cout << "binL width: " << binLW << ", binH width: " << binHW << endl;

		//auto low  = vProfile_t1[chanID]->GetBinCenter( binL ) - 0.5;
		//auto high = vProfile_t1[chanID]->GetBinCenter( binH ) + 0.5;

		//fSigmoid->FixParameter( 0, 1.0 );
		//fSigmoid->SetParameter( 1, high-low );
    //fSigmoid->SetParLimits( 1, 0.05, high-low)
    //fSigmoid->SetParameter( 2, (low+high)/2.0)
    //fSigmoid->SetParLimits( 2, low, high)

		//vProfile_t1[chanID]->Fit( "fSigmoid", "", "", 0, 64 );
		//auto fn = vProfile_t1[chanID]->GetFunction( "fSigmoid" );

		//if( !f ) {
		//	printf( "[WARNING] No fit for %s", vProfile_t1[chanID]->GetName() );
    //  auto zero = (low+high)/2.0;
    //  auto noise = 0.05;
		//}
		//else {
    //  auto zero = fn->GetParameter(2);
    //  auto noise = fn->GetParameter(1);
		//}

		//vProfile_t1[chanID]->Draw();


