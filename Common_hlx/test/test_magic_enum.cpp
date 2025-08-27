#include <iostream>
#include <functional>

// enum introspection
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 256
#include "magic_enum.hpp"
using namespace magic_enum;

#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/printf.h"
#include "fmt/color.h"

using namespace fmt;
using namespace std;

int main() {

  print("Hi!\n");

  // unscoped
  enum Color {eRED,eBLUE,eGREEN};
  auto color_name = enum_name(Color::eRED);
  cout << color_name << endl;

  //scoped
  enum class HSK_COMMAND_ID {
      eMainFlows = 0x0,
      eMainPressure,
      eHeliumLevels,
      eRTDall,
      eMagnetFlows,
      eMagnetPressure,
      eThermistors,
      eHVmon,
      eDCTPressure,
      eInputs,
      eOutputs,
      eSolarTemperatures,
      eAutoPriorityPeriod,
      ePacketCount,
      eMapDevices = 0xFA
  };

  // lambda
  auto P=[](auto x){print("Entry {} has value 0x{:02X}\n", x.second, x.first);};

  for(auto x: enum_entries<HSK_COMMAND_ID>()) P(x);

  // try casting non existent enum val
  auto tryIt = magic_enum::enum_cast<HSK_COMMAND_ID>(0xFF);
  cout << "has value: "  << (uint8_t)tryIt.value() << endl;


  return 0;

}