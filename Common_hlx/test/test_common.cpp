#include <iostream>
#include <spdlog/sinks/basic_file_sink.h>

#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/color.h"

#include "TTimeStamp.h"
#include "HLX_TimeStamp.h"
#include "HLX_HexDumper.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace fmt;
using namespace std;

int main() {

  //vector<uint8_t> out(1, 0xAD);
  vector<uint32_t> out(2, 0x41424344);
  print("0x{:X}\n", out[0]);
  HLX_HexDumper::dump(out);

  /// Time stamp tests
  TTimeStamp now;
  hlog::info("Days since start of flight: {}", HLX_TimeStamp::dT(now, true));
  hlog::info("Days since end of flight  : {}", HLX_TimeStamp::dT(now, true, true));
  hlog::info("Length of Flight          : {}", HLX_TimeStamp::dT(now, true)-HLX_TimeStamp::dT(now, true, true));

  return 0;

  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  //console_sink->set_level(spdlog::level::warn);
  //console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

  auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
  //file_sink->set_level(spdlog::level::trace);

  auto log = new spdlog::logger("multi_sink", {console_sink, file_sink});
  //logger.set_level(spdlog::level::debug);
  //logger.warn("this should appear in both console and file");
  //logger.info("this message should not appear in the console, only in the file");

  //auto dw = new HLX_DiskWriterOld(HLX_DiskWriterOld::eFILE_TYPE::DATA);

  print("======================\n" );
  print("Common test...\n", 9801290);
  TTimeStamp ts;
  ts.Print();

  log->info("Hello! {} with arguments", "World"); // Hello! World with artuments
  log->info("format {:x}", 16); // format 10

  print("======================\n" );


  return 0;

}
