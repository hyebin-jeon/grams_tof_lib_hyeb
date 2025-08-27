////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_TOML_Table.h
/// Created  : 03/04/2022 by wakely
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "TString.h"
#include <filesystem>
#include <bitset>
#include <thread>
#include <limits>
#include <utility>

//#include <TSystem.h>
//#include <TPython.h>

#include "HLX_Logger.h"

/// ref: https://marzer.github.io/tomlplusplus/
#define TOML_EXCEPTIONS 0
#include "toml.hpp"

using namespace std;

/// A recursive class for accessing simple toml tables.  This replaces the HLX_TOML class
class HLX_TOML_Table {

  public:
    HLX_TOML_Table()=default;
    int fVerbose{0};

  public:
    explicit HLX_TOML_Table(const string &ipfName) {
      loadTableFromFile(ipfName);
    }

    explicit HLX_TOML_Table(toml::table *inSubPtr, HLX_TOML_Table* parentPtr) {
      fTopPtr = parentPtr;
      fTablePtr = inSubPtr;
    }

    explicit HLX_TOML_Table(toml::table *inSubPtr, string inKeyPath, int inDepth, HLX_TOML_Table* parentPtr) {
      fTopPtr = parentPtr;
      fTablePtr = inSubPtr;
      fKeyPath = std::move(inKeyPath);
      fTableDepth = inDepth + 1;
    }

    ///copy constructor
    HLX_TOML_Table(const HLX_TOML_Table &inTable) {
      fTopTableFilename = inTable.fTopTableFilename;
      fTopTable = inTable.fTopTable;
      fKeyPath = inTable.fKeyPath;
      fTableDepth = inTable.fTableDepth;
      fTablePtr = &fTopTable;
      fTopPtr = this;
    }

  public:
    string fTopTableFilename;
    toml::table fTopTable;
    toml::table *fTablePtr{nullptr};
    HLX_TOML_Table *fTopPtr{this};  //pointer to the top-most pointer in the chain of HLX_TOML_Table

  private:
    string fKeyPath;       //dotted path to this table.
    int fTableDepth{0}; //how many subs deep are we?

  public:
    bool loadTableFromFile(const string &ipfName) {
      if (fVerbose) hlog::info("HLX_TOML_Table::loadTableFromFile => Attempting to load table from file {} ", ipfName);
      if(filesystem::exists(ipfName) == false){
        hlog::error("HLX_TOML_Table::loadTableFromFile => File {} does not exist!", ipfName);
        return false;
      }
      auto parse_result = toml::parse_file(ipfName);
      if (!parse_result) {
        hlog::error("loadTableFromFile => Could not parse file {}! Check for malformed entries!", ipfName);
        return false;
      }
      fTopTableFilename = ipfName;
      fTopTable = std::move(parse_result.table());
      fTablePtr = &fTopTable;
      return true;
    }

    bool loadTableFromString(const string &inTOML) {
      if (fVerbose) hlog::info("loadTableFromString => Loading table from string");
      auto parse_result = toml::parse(inTOML);
      if (!parse_result) {
        hlog::error("loadTableFromString => Couldn't open or parse toml string!");
        return false;
      }
      fTopTableFilename = "";
      fTopTable = std::move(parse_result.table());
      fTablePtr = &fTopTable;
      return true;
    }

    bool loadTableFromTOMLTable(const toml::table &inTOML) {
      if (fVerbose) hlog::info("loadTableFromTOMLTable => Loading table from toml++ table");
      fTopTableFilename = "";
      fTopTable = inTOML;
      fTablePtr = &fTopTable;
      return true;
    }

  public:
    /// we could avoid this crap if we dump loadFrom methods in favor of constructors that throw exceptions
    bool isZombie() const {
      if (fTablePtr == nullptr) {
        hlog::error("HLX_TOML_Table::isZombie => fTablePtr is null! Did you remember to load toml from a file or string?");
        return true;
      } else {
        return false;
      }
    }

    bool isGood() {
      return !isZombie();
    }

    /// nice overloaded operator, lets you do if(fTable){do something)
    explicit operator bool() const {
      return !isZombie();
    }

    /// Print out the table info
    void dumpTable() const {
      if (isZombie()) return;
      stringstream out;
      out << *fTablePtr << endl;
      fmt::text_style style;
      if (fTableDepth == 0) style = hlog::kBurlyWood;
      if (fTableDepth == 1) style = hlog::kDodger;
      if (fTableDepth == 2) style = hlog::kMagenta;
      if (fTableDepth == 3) style = hlog::kOrange;

      hlog::info(style, "Table depth: {} | Path: '{}'", fTableDepth, fKeyPath);
      hlog::info(style, "==============================================================================");
      TString str = out.str().c_str();
      TString tok;
      Ssiz_t pos{0};
      while (str.Tokenize(tok, pos, "\n")) {
        hlog::info(style, "{}", tok.Data());
      }
      hlog::info(style, "==============================================================================");

    }

    void dumpKeyInfo(const string &inKeyPath) const {
      if (isZombie()) return;

      auto node = fTablePtr->at_path(inKeyPath);
      if (node) {
        hlog::info("getKeyInfo => Node at key {} exists! Type: '{}' | Val: {}", inKeyPath, node.type(), node);
      } else {
        hlog::info("getKeyInfo => No node found at key {}", inKeyPath);
      }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// get a subtable at a string key
    HLX_TOML_Table *getSubTableByKey(const string &inKey) {
      if (isZombie()) return nullptr;

      /// this will return a pointer to the sub-table, if found and is a table.
      auto searchTab = (*fTablePtr)[inKey].as_table();
      if (searchTab) {
        string keyPath = fKeyPath.empty() ? inKey : fKeyPath + "." + inKey;
        auto outBind = new HLX_TOML_Table(searchTab, keyPath, fTableDepth, fTopPtr);  //memory leak
        return outBind;
      } else {
        hlog::error("getSubTableByKey => Couldn't find sub-table with key: {}", inKey);
        return nullptr;
      }
    }

    /// overload to allow a integer key
    HLX_TOML_Table *getSubTableByKey(int inKey) {
      return getSubTableByKey(format("{}", inKey));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Set a key to anything you want.  If the key doesn't exist, you can make a new one.
    template<typename T>
    bool setValueAtKey(string key, T val, bool force = false) {
      if (isZombie()) return false;

      auto node = fTablePtr->get(key);
      if (!node && !force) {
        hlog::error("setValueAtKey => Error!  Couldn't find key='{}'! (To force assignment, call with force=true)",
                    key);
        return false;
      }
      auto [itr, op_type] = fTablePtr->insert_or_assign(key, val);
      if (op_type == true) {
        hlog::info("Key {} inserted with value: {}", key, val);
      } else {
        hlog::info("Key {} updated to: {}", key, val);
      }
      return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Set a keypath to a new value of the same type.
    /// You CANNOT change a node's type or make new keys with this method
    template<typename T>
    bool setValueAtKeyPath(const string &inKeyPath, T inVal) {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if (!node_view) {
        hlog::error("setValueAtKeyPath => Error!  Couldn't find key='{}'! (To force assignment, call with force=true)",
                    inKeyPath);
        return false;
      }
      auto typed_node = node_view.node()->as<T>();
      if (typed_node == nullptr) {
        hlog::error("setValueAtKeyPath => Cannot change type with this method!");
        return false;
      } else {
        *typed_node = inVal;
        hlog::info("setValueAtKeyPath => Key {} updated to: {}", inKeyPath, inVal);
      }
      return true;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assuming a set of keys of the form foo.0, foo.1, etc. return a vector of the ints at those subkeys
    ///
    vector<int> getTableSubKeys(string topKey) {
      vector<int> out;
      if (isZombie()) return out;

      auto tablePtr = fTablePtr->at_path(topKey).as_table();
      if(tablePtr){
        for(const auto &el: *tablePtr) {
          int idx;
          try {
            idx = stoi(el.first.data());
          } catch (exception &err) {
            hlog::error("getTableSubKeys => Couldn't parse table key '{}' as an int. Are you sure you have the right table?", el.first.data());
            return out;
          }
          out.push_back(idx);
        }
      } else {
        hlog::error("getTableSubKeys => Couldn't find table at key '{}'", topKey);
      }
      return out;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Parse current table for subtables and return any of them that have a subkey that matches the search term
    /// e.g., you can parse all [links.x] tables and return any that have a key named 'type' and equal to 'merger'
    /// NB: assumes that the key to the topmost table is numerical, as in [links.0], [links.1]
    ///  ==> this is a specialty helper for the main daq config.
    /// returns an stl map of <index/table>, where index is the key to the topmost table, i.e., the link number.
    map<int, HLX_TOML_Table *> getTablesBySubKey(string inKey, string inSearchTerm) {
      map<int, HLX_TOML_Table *> out;

      for(const auto &el: *fTablePtr->as_table()) {
        int idx;
        try {
          idx = stoi(el.first.data());
        } catch (exception &err) {
          hlog::error(
              "getTablesBySubKey => Couldn't parse table key '{}' as an int. Are you sure you have the right table?",
              el.first.data());
          return out;
        }

        // see if there is a table at the element
        if (el.second.is_table()) {
          auto tab = el.second.as_table();
          if (tab->get(inKey)->value_or("") == inSearchTerm) {
            auto tabW = new HLX_TOML_Table(tab, fKeyPath + '.' + el.first.data(), fTableDepth, fTopPtr);
            out[idx] = tabW;
          }
        }
      } // loop over top table

      for(auto el: out) {
        hlog::info("El: {}", el.first);
        el.second->dumpTable();
      }
      return out;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Parse the table and return a vector of table pointers at the key
    /// For example,
    ///chans = [
    ///{bias=[56.863, 56.836, 56.069, 56.032], zlo_thr=0.001, zhi_thr=0.005350, time_thr=0.026, hyst=0.004, fee_on=1}
    ///]
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    vector<HLX_TOML_Table *> getTableVectorAtKeyPath(const string &inKeyPath, bool reportMissing = true)
    {
      vector<HLX_TOML_Table *> out;
      auto node_view = fTablePtr->at_path(inKeyPath);

      if (node_view.is_array()) {
        int idx = 0;
        for(auto &node: *node_view.as_array()) {
          auto tab = node.as_table();
          string newPath = format("{}.{}[{}]", fKeyPath, inKeyPath, idx++);
          if(tab) {
            auto tabW = new HLX_TOML_Table(tab, newPath, fTableDepth, fTopPtr);
            out.push_back(tabW);
          } else {
            hlog::warn("Couldn't cast node {} as table", newPath);
          }
        }
      } else {
        hlog::error("getTableVectorAtKeyPath => No array found at key {}", inKeyPath);
      }
      return out;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Full-path getters.
    /// Can accept ['a']['path']['like']['this'] or.like.this or a[0].path[1].with.arrays.at or
    /// just single keys for top-level access
    /// bools are handled by the is_integral case
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename someIntType, std::enable_if_t<std::is_integral<someIntType>::value, bool> = true>
    bool getValueAtKeyPath(const string &inKeyPath, someIntType &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;
      auto node_view = fTablePtr->at_path(inKeyPath);
      if (node_view) {
        if (node_view.is_integer()) {
          auto tmp = node_view.as_integer()->get();
          if (tmp > numeric_limits<someIntType>::max() || tmp < numeric_limits<someIntType>::lowest()) {
            hlog::error("getValueAtKeyPath =? provided type ('{}') too small to contain toml-stored value (try a long)!", typeid(outVal).name());
            hlog::error("  parsed val: {}", tmp);
            hlog::error("  max val   : {}", numeric_limits<someIntType>::max());
            hlog::error("  min val   : {}", numeric_limits<someIntType>::min());
            return false;
          }
          outVal = tmp;
          if (fVerbose) hlog::info("getValueAtKeyPath => Getting value: {} from key '{}'", outVal, inKeyPath);
          return true;
        } else if (node_view.is_boolean()) {
          outVal = node_view.as_boolean()->get();
          if (fVerbose) hlog::info("getValueAtKeyPath => Getting value: {} from key '{}'", outVal, inKeyPath);
          return true;
        } else if (node_view.is_floating_point()) {
          outVal = round(node_view.as_floating_point()->get());
          if (fVerbose) hlog::warn("getValueAtKeyPath => Rounded value: {} from key '{}'", outVal, inKeyPath);
          return true;
        } else {
          hlog::error("getValueAtKeyPath => Value at key '{}' is not an integer! Val: {}", inKeyPath, node_view);
          return false;
        }
      } else {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing key '{}'!", inKeyPath);
        return false;
      }
    }

    /// we don't allow floats, just doubles, to avoid precision loss.  We do more tricks for integers (above),
    /// but maybe we shouldn't.
    bool getValueAtKeyPath(const string &inKeyPath, double &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if (node_view) {
        if (node_view.is_number() && node_view.value<double>().has_value()) {
          auto tmp = node_view.value<double>().value();
          outVal = tmp;

          if (fVerbose) hlog::info("getValueAtKeyPath => Getting value: {} from keypath '{}'", outVal, inKeyPath);
          return true;
        } else {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' is not a double! Val: {}", inKeyPath, node_view);
          return false;
        }
      } else {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }
    }

    /// Strings...
    bool getValueAtKeyPath(const string &inKeyPath, string &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);

      if (node_view) {
        if (node_view.is_string()) {
          outVal = node_view.as_string()->get();
          if (fVerbose) hlog::info("getValueAtKeyPath => Getting value: {} from keypath '{}'", outVal, inKeyPath);
          return true;
        } else {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' is not a string! Val: {}", inKeyPath, node_view);
          return false;
        }
      } else {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }
    }

    /// Arrays of integer things
    template<typename someIntType, typename A, std::enable_if_t<std::is_integral<someIntType>::value, bool> = true>
    bool getValueAtKeyPath(const string &inKeyPath, vector<someIntType,A> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if (node_view) {
        outVal.clear();
        auto arr = node_view.as_array();
        if(arr->is_homogeneous<int64_t>()){
          for(auto &el : *arr){
            outVal.push_back(el.as_integer()->get());
          }
        } else {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' is not a homogeneous integer array!", inKeyPath);
          return false;
        }
      } else {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }
      return true;
    }

    /// Arrays of doubles
    bool getValueAtKeyPath(const string &inKeyPath, vector<double> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if (node_view) {
        outVal.clear();
        auto arr = node_view.as_array();
        if(arr->is_homogeneous<double>()){
          for(auto &el : *arr){
            outVal.push_back(el.as_floating_point()->get());
          }
        } else {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' is not a homogeneous double array!", inKeyPath);
          return false;
        }
      } else {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }
      return true;
    }

    /// 2D arrays of floats , passed into a vector of vectors
    bool getValueAtKeyPath(const string &inKeyPath, vector<vector<double>> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);

      /// first, we verify that this is an array of arrays of integers
      if (node_view) {
        auto arr = node_view.as_array();
        if (arr->is_homogeneous<toml::array>()) {
          int cnt = 0;
          for(auto &el: *arr) {
            if (!el.as_array()->is_homogeneous<double>()) {
              hlog::error("getValueAtKeyPath => Value at keypath '{}' is contains non-integer values at row {}", inKeyPath, cnt);
              return false;
            }
            cnt++;
          }
        }
      } else {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }
      /// If we are here, then the nested array is homogeneous and all ints, so we can proceed with reading into outVal
      outVal.clear();
      auto outer_arr = node_view.as_array();
      for(auto &outer_el: *outer_arr) {
        vector<double> row;
        for(auto &inner_el: *outer_el.as_array()) {
          row.push_back(inner_el.as_floating_point()->get());
        }
        outVal.push_back(row);
      }
      return true;
    }

    /// 2D arrays of ints (usually ID tables), passed into a vector of vectors
    template<typename someIntType, typename A, typename B, std::enable_if_t<std::is_integral<someIntType>::value, bool> = true>
    bool getValueAtKeyPath(const string &inKeyPath, vector<vector<someIntType,A>,B> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);

      /// first, we verify that this is an array of arrays of integers
      if (node_view) {
        auto arr = node_view.as_array();
        if (arr->is_homogeneous<toml::array>()) {
          int cnt = 0;
          for(auto &el: *arr) {
            if (!el.as_array()->is_homogeneous<int64_t>()) {
              hlog::error("getValueAtKeyPath => Value at keypath '{}' is contains non-integer values at row {}", inKeyPath, cnt);
              return false;
            }
            cnt++;
          }
        }
      } else {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }
      /// If we are here, then the nested array is homogeneous and all ints, so we can proceed with reading into outVal
      outVal.clear();
      auto outer_arr = node_view.as_array();
      for(auto &outer_el: *outer_arr) {
        vector<int> row;
        for(auto &inner_el: *outer_el.as_array()) {
          row.push_back(inner_el.as_integer()->get());
        }
        outVal.push_back(row);
      }
      return true;
    }

    /// 2D arrays of ints (usually ID tables), passed into a fixed std::array<std::array<someIntType, N1>, N0>
    template<size_t N0, size_t N1, typename someIntType, std::enable_if_t<std::is_integral<someIntType>::value, bool> = true>
    bool getValueAtKeyPath(const string &inKeyPath, std::array<std::array<someIntType, N1>, N0> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if(!node_view) {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }

      /// first, we verify that this is an array of arrays of integers and all the right sizes
      auto arr = node_view.as_array();
      if(arr->size() != N0) {
        hlog::error("getValueAtKeyPath => Value at keypath '{}' is a 2D array with outer size {}, but you passed a 2D of outer size {}!", inKeyPath, arr->size(), N0);
        return false;
      }
      if (arr->is_homogeneous<toml::array>()) {
        int cnt = 0;
        for(auto &el: *arr) {
          if(el.as_array()->size() != N1) {
            hlog::error("getValueAtKeyPath => Value at keypath '{}' is a 2D array with inner size {}, but you passed a 2D of inner size {}!", inKeyPath, el.as_array()->size(), N1);
            return false;
          }
          if (!el.as_array()->is_homogeneous<int64_t>()) {
            hlog::error("getValueAtKeyPath => Value at keypath '{}' is contains non-integer values at row {}", inKeyPath, cnt);
            return false;
          }
          cnt++;
        }
      } else {
        hlog::error("getValueAtKeyPath => Value at keypath '{}' is not a pure array of arrays!", inKeyPath);
        return false;
      }

      /// If we are here, then the nested array is homogeneous and all ints, so we can proceed with reading into outVal
      int out_idx = 0;
      auto outer_arr = node_view.as_array();
      for(auto &outer_el: *outer_arr) {
        int in_idx = 0;
        for(auto &inner_el: *outer_el.as_array()) {
          outVal[out_idx][in_idx++] = inner_el.as_integer()->get();
        }
        out_idx++;
      }
      return true;
    }

    /// wrapper around above that allows you to clobber anything in the table with the default.  Allows you to write
    /// one-liners like:
    /// writeRegister(OFFSET, fTab.getValueOrDefaultAtKeyPath("key1", 0.001, true, false))
    /// writeRegister(OFFSET, fTab.getValueOrDefaultAtKeyPath("key2", 1.234, false, true))
    template <class T>
    T getValueOrDefaultAtKeyPath(const string& inKeyPath, T defaultVal, bool reportMissing=true, bool forceDefault=false) const
    {
      if(forceDefault) return defaultVal;
      if(fTablePtr == nullptr) {
        hlog::error("getValueOrDefaultAtKeyPath => fTablePtr is null!  Maybe you called this with no config file, but with forceDefault=false?");
        return defaultVal;
      }
      auto outVal = defaultVal;
      getValueAtKeyPath(inKeyPath, outVal, reportMissing);
      return outVal;
    }

    /// Version of above that helps with string literals
    string getValueOrDefaultAtKeyPath(const string& inKeyPath, const char* defaultVal, bool reportMissing=true, bool forceDefault=false) const
    {
      if(forceDefault) return defaultVal;
      if(fTablePtr == nullptr) {
        hlog::error("getValueOrDefaultAtKeyPath => fTablePtr is null!  Maybe you called this with no config file, but with forceDefault=false?");
        return defaultVal;
      }
      string outVal = defaultVal;
      getValueAtKeyPath(inKeyPath, outVal, reportMissing);
      return outVal;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Special getters made for complex mapping.  Fills hash maps of values for boards, connectors, etc.
    /// The config should be stored in an array of inline tables that looks like this:
    /// {id=ID_Code_in_Hex, data=vector_of_data], e.g., [ {id=0xDDDDD000,data=[ 0, 0]}, {id=0xDDDDD001,data=[ 1, 1]}, etc. ]
    bool getValueAtKeyPath(const string &inKeyPath, map<uint32_t, vector<int>> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if(!node_view) {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }

      /// first, we verify that this is an array of tables
      auto arr = node_view.as_array();
      if (!arr->is_homogeneous<toml::table>()) {
        hlog::error("getValueAtKeyPath => Malformed data at keypath '{}'!  Expecting an array of arrays", inKeyPath);
        hlog::error("getValueAtKeyPath => Expecting an array of inline tables of form [ {id=id_code, data=[data] } ]");
        return false;
      }

      /// If we are here, we should be good to go.
      map<uint32_t, vector<int>> tmpMap;
      int out_idx = 0;
      for(auto &arr_el: *arr) {
        /// getting the key is easy
        uint32_t key = arr_el.as_table()->get("id")->as_integer()->get();

        /// to get the data, we need to confirm that it is an array of integers
        auto data = arr_el.as_table()->get("data")->as_array();
        if(!data->is_homogeneous<int64_t>()) {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' is contains non-integer values in table with key 0x{:X}", inKeyPath, key);
          return false;
        }
        vector<int> tmpVec;
        for(auto &el: *data) {
          tmpVec.push_back(el.as_integer()->get());
        }

        if(tmpMap.find(key) != tmpMap.end()) {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' has duplicate key 0x{:X}", inKeyPath, key);
          return false;
        } else {
          tmpMap[key] = tmpVec;
        }
        //hlog::info("key = 0x{:8X} | data = {}", key, tmpVec);
        out_idx++;
      }
      outVal = tmpMap;
      return true;
    }

    /// this one reads in an array of nested maps in the form
    /// [ {id=ID_Code_in_Hex, data={inline table} }, {} ], e.g., {id=0xDDDDD000,data={sipm=0, conn=0}}
    bool getValueAtKeyPath(const string &inKeyPath, map<uint32_t, map<string, int>> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if(!node_view) {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }

      /// first, we verify that this is an array of tables
      auto arr = node_view.as_array();
      if (!arr->is_array_of_tables()) {
        hlog::error("getValueAtKeyPath => Malformed data at keypath '{}'!  Expecting an array of tables!", inKeyPath);
        hlog::error("getValueAtKeyPath => Expecting an array of inline tables of form [ {id=id_code, data={data} } ]");
        return false;
      }

      /// If we are here, we should be good to go.
      map<uint32_t, map<string, int>> tmpMap;
      for(auto &arr_el: *arr) {
        uint32_t key = arr_el.as_table()->get("id")->as_integer()->get();
        auto data    = arr_el.as_table()->get("data")->as_table();
        if(!data || !data->is_table()) {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' is contains non-table data at key 0x{:X}", inKeyPath, key);
          return false;
        }
        map<string, int> tmpInnerMap;
        for(auto &el: *data){
          string innerKey = el.first.data();
          int innerData = el.second.as_integer()->get();
          tmpInnerMap[innerKey] = innerData;  // NB! we don't check for dupe keys!
        }

        if(tmpMap.find(key) != tmpMap.end()) {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' has duplicate key 0x{:X}", inKeyPath, key);
          return false;
        } else {
          tmpMap[key] = tmpInnerMap;
        }
        //hlog::info("key = 0x{:8X} | data = {}", key, tmpInnerMap);
      }
      outVal = tmpMap;
      return true;
    }

    /// this one reads in an array of nested maps in the form
    /// [ {id=ID_Code_in_Hex, data={inline table} }, {} ], e.g., {id=0xDDDDD000,data={varA=9.222, varB=0.3403}}
    bool getValueAtKeyPath(const string &inKeyPath, map<uint32_t, map<string, double>> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if(!node_view) {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }

      /// first, we verify that this is an array of tables
      auto arr = node_view.as_array();
      if (!arr->is_array_of_tables()) {
        hlog::error("getValueAtKeyPath => Malformed data at keypath '{}'!  Expecting an array of tables!", inKeyPath);
        hlog::error("getValueAtKeyPath => Expecting an array of inline tables of form [ {id=id_code, data={data} } ]");
        return false;
      }

      /// If we are here, we should be good to go.
      map<uint32_t, map<string, double>> tmpMap;
      for(auto &arr_el: *arr) {
        uint32_t key = arr_el.as_table()->get("id")->as_integer()->get();
        auto data    = arr_el.as_table()->get("data")->as_table();
        if(!data || !data->is_table()) {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' is contains non-table data at key 0x{:X}", inKeyPath, key);
          return false;
        }
        map<string, double> tmpInnerMap;
        for(auto &el: *data){
          string innerKey = el.first.data();
          double innerData = el.second.as_floating_point()->get();
          tmpInnerMap[innerKey] = innerData;  // NB! we don't check for dupe keys!
        }

        if(tmpMap.find(key) != tmpMap.end()) {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' has duplicate key 0x{:X}", inKeyPath, key);
          return false;
        } else {
          tmpMap[key] = tmpInnerMap;
        }
        //hlog::info("key = 0x{:8X} | data = {}", key, tmpInnerMap);
      }
      outVal = tmpMap;
      return true;
    }

    /// this one reads in an array of nested maps in the form
    /// [ {id=int, data={inline table with doubles} }, {} ], e.g., {id=444,data={sipm=0.0, conn=0.0}}
    bool getValueAtKeyPath(const string &inKeyPath, map<int, map<string, double>> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if(!node_view) {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }

      /// first, we verify that this is an array of tables
      auto arr = node_view.as_array();
      if (!arr->is_array_of_tables()) {
        hlog::error("getValueAtKeyPath => Malformed data at keypath '{}'!  Expecting an array of tables!", inKeyPath);
        hlog::error("getValueAtKeyPath => Expecting an array of inline tables of form [ {id=id_code, data={data} } ]");
        return false;
      }

      /// If we are here, we should be good to go.
      map<int, map<string, double>> tmpMap;
      for(auto &arr_el: *arr) {
        int key = arr_el.as_table()->get("id")->as_integer()->get();
        auto data    = arr_el.as_table()->get("data")->as_table();
        if(!data || !data->is_table()) {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' is contains non-table data at key 0x{:X}", inKeyPath, key);
          return false;
        }
        map<string, double> tmpInnerMap;
        for(auto &el: *data){
          string innerKey = el.first.data();
          double innerData = el.second.as_floating_point()->get();
          tmpInnerMap[innerKey] = innerData;  // NB! we don't check for dupe keys!
        }

        if(tmpMap.find(key) != tmpMap.end()) {
          hlog::error("getValueAtKeyPath => Value at keypath '{}' has duplicate key 0x{:X}", inKeyPath, key);
          return false;
        } else {
          tmpMap[key] = tmpInnerMap;
        }
        //hlog::info("key = 0x{:8X} | data = {}", key, tmpInnerMap);
      }
      outVal = tmpMap;
      return true;
    }

    /// This is a slightly more complicated version of the above, where the key is now a pair<int>, such as you might have
    /// with row/column or lane/channel-indexed data. Data should look like this:
    /// [ {id=[0,0], data={a=x.xx, b=y.yyy, c=z.zzz}, {}, {}, ... ]
    bool getValueAtKeyPath(const string &inKeyPath, map<pair<int, int>, map<string, double>> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if(!node_view) {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }

      /// first, we verify that this is an array of tables
      auto arr = node_view.as_array();
      if (!arr->is_array_of_tables()) {
        hlog::error("getValueAtKeyPath => Malformed data at keypath '{}'!  Expecting an array of tables!", inKeyPath);
        hlog::error("getValueAtKeyPath => Expecting an array of tables of form [id=[0,0], data={a=x.xx, b=y.yyy, c=z.zzz}]");
        return false;
      }

      /// If we are here, we should be good to go.
      map<pair<int, int>, map<string, double>> tmpOut;
      for(auto &arr_el: *arr) {

        auto el_table = arr_el.as_table();
        if(!el_table){
          hlog::error("Couldn't convert array element into a table");
          return false;
        }
        /// get the key
        auto id_node = el_table->get("id");
        if(!id_node || !id_node->is_array()){
          hlog::error("getValueAtKeyPath => Malformed key at keypath '{}'! Missing key 'id' or 'id' is not an array", inKeyPath);
          return false;
        }
        auto id_array = id_node->as_array();
        if(!id_array->is_homogeneous<int64_t>() || id_array->size() != 2){
          hlog::error("getValueAtKeyPath => Malformed key at keypath '{}'! Key 'id' is not a homogeneous int array of size 2!", inKeyPath);
          return false;
        }
        int locX = (int)id_array->at(0).as_integer()->get();
        int locY = (int)id_array->at(1).as_integer()->get();
        pair<int, int> keyPair = {locX, locY};

        /// now get the data
        auto data_node = el_table->get("data");
        if(!data_node || !data_node->is_table()){
          hlog::error("getValueAtKeyPath => Malformed data at keypath '{}'! Missing key 'data' or 'data' is not a table", inKeyPath);
          return false;
        }

        auto data_table = data_node->as_table();
        map<string, double> tmpInnerMap;
        for(auto &el: *data_table){
          string innerKey = el.first.data();
          auto node = el.second.as_floating_point();
          if(node){
            double innerData = el.second.as_floating_point()->get();
            tmpInnerMap[innerKey] = innerData;  // NB! we don't check for dupe keys!
          } else {
            hlog::error("getValueAtKeyPath => Value at keypath '{}' contains non-float data at key {}", inKeyPath, keyPair);
            return false;
          }
        }
        tmpOut[keyPair] = tmpInnerMap;
        //hlog::info("key = {} | data = {}", keyPair, tmpInnerMap);
      }
      outVal = tmpOut;
      return true;
    }

    /// Int version of the above
    /// [ {id=[0,0], data={a=x, b=y, c=z}, {}, {}, ... ]
    bool getValueAtKeyPath(const string &inKeyPath, map<pair<int, int>, map<string, int>> &outVal, bool reportMissing = true) const {
      if (isZombie()) return false;

      auto node_view = fTablePtr->at_path(inKeyPath);
      if(!node_view) {
        if (reportMissing) hlog::error("getValueAtKeyPath => Missing keypath '{}'!", inKeyPath);
        return false;
      }

      /// first, we verify that this is an array of tables
      auto arr = node_view.as_array();
      if (!arr->is_array_of_tables()) {
        hlog::error("getValueAtKeyPath => Malformed data at keypath '{}'!  Expecting an array of tables!", inKeyPath);
        hlog::error("getValueAtKeyPath => Expecting an array of tables of form [id=[0,0], data={a=x.xx, b=y.yyy, c=z.zzz}]");
        return false;
      }

      /// If we are here, we should be good to go.
      map<pair<int, int>, map<string, int>> tmpOut;
      for(auto &arr_el: *arr) {

        auto el_table = arr_el.as_table();
        if(!el_table){
          hlog::error("Couldn't convert array element into a table");
          return false;
        }
        /// get the key
        auto id_node = el_table->get("id");
        if(!id_node || !id_node->is_array()){
          hlog::error("getValueAtKeyPath => Malformed key at keypath '{}'! Missing key 'id' or 'id' is not an array", inKeyPath);
          return false;
        }
        auto id_array = id_node->as_array();
        if(!id_array->is_homogeneous<int64_t>() || id_array->size() != 2){
          hlog::error("getValueAtKeyPath => Malformed key at keypath '{}'! Key 'id' is not a homogeneous int array of size 2!", inKeyPath);
          return false;
        }
        int locX = (int)id_array->at(0).as_integer()->get();
        int locY = (int)id_array->at(1).as_integer()->get();
        pair<int, int> keyPair = {locX, locY};

        /// now get the data
        auto data_node = el_table->get("data");
        if(!data_node || !data_node->is_table()){
          hlog::error("getValueAtKeyPath => Malformed data at keypath '{}'! Missing key 'data' or 'data' is not a table", inKeyPath);
          return false;
        }

        auto data_table = data_node->as_table();
        map<string, int> tmpInnerMap;
        for(auto &el: *data_table){
          string innerKey = el.first.data();
          auto node = el.second.as_integer();
          if(node){
            double innerData = el.second.as_integer()->get();
            tmpInnerMap[innerKey] = innerData;  // NB! we don't check for dupe keys!
          } else {
            hlog::error("getValueAtKeyPath => Value at keypath '{}' is contains non-float data at key {}", inKeyPath, keyPair);
            return false;
          }
        }
        tmpOut[keyPair] = tmpInnerMap;
        //hlog::info("key = {} | data = {}", keyPair, tmpInnerMap);
      }
      outVal = tmpOut;
      return true;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Merge table test code. - uses ROOT stuff, which we don't want to include in the CommonUtil lib, so comment out.
    /*
    bool mergeTOMLFiles(string inFinalFilename){

      string tmp_name = "/tmp/tmp.toml";

      // first, dump the current data to a file...
      if(fTablePtr) {
        ofstream opf(tmp_name, ios::out);

        // info on this here: https://github.com/marzer/tomlplusplus/pull/89
        // SPW has edit at line 9526 to add fixed point
        using formatter = toml::toml_formatter;
        opf << formatter{ *fTablePtr, formatter::default_flags | toml::format_flags::relaxed_float_precision };
        opf.close();
      } else {
        hlog::error("mergeTOMLFiles => Bad table pointer!  Couldn't write tmp file");
      }

      //TPython::LoadMacro( "../FlightOps/test/test_toml_merge.py");
      gSystem->Exec(format("./config/toml_merge.py --master {} --delta {} --out {}", fTopTableFilename, tmp_name, inFinalFilename).c_str());

      return true;
    }
    */

};
