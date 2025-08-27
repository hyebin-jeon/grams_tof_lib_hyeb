#include <iostream>
#include <cstring>

#include <chrono>
#include <thread>
#include <mutex>

#include "HLX_Logger.h"
#include "HLX_ObjectFile.h"
#include "HLX_RunHeader.h"

using namespace std;

int main(int argc, char **argv)
{

  HLX_ObjectFile of, of2;
  of.readFile("tof.root");
  of.writeFile("derp");
  of2.readFile("derp");
  of2.writeFile("tof2.root");
  hlog::info("{} {} equal? {}", of.fData.size(), of2.fData.size(), of.fData==of2.fData);

  HLX_RunHeader rh;
  rh.saveInfo(argc, argv);
  rh.dump();

  rh.fSourceTree.writeFile("derp.tar");

  return 0;
}
