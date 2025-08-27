////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_CLI.h
/// Created  : 6/9/2020 by wakely
///
/// A wrapper and assistant class for handling command-line arguments.  The underlying argument parsing engine is CLI11
/// https://github.com/CLIUtils/CLI11
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "CLI11.hpp"
#include <iostream>

class HLX_CLI {
  private:
    HLX_CLI(){
      fApp = new CLI::App{""};
      // Maybe some flags are added for everyone?
      fApp->add_flag("-V", R"(Set verbosity level (-V - verbose, -VV very verbose, -VVV really???))");
    };
    inline static HLX_CLI *fInstance{nullptr}; //the instance of the wrapper
    inline static CLI::App *fApp{nullptr};     //the instance of the wrappee

    //private copy/assign
    HLX_CLI(const HLX_CLI &)=default;
    HLX_CLI &operator=(const HLX_CLI &)=default;

  public:
    ~HLX_CLI(){
      delete fApp;
    };
    static CLI::App *get(){
      if(fInstance == nullptr){
        fInstance = new HLX_CLI(); //will make a new fApp
      }
      return fApp;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    /// This is the only function we wrap in the class, to avoid the hassle of catch/try
    /// loops in the main apps (without resorting to the supplied macro CLI11_PARSE(*fApp, argc, argv))
    static void parse(int argc, char *argv[]) {
      try {
        if(fApp == nullptr){
          std::cerr << "this shouldn't be possible!" << std::endl;
          exit(-1);
        }
        fApp->parse(argc, argv);
      } catch (CLI::ParseError &e) {                                                                                \
        auto rval = fApp->exit(e);
        exit(rval);
      }
    }

    template <class T>
    static T value_or(std::string inKey, T inDefault){
      if(fApp->count(inKey)>0){
        return (*fApp)[inKey]->as<T>();
      } else {
        return inDefault;
      }
    }

};

/// Namespace some shortcut helpers.
namespace hcli
{
    inline CLI::App *get(){
      return HLX_CLI::get();
    }

    // NB!!! This is a different signature!  This one call the HLX_CLI parse function, which wraps
    // a parse call in a try/catch loop.  If you don't do that, you will get core dumps.  The prefered
    // alternative to this is using the macro CLI11_PARSE(*cli, argc, argv)
    inline void parse(int argc, char *argv[]){
      return HLX_CLI::parse(argc, argv);
    }

    template <typename T> inline T getOptionAs(const std::string &inOptionName) {
      return (*HLX_CLI::get())[inOptionName.c_str()]->as<T>();
    }

    template <typename T>
    inline void getOptionAs(const std::string &inOptionName, T outVal) {
      (*HLX_CLI::get())[inOptionName.c_str()]->results(outVal);
    }

    inline int getFlag(const std::string &inFlagName) {
      return (*HLX_CLI::get())[inFlagName.c_str()]->count();
    }
}