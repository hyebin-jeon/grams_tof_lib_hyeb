
void quickCheck()
{
	const char* fpath = "~/Box/GRAMS/TOF_MPD/0_TestOutputs/20260122_PulseTests/tofdata/stg1/ampl_1.5V/run_2026-01-22_22-22-37.717Z.stg1.root";

	//TString path = fname_;
	//int idx_fileName = path.Last('/')+1;
	//TString fname = (TString) path(idx_fileName, path.Length());

	//cout << "fname: " << fname << endl;

	std::string fname = std::filesystem::path(fpath).filename().string();
	cout << fname << endl;

	try {
        // 2. substr(시작위치, 길이)로 잘라내고 stoi로 변환
        int year  = std::stoi(fname.substr(4, 4));
        int month = std::stoi(fname.substr(9, 2));
        int day   = std::stoi(fname.substr(12, 2));
        int hour  = std::stoi(fname.substr(15, 2));
        int min   = std::stoi(fname.substr(18, 2));
        int sec   = std::stoi(fname.substr(21, 2));
				int ms = std::stoi(fname.substr(24, 3));

        // 3. TTimeStamp 객체 생성
        TTimeStamp ts(year, month, day, hour, min, sec, ms);

        std::cout << "Year: " << year << ", Month: " << month << ", Day: " << day << std::endl;
				std::cout << "data, time: " << ts.GetDate() << " " << ts.GetTime() << " " << ts.GetNanoSec() << endl;
        std::cout << "Timestamp (Seconds): " << ts.GetSec() << std::endl;
        std::cout << "Readable: " << ts.AsString("s") << " (UTC)" << std::endl;

    } catch (const std::exception& e) {
        // 파일 이름 형식이 다를 경우 에러 처리 (아까 만든 빨간색 매크로를 써보세요!)
        std::cerr << "\033[31m[ERROR] Failed to parse date from fname: " << e.what() << "\033[0m" << std::endl;
    }


  auto theChanConv = TOF_ChannelConversion::getInstance();
	uint8_t febD_connID = 4;
	std::vector<uint8_t>  smaChannels = { 1, 2, 65, 66, 20 }; // SMA connector IDs
	std::vector<uint32_t> activeChannels;
	for( auto chan: smaChannels )
	{
		auto achanID = theChanConv->getChannelID_128( chan );
		//auto achanID = theChanConv->getAbsoluteChannelID( febD_connID, chan );
		activeChannels.push_back( achanID );
		cout << (int) chan << "--> " << achanID << endl;
	}
	std::sort( activeChannels.begin(), activeChannels.end() );
	for( auto chan: activeChannels ) cout << "channel: " << chan << endl;



}
