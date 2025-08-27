////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_ObjectFile.h, adapted from HTCom_TObjectFile.hh
/// Created  : Fri Nov 13 13:10:06 2015 by S.P. Wakely
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <fstream>
#include <cstdint>
#include <iterator>
#include <vector>
//#include <filesystem> //filesystem doesn't work with ROOT!
#include "TObject.h"
#include "HLX_Logger.h"

using namespace std;

class HLX_ObjectFile :public TObject
{
  public:
    HLX_ObjectFile()=default;
    ~HLX_ObjectFile()=default;

  public:
    string          fFileName; //original filename
    vector<uint8_t> fData;

  public:
    long readFile(string ipfName);
    long writeFile(string opfName);

  public:
    inline string getFileName(){return fFileName;}
    inline size_t getSize(){return fData.size();}

    size_t getFileSystemSize(string fName){
      ifstream ipf(fName, ios_base::in | ios_base::binary);
      ipf.seekg(0, std::ios::end);
      std::streamoff size = ipf.tellg();
      ipf.seekg(0);
      ipf.close();
      return size;
    }

  ClassDef(HLX_ObjectFile,1)

};
