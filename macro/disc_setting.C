
void disc_setting( 
		const char* finName = "/Users/hjeon7/Library/CloudStorage/Box-Box/GRAMS/TOF_MPD/0_TestOutputs/20260122_PulseTests/tofdata_20260126/disc/disc_calibration_2026-01-22_20-55-11.590Z.tsv",
		const char* foutName = "output/disc_setting.tsv" 
		)
{
	/// input file
  std::ifstream fin( finName );
	if( ! fin.is_open() ) {
		cout << "No iniput file" << endl;
		return;
	}
	else cout << "INPUT: " << finName << endl;

	/// output file
  std::ofstream fout( foutName );
	if (!fout.is_open()) {
		std::cerr<< "[ERR] Cannot generate an output file: " << foutName << std::endl;
		return;
	}
	else cout << Form("OUTPUT: %s", foutName) << endl;

	/// first line 
	fout << "#portID\tslaveID\tchipID\tchannelID\tvth_t1\tvth_t2\tvth_e\n";


	/// read input file
  TString head;
  char buf[256];
  int portID, slaveID, chipID, channelID;
	int baseT, baseE;
	double zeroT1, zeroT2, zeroE;
	double noiseT1, noiseT2, noiseE;
	int nline=0;

  std::string sWord, sLine;
  std::stringstream ssLine;
  int lineN{0}, wordN{0};
  int line0 = 1;
  const char fSep= '\t';

	double vth_t1, vth_t2, vth_e;

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
		
		//auto achanID = theChanConv->getAbsoluteChannelID( portID, slaveID, chipID, channelID );
		auto achanID = 131072*portID + 4096*slaveID + 64*chipID + channelID;
		//cout << Form("[%03d] %2u, %2u, %2u, %03hu, %2d, %2d, %3.4f, %3.4f, %3.4f, %3.4f, %3.4f, %3.4f", lineN, portID, slaveID, chipID, channelID, baseT, baseE, zeroT1, zeroT2, zeroE, noiseT1, noiseT2, noiseE ) << endl;

		vth_t1 = zeroT1 - noiseT1;
		vth_t2 = zeroT2 - noiseT2;
		vth_e  = zeroE  - noiseE;

		fout << portID << "\t" << slaveID  << "\t" << chipID  << "\t" << channelID  << "\t" << vth_t1 << "\t" << vth_t2 << "\t" << vth_e << std::endl;

    lineN++;
  }
	
	fout.close();
	fin.close();


	return;
}

