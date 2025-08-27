//#include "TOF_TreeData.h"

gSystem->Load("$GRAMS_LIB_PATH/libGramsTofAnalysisLib.dylib");

void channelID_test()
{
	auto theChanConv = TOF_ChannelConversion::getInstance();

	auto absChanID = theChanConv->getAbsoluteChannelID( 2, 65 );
	auto absChanID_bin = std::bitset<32>(absChanID);

	cout << "Absolute channel ID: 0b" << absChanID_bin ;
	cout << Form( "\t0x%X\t%d", absChanID, absChanID) << endl;

	auto portID = (absChanID>>17) & 0x1F;
	auto portID_bin = std::bitset<32>(portID);
	//cout << "----------> port ID: 0b" << portID_bin ;
	//cout << Form( "\t0x%X\t%d", portID, portID) << endl;

	auto asicID = (absChanID>>6) & 0x3F;
	auto asicID_bin = std::bitset<32>(asicID);
	cout << "----------> asic ID: 0b" << asicID_bin ;
	cout << Form( "\t0x%X\t%d", asicID, asicID) << endl;

	auto chanID = (absChanID>>0) & 0x3F;
	auto chanID_bin = std::bitset<32>(chanID);
	cout << "----------> chan ID: 0b" << chanID_bin ;
	cout << Form( "\t0x%X\t%d", chanID, chanID) << endl;

}
