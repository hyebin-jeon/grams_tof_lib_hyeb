#include <iostream>
#include <TBufferJSON.h>
#include <TFile.h>
#include <TObjString.h>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "fmt/format.h"
#include "fmt/color.h"
using namespace fmt;
using namespace std;

#include "HLX_CLI.h"
#include "HLX_HSK_Store.h"

int main(int argc, char *argv[]) {

  auto cli = HLX_CLI::get();
  CLI11_PARSE(*cli, argc, argv)

  /// lovely fucker.
  HLX_HSK_Store hskStore;

  auto *opf = new TFile("/tmp/test.stg0.root", "RECREATE");
  //export that bitch
  TString json = TBufferJSON::ToJSON(&hskStore);
  TObjString out(json.Data());
  out.Write("hskState");
  opf->Write();
  opf->Close();

  auto *ipf = TFile::Open("/tmp/test.stg0.root");
  auto store = ipf->Get<TObjString>("hskState");
  if(store){
    cout << store->String() << endl;
  } else {
    cerr << "No fucking store!" << endl;
  }

  return 0;

}
