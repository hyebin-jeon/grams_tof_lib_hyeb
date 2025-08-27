////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_Logger.h
/// Created  : 6/4/2020 by wakely
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
// you shouldn't need both of these, but ROOT doesn't seem to honor the pragma
#ifndef HLX_Logger_h
#define HLX_Logger_h

#include <iostream>
#include <string>

#include <fmt/core.h>   //in header-only mode, just include fmt/format.h
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h> //print tuples
#include <fmt/chrono.h>
#include <fmt/printf.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/dup_filter_sink.h>

using namespace fmt;

namespace hlog {
    enum class lvl
    {
        trace = SPDLOG_LEVEL_TRACE,
        debug = SPDLOG_LEVEL_DEBUG,
        info = SPDLOG_LEVEL_INFO,
        warn = SPDLOG_LEVEL_WARN,
        error = SPDLOG_LEVEL_ERROR,
        critical = SPDLOG_LEVEL_CRITICAL,
        off = SPDLOG_LEVEL_OFF,
        n_levels
    };
}

// Info on logging philosophy
// http://labs.ig.com/logging-level-wrong-abstraction
// https://www.reddit.com/r/programming/comments/66ftqf/logging_levels_the_wrong_abstraction/
// Default spdlog log levels.
// 6 : off   => nothing gets logged
// 5 : critical
// 4 : error => equiv to raw cerrs
// 3 : warn
// 2 : info  => equiv to raw couts/printfs
// 1 : debug => equiv to usual verbose=1
// 0 : trace => equiv to usual verbose=2

class HLX_Logger {
  public:
    ~HLX_Logger(){
      if(fFileSink)fFileSink->flush();
      if(fConsoleSink)fConsoleSink->flush();
    }

  private:
    HLX_Logger()=default;  //private default to prevent instantiation

  private:
    inline static std::string kLogFileName {};  //NO Default filename.  If you don't provide, no logfile is made.
    inline static std::shared_ptr<spdlog::logger>                    fLogger     {nullptr};
    inline static std::shared_ptr<spdlog::sinks::sink>               fConsoleSink{nullptr};
    inline static std::shared_ptr<spdlog::sinks::sink>               fFileSink   {nullptr};
    inline static std::shared_ptr<spdlog::sinks::dup_filter_sink_mt> fDupeFilter {nullptr};

    // The singleton instance
    inline static HLX_Logger *theInstance {nullptr};

    /// configure the loggers
    static void initialize();

  public:
    /// Get the instance.
    /// Currently, passing a filename provides a kludgy way to set filename if and only if this
    /// is done in the first call of an application.  Need to do this since there is no file rename function in spdlog
    static HLX_Logger *getInstance(const std::string& logfileName=""){
      if(!logfileName.empty()) kLogFileName = logfileName;
      if(theInstance == nullptr){
        theInstance = new HLX_Logger();
        initialize();
      }
      return theInstance;
    }

  public:
    void setConsoleLogLevel(hlog::lvl newLevel){
      if(fConsoleSink) fConsoleSink->set_level((spdlog::level::level_enum)newLevel);
    }
    void setFileLogLevel(hlog::lvl newLevel){
      if(fFileSink) fFileSink->set_level((spdlog::level::level_enum)newLevel);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// turns off all the times and message classes for the console only... for cleaner debug experience
    void wartsOff(bool noColorToo=false){
      fConsoleSink->set_pattern("%v%$");
      if(noColorToo)fConsoleSink->set_pattern("%v");
    }
    void noWarts(bool noColorToo=false){
      fConsoleSink->set_pattern("%v%$");
      if(noColorToo)fConsoleSink->set_pattern("%v");
    }

    // turn on to flush all.  Doesn't seem to be a way to undo this.
    void setAutoFlush(bool on){
      if(on) {
        fLogger->flush_on(spdlog::level::trace);
      } else {
        fLogger->flush_on(spdlog::level::off);
      }
    }
    void addLogFile(std::string inFilename){
      kLogFileName = inFilename;
      fmt::print("HLX_Logger::addLogFile() => opening log file with name: {}\n", kLogFileName);
      fFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(kLogFileName, 1024*1024*250, 20); //250MB max, 20 total
      //fFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(kLogFileName, true); //basic file
      //fFileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(kLogFileName, 2, 0); //daily at 2AM
      fFileSink->set_level(spdlog::level::trace);
      fFileSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%-5l]%$ %v");  //%$ ends the color

      //Adding to sinks this way can only be done on initialization!
      //fLogger->sinks().push_back(fFileSink);
      fDupeFilter->add_sink(fFileSink); //automatically will cascade down to fLogger
    }


    // set colors with fmt strings
    static void setConsoleLogColor(hlog::lvl inLevel, fmt::color inColor){
      //auto pCon = dynamic_cast<spdlog::sinks::stdout_color_sink_mt *>(fLogger->sinks().at(0).get());  //if not using DupeFilter.  Kinda dangerous access here...

      auto theColor = fmt::string_view(fmt::detail::make_foreground_color<char>(fmt::detail::color_type(inColor)).begin());
      auto pCon = std::dynamic_pointer_cast<spdlog::sinks::stdout_color_sink_mt>(fConsoleSink);
      if(pCon) pCon->set_color((spdlog::level::level_enum)inLevel, theColor);

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// spdlog wrappers - there are 2 per level, plus 2 more if you want to support function locations
    /// If we could inject parameters into variadics, we could add call location very simply with GCC built-ins
    /// ala info(msg, args..., callSite=__builtin_FUNCTION()
    template<typename... Args>
    void critical(text_style inStyle, fmt::string_view inMessage, const Args&... args) {
      auto msg1 = fmt::format(inStyle, inMessage, args...);
      fLogger->critical(msg1);
    }
    template<typename... Args>
    void critical(fmt::string_view inMessage, const Args&... args) {
      critical(text_style{}, inMessage, args...);
    }
    template<typename... Args>
    void error(text_style inStyle, fmt::string_view inMessage, const Args&... args) {
      auto msg1 = fmt::format(inStyle, inMessage, args...);
      fLogger->error(msg1);
    }
    template<typename... Args>
    void error(fmt::string_view inMessage, const Args&... args) {
      error(text_style{}, inMessage, args...);
    }
    template<typename... Args>
    void warn(text_style inStyle, fmt::string_view inMessage, const Args&... args) {
      auto msg1 = fmt::format(inStyle, inMessage, args...);
      fLogger->warn(msg1);
    }
    template<typename... Args>
    void warn(fmt::string_view inMessage, const Args&... args) {
      warn(text_style{}, inMessage, args...);
    }
    template<typename... Args>
    void info(text_style inStyle, fmt::string_view inMessage, const Args&... args) {
      auto msg1 = fmt::format(inStyle, inMessage, args...);
      fLogger->info(msg1);
    }
    template<typename... Args>
    void info(fmt::string_view inMessage, const Args&... args) {
      info(text_style{}, inMessage, args...);
    }
    template<typename... Args>
    void trace(text_style inStyle, fmt::string_view inMessage, const Args&... args) {
      auto msg1 = fmt::format(inStyle, inMessage, args...);
      fLogger->trace(msg1);
    }
    template<typename... Args>
    void trace(fmt::string_view inMessage, const Args&... args) {
      trace(text_style{}, inMessage, args...);
    }
    template<typename... Args>
    void debug(text_style inStyle, fmt::string_view inMessage, const Args&... args) {
      auto msg1 = fmt::format(inStyle, inMessage, args...);
      fLogger->debug(msg1);
    }
    template<typename... Args>
    void debug(fmt::string_view inMessage, const Args&... args) {
      debug(text_style{}, inMessage, args...);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// General logger
    template<typename... Args>
    void log(hlog::lvl lvl, text_style inStyle, fmt::string_view inMessage, const Args&... args) {
      auto msg1 = fmt::format(inStyle, inMessage, args...);
      fLogger->log((spdlog::level::level_enum)lvl, msg1);
    }
    template<typename... Args>
    void log(hlog::lvl lvl, fmt::string_view inMessage, const Args&... args) {
      log(lvl, text_style{}, inMessage, args...);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Here be functions that also include the caller location.  These are primarily for being called from
    /// macros below, which is not a high priority now, but leaving as an example
    template<typename... Args>
    void infoLoc(fmt::string_view inCallLocation, fmt::v7::text_style inStyle, fmt::string_view inMessage, const Args&... args) {
      auto msg1 = fmt::format(inStyle, inMessage, args...);
      if(inCallLocation != "") msg1 = fmt::format("[{}] {}", inCallLocation, msg1);
      fLogger->info(msg1);
    }
    template<typename... Args>
    void infoLoc(fmt::string_view inCallLocation, fmt::string_view inMessage, const Args&... args) {
      infoLoc(inCallLocation, text_style{}, inMessage, args...);
    }

};

// helpful shortcut, or horror????
#define __HLXLOC  __FUNCTION__
// this works to overload everything
#define HLOG_INFO(format, ...) (HLX_Logger::getInstance())->infoLoc(__FUNCTION__, format, ##__VA_ARGS__)
//unfortunately, macros like this are illegal, or we could do some very nasty trickery here
//#define hlog::info(format, ...) (hlog::infoFull(__FUNCTION__, format, ##__VA_ARGS__)

// Wrap some functions in a namespace to enable things like: hlog::info()
namespace hlog {

    // some helpers for colors
    const text_style kRed       = fmt::fg(fmt::color::red);
    const text_style kBlue      = fmt::fg(fmt::color::cornflower_blue);
    const text_style kGreen     = fmt::fg(fmt::color::lime_green);
    const text_style kYellow    = fmt::fg(fmt::color::yellow);
    const text_style kCyan      = fmt::fg(fmt::color::cyan);
    const text_style kMagenta   = fmt::fg(fmt::color::magenta);
    const text_style kOrange    = fmt::fg(fmt::color::orange);
    const text_style kSienna    = fmt::fg(fmt::color::sienna);
    const text_style kBurlyWood = fmt::fg(fmt::color::burly_wood);

    const text_style kBeige   = fmt::fg(fmt::color::beige);
    const text_style kCrimson = fmt::fg(fmt::color::crimson);

    const text_style kStyleLow   = fmt::fg(fmt::color::moccasin);
    const text_style kStyleMed   = fmt::fg(fmt::color::chartreuse);
    const text_style kStyleHi    = fmt::fg(fmt::color::orange);
    const text_style kStyleCool  = fmt::fg(fmt::color::chartreuse);
    const text_style kStyleInfo  = fmt::fg(fmt::color::cornflower_blue);
    const text_style kStyleWarn  = fmt::fg(fmt::color::yellow);
    const text_style kStyleError = fmt::fg(fmt::color::crimson);

    const text_style kDodger = fmt::fg(color::dodger_blue);
    const text_style kViolet = fmt::fg(color::pale_violet_red);
    const text_style kSteel  = fmt::fg(color::steel_blue);
    const text_style kLightSteel  = fmt::fg(color::light_steel_blue);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /// very kludgy way to shortcut the initialization.
    inline void initialize(const std::string &inLogfileName=""){
      HLX_Logger::getInstance(inLogfileName);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /// turns off time and severity printout
    inline void noWarts(){
      HLX_Logger::getInstance()->noWarts();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    inline void critical(text_style inStyle, fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->critical(inStyle, inMessage, args...);
    }
    template<typename... Args>
    inline void critical(fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->critical(inMessage, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    inline void error(text_style inStyle, fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->error(inStyle, inMessage, args...);
    }
    template<typename... Args>
    inline void error(fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->error(inMessage, args...);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    inline void warn(text_style inStyle, fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->warn(inStyle, inMessage, args...);
    }
    template<typename... Args>
    inline void warn(fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->warn(inMessage, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    inline void info(text_style inStyle, fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->info(inStyle, inMessage, args...);
    }
    template<typename... Args>
    inline void info(fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->info(inMessage, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Little helpers to make tidy separators in info mode.
    inline std::string build_sep(std::string text= "", int sepLen= 100, char sepTag= '='){
      int textLen = text.empty() ? 0 : text.length() + 2;

      int markCount = sepLen-textLen;
      int halfMarkCount = int(markCount/2);
      std::string out;
      for(int n=0;n<halfMarkCount;n++) out += format("{}", sepTag);
      if(!text.empty()) out += ' ' + text + ' ';
      for(int n=0;n<sepLen-halfMarkCount-textLen;n++) out += format("{}", sepTag);
      return out;
    }

    inline void sep(text_style inStyle, std::string text="", int sepLen=100, char sepTag='=') {
      auto sep = build_sep(text, sepLen, sepTag);
      HLX_Logger::getInstance()->info(inStyle, sep);
    }

    /// Default is a green line =======
    inline void sep(std::string text="", int sepLen=100, char sepTag='=') {
      auto sep = build_sep(text, sepLen, sepTag);
      HLX_Logger::getInstance()->info(kGreen, sep);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    inline void debug(text_style inStyle, fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->debug(inStyle, inMessage, args...);
    }
    template<typename... Args>
    inline void debug(fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->debug(inMessage, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    inline void trace(text_style inStyle, fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->trace(inStyle, inMessage, args...);
    }
    template<typename... Args>
    inline void trace(fmt::string_view inMessage, const Args &... args) {
      HLX_Logger::getInstance()->trace(inMessage, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    inline void log(hlog::lvl lvl, text_style inStyle, fmt::string_view inMessage, const Args&... args) {
      HLX_Logger::getInstance()->log(lvl, inStyle, inMessage, args...);
    }
    template<typename... Args>
    inline void log(hlog::lvl lvl, fmt::string_view inMessage, const Args&... args) {
      HLX_Logger::getInstance()->log(lvl, inMessage, args...);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    inline void setConsoleLogLevel(hlog::lvl newLevel){
      HLX_Logger::getInstance()->setConsoleLogLevel(newLevel);
    }
    inline void setFileLogLevel(hlog::lvl newLevel){
      HLX_Logger::getInstance()->setFileLogLevel(newLevel);
    }
    inline void setConsoleLogColor(hlog::lvl inLevel, fmt::color inColor){
      HLX_Logger::getInstance()->setConsoleLogColor(inLevel, inColor);
    }


}

#endif
