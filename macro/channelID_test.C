
void channelConv( uint8_t febD_connID, uint8_t febS_connID ) ;

auto theChanConv = TOF_ChannelConversion::getInstance();

void channelID_test()
{
	uint8_t test = -1;
	cout << test << endl;
	channelConv( 1, 1 );
	channelConv( 1, 2 );
	channelConv( 1, 65 );
	channelConv( 1, 66 );
}



void channelConv( uint8_t febD_connID, uint8_t febS_connID ) 
{
	auto absChanID = theChanConv->getAbsoluteChannelID( febD_connID, febS_connID );
	auto absChanID_bin = std::bitset<32>(absChanID);

	cout << Form("[FEB/D %2d, FEB/S %03d] \nAbsolute channel ID: 0b", febD_connID, febS_connID) << absChanID_bin ;
	cout << Form( "\t0x%X\t%d", absChanID, absChanID) << endl;

	//auto portID = (absChanID>>17) & 0x1F;
	auto portID = theChanConv->getPortID( absChanID );
	auto portID_bin = std::bitset<32>(portID);

	auto asicID = (absChanID>>6) & 0x3F;
	//auto asicID = theChanConv->getAsicID( absChanID );
	auto asicID_bin = std::bitset<32>(asicID);
	auto febD_connID0 = theChanConv->getConnIdOnFebD( absChanID );
	cout << "----------> asic ID: 0b" << asicID_bin ;
	cout << Form( "\t0x%X\t%d,,,,,FEB/D connID = %3d", asicID, asicID, febD_connID0) << endl;

	//auto chanID = (absChanID>>0) & 0x3F;
	auto chanID = theChanConv->getChannelID( absChanID ); 
	auto chanID_bin = std::bitset<32>(chanID);
	auto febS_connID0 = theChanConv->getConnIdOnFebS( absChanID );
	cout << "------> chan ID (064) : 0b" << chanID_bin ;
	cout << Form( "\t0x%X\t%d,,,,,FEB/S connID = %3d", chanID, chanID, febS_connID0) << endl;

	chanID = theChanConv->getChannelID_128( absChanID ); 
	chanID_bin = std::bitset<32>(chanID);
	febS_connID0 = theChanConv->getConnIdOnFebS( absChanID );
	cout << "------> chan ID (128) : 0b" << chanID_bin ;
	cout << Form( "\t0x%X\t%d,,,,,FEB/S connID = %3d", chanID, chanID, febS_connID0) << endl;
	cout << "==========================================" << endl;

}
