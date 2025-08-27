#include <iostream>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "fmt/format.h"
#include "fmt/color.h"
using namespace fmt;
using namespace std;

#include "HLX_CLI.h"

int main(int argc, char *argv[]) {

  /// Examples of using the HLX_CLI class
  /// try calling as ./bin/test_cli.exe -i infile.root -o 123 -Q -VV

  /// getting a pointer to the underlying CL11 object
  auto cli = HLX_CLI::get();
  //cli->add_option("positional", "Positionals don't have a - in fron of their name");
  cli->add_option("-i, --ipf", "Input filename")->required(true);
  cli->add_option("-o, --opf", "Output filename")->required(false)->default_val("out.root");
  cli->add_option("--derp", "Some derpy thing");

  set<int> theSet;
  //cli->add_option("--set", theSet, "Some derpy thing");

  cli->add_flag("-Q", "Set Q Level");
  // added in wrapper... cli->add_flag("-V", "Set verbosity level (-V - verbose, -VV very verbose!)");
  cli->add_flag("--shell, -s", "Set shell[or get from env]")->envname("SHELL");

  ///Positional argument - add_option with just name, get the value later by name
  cli->add_option("Pos_1");

  /// get the pointer using the namespaced functions
  hcli::get()->add_flag("-Z, --zoom", "Set zoom factor");

  /// parsing can be done like so
  //hcli::parse(argc, argv);
  /// or like so: HLX_CLI::parse(argc, argv);
  /// but don't do this, though!!!  This calls the underlying parse, which will abort if exceptions aren't caught!
  // NO!!!  cli->parse(argc, argv);
  /// instead, try this:
  CLI11_PARSE(*cli, argc, argv)

  ///////////////////////////////////////////////////////////////////////////////////////
  /// Was an option specified at all?
  print("--derp was NOT on cli?: {}\n", (*cli)["--derp"]->empty());

  //print("--set was: {}\n", (*cli)["--set"]);

  ///////////////////////////////////////////////////////////////////////////////////////
  /// Accessing options can be done a few ways - via CLI11 object:
  /// using results call (returns vector of strings)
  print("-Q is         {}\n", (*cli)["-Q"]->results());
  print("-Q is         {}\n", cli->operator[]("-Q")->results()); //another way to do it

  /// using <T>as():
  print("-Q is         {}\n", (*cli)["-Q"]->as<bool>());
  print("in file is:   {}\n", (*cli)["--ipf"]->as<std::string>());
  print("out file is:  {}\n", (*cli)["--opf"]->as<std::string>());
  print("shell is:     {}\n", (*cli)["--shell"]->as<string>());
  /// using count():
  print("-Q is         {}\n", (*cli)["-Q"]->count());
  print("Verbose:      {}\n", (*cli).count("-V"));

  /// by reference into existing variable
  bool isQ{false}, isQ2{true};
  (*cli)["-Q"]->results(isQ);
  print("-Q is         {}\n", isQ);

  /// using wrappers:
  print("-Z is         {}\n", hcli::getFlag("-Z"));
  print("--ipf is      {}\n", hcli::getOptionAs<string>("--ipf"));
  hcli::getOptionAs("-Q", isQ2);
  print("-Q is         {}\n", isQ2);
  print("Positional 1: {}\n", hcli::getOptionAs<string>("Pos_1"));

  /// bad conversions should terminate with some info
  print("out as int?   {}\n", (*cli)["--opf"]->as<int>());

  return 0;

}
