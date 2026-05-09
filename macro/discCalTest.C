#include "tempDep_calParams.C"

void discCalTest()
{
  auto theCalib    = TOF_DiscriminatorCalibration::getInstance();


	std::vector<kFilePath> vPath;
	getPathList( &vPath );

	theCalib->readCalibFromDir( vPath.at(0).fpath.Data() );  

}
