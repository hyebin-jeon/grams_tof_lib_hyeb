///////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_RunHeader.h, adapted from HLX_RunHeader.hh
/// Created  : Sun Nov 15 12:52:36 2015 by S.P. Wakely
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "TObject.h"
#include "TSystem.h"
#include "TUnixSystem.h"
#include "TROOT.h" //gROOT
#include "TTimeStamp.h"
#include "HLX_ObjectFile.h"

#include <cstdio>
#include <iostream>
using namespace std;

class HLX_RunHeader :public TNamed
{
  public:
    HLX_RunHeader(){
      //Set the ROOT name, etc
      SetName("rh");
      SetTitle("HELIX RunHeader");
    }
    ~HLX_RunHeader()=default;

  public:
    ////////////////////////////////////////////////////
    /// System-level info
    string         fCommandLine;  // what were we run with?
    TTimeStamp     fCreatedTime;  // when was the header built
    string         fROOTVersion;
    string         fUserInfo;
    string         fMachineName;
    string         fMachineIP;
    SysInfo_t      fMachineInfo;
    CpuInfo_t      fCPUInfo;

    HLX_ObjectFile   fSourceTree;  //copy of the source tree
    HLX_ObjectFile   fConfigTree;  //copy of the config directory

  public:
    /// End-of-run info
    int    fRunNumber{-1};
    string fRunStopReason{"Unknown"}; //default is unknown unless overwritten

  public:
    /////////////////////////////////////////////////////////////
    /// Grab most of the info and save it to the header
    int   saveInfo(int argc=0, char **argv=0);
    void  dump();

  private:
    /////////////////////////////////////////////////////////////
    /// Private collectors
    int saveConfigTree();
    int saveSourceTree();

    void setCommandLine(int argc, char **argv) {
      for(int i=0;i<argc;i++) fCommandLine += string(" ")+string(argv[i]);
    }

    ClassDef(HLX_RunHeader, 2)

};

