////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_Logger.h
/// Created  : 6/4/2020 by wakely
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "HLX_Logger.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Main Setup
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HLX_Logger::initialize() {
  // Logs to screen
  //fConsoleSink = std::make_shared<spdlog::sinks::stdout_sink_mt>();          // log levels don't highlight, but text does
  //fConsoleSink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();  // log level and text highlights
  fConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();    //this is just the same as the above
  // you can set colors below

  //default level
  fConsoleSink->set_level(spdlog::level::debug);

  // Formatting reference:
  // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
  //fConsoleSink->set_pattern("[%H:%M:%S.%e][%-5l][%t]%$ %v");  //includes thread (%t)
  //fConsoleSink->set_pattern("[%H:%M:%S.%e][%-5l]%$ %v");      //sets color up to user text only
  fConsoleSink->set_pattern("[%H:%M:%S.%e][%-5l] %v%$"); //sets color of user text too (can override)

  /// Allows one to omit repeated messages, which helps with error spam
  fDupeFilter = std::make_shared<spdlog::sinks::dup_filter_sink_mt>(std::chrono::milliseconds(100));
  fDupeFilter->add_sink(fConsoleSink);

  // Syslog sink also possible
  // auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>("HLX_DAQ", "HLX_DAQ", LOG_PID);

  // Join the sinks into single logger.
  //fLogger = std::make_shared<spdlog::logger>("hlog", spdlog::sinks_init_list({fConsoleSink}));  //working prior to dupe filter
  fLogger = std::make_shared<spdlog::logger>("hlog", fDupeFilter);

  // Set custom colors - ansicolor_sink methods:
  // https://github.com/gabime/spdlog/issues/879
  // Set all normal things to light grey and leave warn/error/critical as is
  setConsoleLogColor(hlog::lvl::trace, fmt::color::light_gray);
  setConsoleLogColor(hlog::lvl::debug, fmt::color::gainsboro);
  setConsoleLogColor(hlog::lvl::info, fmt::color::light_gray);
  //setConsoleLogColor(hlog::lvl::warn, fmt::color::yellow);
  //setConsoleLogColor(hlog::lvl::error, fmt::color::crimson);
  //setConsoleLogColor(hlog::lvl::critical, fmt::color::crimson);

  // Logs to file, if requested
  if(!kLogFileName.empty()) {
    fmt::print("HLX_Logger::initialize() => opening log file with name: {}\n", kLogFileName);
    fFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(kLogFileName, 1024 * 1024 * 100, 3); //100MB max, 3 total
    //fFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(kLogFileName, true); //basic file
    //fFileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(kLogFileName, 2, 0); //daily at 2AM
    fFileSink->set_level(spdlog::level::trace);
    fFileSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%-5l]%$ %v");  //%$ ends the color

    //Adding to sinks this way can only be done on initialization!
    fLogger->sinks().push_back(fFileSink);
  }

  // If the sinks are more restrictive than this level, they will override.
  fLogger->set_level(spdlog::level::trace);

  // register for the selective service or face penalties of up to 5 yrs jail and/or a fine up to $250k!
  spdlog::register_logger(fLogger);
  spdlog::set_default_logger(fLogger);

  // you can set log levels from command line like so, if you call spdlog::cfg::load_env_levels();
  // SPDLOG_LEVEL=info ./exe.exe
  // spdlog::cfg::load_env_levels();
}

