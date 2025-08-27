#include <iostream>

#include <HLX_Logger.h>
#include <HLX_DiskWriter.h>

using namespace std;

int main() {
  HLX_Logger::getInstance()->setConsoleLogLevel(hlog::lvl::trace);


  auto diskWriter = new HLX_DiskWriter(HLX_DiskWriter::kDataS0, "/data1");
  diskWriter->useShortNameMode(true);  //turns off timestamps

  for(int n=0;n<10000;n++) {
    /// total override, if the -o option is given
    auto fname = format("/tmp/{:04d}.root", n);
    diskWriter->setFullFilepath(fname);
    hlog::info("file: {}", fname);
    diskWriter->openNewROOTFileAndSetupTrees();
    this_thread::sleep_for(50ms);
  }

  this_thread::sleep_for(1000s);

  auto rootWriter = new HLX_DiskWriter(HLX_DiskWriter::kMergedHSK, "./out");
  rootWriter->setMaxFileSizeGB(0.001*0.001);

  HLX_HSK_Heartbeat hb;
  rootWriter->writeROOTObject(&hb);
  HLX_HSK_ScienceStack ss;
  rootWriter->writeROOTObject(&ss);
  HLX_HSK_Summary sum;
  rootWriter->writeROOTObject(&sum);

  return 0;

}
