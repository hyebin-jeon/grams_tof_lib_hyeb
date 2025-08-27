////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_RunHeader.cpp, adapted from HTSim_RunHeader.cc
/// Created  : Sun Nov 15 13:01:58 2015 by S.P. Wakely
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "HLX_RunHeader.h"
#include "HLX_ObjectFile.h"

#include "TSystem.h"
#include "TUnixSystem.h"

#include <iostream>
using namespace std;

ClassImp(HLX_RunHeader)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Collect all the required info.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int HLX_RunHeader::saveInfo(int argc, char **argv)
{
  hlog::info(hlog::kBurlyWood, "HLX_RunHeader::saveInfo => Saving System State");

  // Get the time and machine info
  fCreatedTime = TTimeStamp{};

  /// assembles the command line
  setCommandLine(argc, argv);

  //////////////////////////////////////////////////
  /// System & User Stuff
  fROOTVersion = gROOT->GetVersion();

  auto *sys = (TUnixSystem*)gSystem;
  fMachineName = sys->HostName();
  sys->GetSysInfo(&fMachineInfo);
  sys->GetCpuInfo(&fCPUInfo);
  fUserInfo = sys->GetUserInfo()->fUser;

  /// try to get IP address
  fMachineIP = "not found!";
  auto ip = sys->GetHostByName(fMachineName.c_str());
  if(ip.IsValid()) fMachineIP = ip.GetHostAddress();

  //////////////////////////////////////////////////
  /// File stuff
  int rval1 = saveConfigTree();
  int rval2 = saveSourceTree();
  if(rval1 < 0 || rval2 < 0) return -1;

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Save the configuration dicrection, which should be at ./config
/// We tar this every time because the files can be hand-edited
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int HLX_RunHeader::saveConfigTree() {
  auto rval = system("tar czf .config.tgz ./config");
  if(rval < 0){
    hlog::error("HLX_RunHeader::saveConfigTree => Error tarring config directory!");
    return rval;
  }
  hlog::info("HLX_RunHeader::saveConfigTree => Config directory tarred into ./.config.tgz");
  rval = fConfigTree.readFile(".config.tgz"); //save the current config
  if(rval < 0){
    hlog::error("HLX_RunHeader::saveConfigTree => Error reading/saving config directory!");
    return rval;
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This depends on the cmakefile producing a source tree.  We could do it here,
/// but it is harder to sort out the paths from here.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int HLX_RunHeader::saveSourceTree() {
  int rval = fSourceTree.readFile(".sourceTree.tgz");
  if(rval < 0){
    hlog::error("HLX_RunHeader::saveSourceTree => Error reading/saving source tree!");
    return rval;
  }
  return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Dump what we know...
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HLX_RunHeader::dump()
{
  hlog::info(hlog::kDodger, "====================== HELIX Data Run Header ======================");
  hlog::info("  Command Line   : {}", fCommandLine);
  hlog::info("  Run Number     : {}", fRunNumber);
  hlog::info("  Stop Reason    : {}", fRunStopReason);
  hlog::info("  Hdr Build time : {}", fCreatedTime.AsString("C"));
  hlog::info("  Run by user    : {}", fUserInfo);
  hlog::info("  Hostname       : {} ({})", fMachineName, fMachineIP);
  hlog::info("  Machine load   : {}, {}, {}", fCPUInfo.fLoad1m, fCPUInfo.fLoad5m, fCPUInfo.fLoad15m);
  hlog::info("  ROOT Version   : {}", fROOTVersion);
  hlog::info("  Source Tree    : {} ({:2.2f} kB)", fSourceTree.getFileName(), fSourceTree.getSize() / (1024.0));
  hlog::info("  Config Tree    : {} ({:2.2f} kB)", fConfigTree.getFileName(), fConfigTree.getSize() / (1024.0));
  hlog::info(hlog::kDodger, "====================== HELIX Data Run Header ======================");

}
