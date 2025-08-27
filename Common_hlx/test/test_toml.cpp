#include <iostream>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "fmt/format.h"
#include "fmt/color.h"
using namespace fmt;
using namespace std;

#include "HLX_CLI.h"
#include "HLX_TOML_Table.h"

int main(int argc, char *argv[]) {

  auto cli = HLX_CLI::get();
  CLI11_PARSE(*cli, argc, argv)

  HLX_TOML_Table biasTable;
  bool goodFile = biasTable.loadTableFromFile("config/rich/cathode_bias_35ADCperPE_2Vtrim_20230531.toml");
  if(!goodFile) {
    hlog::error("HLX_DAQ_RICH::readCathodeBiasFileTOMLOld => Could not open bias file!");
    return -1;
  }

  map<uint32_t, map<string, double>> FullBiasMap;
  auto ok = biasTable.getValueAtKeyPath("BiasSettings", FullBiasMap);
  if(ok){
    for(auto el: FullBiasMap){
      hlog::info("0x{:X} => {:f}", el.first, el.second.at("vhi"));
    }
  }

  return 0;

}
