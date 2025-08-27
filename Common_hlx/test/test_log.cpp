#include <iostream>
#include <cstring>

#include <chrono>
#include <thread>
#include <mutex>

#include "HLX_Logger.h"

using namespace std;

class SomeClass{
  public:
    SomeClass(){
      HLX_Logger::getInstance()->info("from constructor");
    }
    void doSomething(){
      HLX_Logger::getInstance()->info("from function");
      HLOG_INFO("Info called from a macro");
    }
    ////////////////////////////////////////////////////////////////
    std::thread fThread;
    atomic_bool fContinueThread{true};
    bool        fThreadSpawned{false};
    void killCollectorThread(){fContinueThread = false;}
    void joinCollectorThread(){if(fThread.joinable()) fThread.join();}
    int spawnCollectorThread(){
      if(fThreadSpawned) return -1;
      fThread = std::thread{&SomeClass::collectorThread, this};
      fThreadSpawned = true;
      return 0;
    }
    void collectorThread(){
      while(fContinueThread) {
        hlog::info("Hi From Thread!");
        this_thread::sleep_for(chrono::milliseconds(200));
      }
    }
};


using namespace spdlog;
int main()
{

  // can initialize like this:
  //auto log = HLX_Logger::getInstance("log/logfile.log");
  auto log = HLX_Logger::getInstance();
  log->addLogFile("log/000.log");
  /// if you don't have target_compile_definitions(fmt PUBLIC FMT_EXCEPTIONS=0)
  /// set in your CMakeLists.txt, then you will get an unhandled exception here:
  log->info("Unhandled Exception Info? {:d}", 2.22222); ///unhandled exception?
  log->warn("Unhandled Exception Warn? {:d}", 2.22222); ///unhandled exception?
  log->error("Unhandled Exception Error? {:d}", 2.22222); ///unhandled exception?
  log->error("Unhandled Exception Error? {:f}", -66, 555); ///unhandled exception?
  log->error("Unhandled Exception Error? {:f} {:s}", -66, 555); ///unhandled exception?
  log->error("Unhandled Exception Error? {:f} {} {}", -66, 555); ///unhandled exception?

  log->warn("I can warn this way too!");
  log->warn("I can warn this way too!");
  log->warn("I can warn this way too!");
  log->warn("I can warn this way too!");
  log->warn("I can warn this way too! 2");

  log->log(hlog::lvl::critical, "What does critical look like???");

  hlog::setConsoleLogLevel(hlog::lvl::info);
  hlog::error("Always");
  hlog::error("Always {}", 2.2222);
  hlog::info("Info");
  hlog::info("Info {}", 2.2222);
  hlog::debug("Should not see me!");
  hlog::trace("Should not see me either!");
  hlog::setConsoleLogLevel(hlog::lvl::debug);
  hlog::debug("Now I am here !");
  hlog::trace("But I am still hidden...");
  hlog::setConsoleLogLevel(hlog::lvl::trace);
  hlog::trace("Now you should see me!  Here is a number: {:2.2f}", 2.2222);

  hlog::error("-------------------------------------------------------------");
  hlog::log(hlog::lvl::error, "hi! from general logger");
  hlog::error("-------------------------------------------------------------");

  HLOG_INFO("hi from macro.  Some colors are below");
  hlog::info(hlog::kRed, "With hlog text style");
  hlog::info(hlog::kBlue, "With hlog text style");
  hlog::info(hlog::kGreen, "With hlog text style");
  hlog::info(hlog::kYellow, "With hlog text style");
  hlog::info(hlog::kCyan, "With hlog text style");
  hlog::info(hlog::kMagenta, "With hlog text style");

  SomeClass sc;
  sc.doSomething();

#if 0
  // threaded test
  hlog::info("hi from hlog!");
  hlog::info(__FUNCTION__, "hi from hlog!");
  HLOG_INFO("hi from macro");
  SomeClass *sc[20];
  for(auto x:sc){
    x = new SomeClass;
    x->spawnCollectorThread();
  }

  while(true) {
    hlog::info("Hi From Main!");
    this_thread::sleep_for(chrono::milliseconds(200));
  }
#endif


#if 0
  //Old tests
  cout << "Logging Test" << endl;

  auto ss = fmt::format(fmt::fg(fmt::color::crimson), "Red String??");
  fmt::print(ss+"\n");

  //stuff to print to screen
  auto fConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  fConsoleSink->set_level(spdlog::level::warn);
  fConsoleSink->set_pattern("[multi_sink_example] [%^%l%$] %v");

  //stuff to go to file only
  auto fFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/hlx.log", true);
  fFileSink->set_level(spdlog::level::trace);

  //join sinks into single logger.
  spdlog::logger logger("helix", {fConsoleSink, fFileSink});
  logger.set_level(spdlog::level::debug);
  logger.warn("this should appear in both console and file");
  logger.info("this message should not appear in the console, only in the file");
  logger.debug("debuggggggggy");

  //this is the default global logger (to stdout, colored and multithreaded).
  //you can swap this out with spdlog::set_default_logger(some_other_logger);
  spdlog::error("What ho, error boy??");
  spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
  spdlog::warn("Easy padding in numbers like {:08d}", 12);
  spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  spdlog::info("Support for floats {:03.2f}", 1.23456);
  spdlog::info("Positional args are {1} {0}..", "too", "supported");
  spdlog::info("{:>8} aligned, {:<8} aligned", "right", "left");

  //Change log format
  //default
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][%-8l]%$[%!] %v");
  //spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
  spdlog::info("New Pattern");
  spdlog::warn("New Pattern");
  SPDLOG_TRACE(ss);
  SPDLOG_INFO("what is this info");
  SPDLOG_ERROR("what is this error");
  logger.warn(ss);

 //what does trace do?
  logger.set_level(spdlog::level::trace);
  fConsoleSink->set_level(spdlog::level::trace);

  spdlog::trace("this is a trace");
  logger.trace("this is also a trace");
  SPDLOG_TRACE(ss);

  //can insert info using normal methods like so:L
  spdlog::warn("{}: Some log message {}", __FUNCTION__, 1234);


  //fConsoleSink->info("works???");
  //Setting log level at runtime:
#endif


  return 0;
}
