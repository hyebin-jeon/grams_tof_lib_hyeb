////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_ObjectFile.cpp, adapted from HTCom_TObjectFile.cpp
/// Created  : Fri Nov 13 13:10:06 2015 by S.P. Wakely
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "HLX_ObjectFile.h"

#include <fstream>
#include <climits>
#include <cassert>

using namespace std;

ClassImp(HLX_ObjectFile)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Read a file in binary and store internally
/// \param ipfName path to file
/// \return size of object, or -1 on error
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long HLX_ObjectFile::readFile(string ipfName)
{
  fFileName = ipfName;
  ifstream ipf(fFileName.c_str(), ios_base::in | ios_base::binary);

  if(!ipf.is_open()) {
    hlog::error("HLX_ObjectFile::readFile => Couldn't open file {}!", ipfName);
    return -1;
  }

  /// slick modern reading
  istreambuf_iterator<char> start{ipf}, end;
  fData = vector<uint8_t>{start, end}; //copy over existing object
  ipf.close();

  /// filesystem's guess, for sanity
  //size_t size = filesystem::file_size(ipfName);
  size_t size = getFileSystemSize(ipfName);

  hlog::info("HLX_ObjectFile::readFile => File {} read with size {}", ipfName, fData.size());
  if(size != fData.size()) hlog::warn("HLX_ObjectFile::readFile => Filesystem/Read mismatch!! {} vs {}", size, fData.size());


  return fData.size();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Write a file in binary from internal data
/// \param opfName path to file
/// \return size of object, or -1 on error
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long HLX_ObjectFile::writeFile(string opfName)
{
  if(fData.size() == 0){
    hlog::error("HLX_ObjectFile::writeFile => internal data is empty, bailing out!");
    return -1;
  }

  hlog::info("HLX_ObjectFile::writeFile => Writing output file to {}", opfName);
  ofstream opf(opfName, ios_base::out | ios_base::binary );

  if(!opf.is_open()) {
    hlog::error("HLX_ObjectFile::writeFile => Couldn't open file {} for writing!", opfName);
    return -1;
  }

  /// copy to the file
  copy(fData.begin(), fData.end(), ostreambuf_iterator<char>(opf));
  opf.close();

  //size_t size = filesystem::file_size(opfName);
  size_t size = getFileSystemSize(opfName);

  hlog::info("HLX_ObjectFile::writeFile => File {} (original name: {}) written with size {}", opfName, fFileName, fData.size());
  if(size != fData.size()) hlog::warn("HLX_ObjectFile::writeFile => Filesystem/Read mismatch!! {} vs {}", size, fData.size());

  return fData.size();

}
