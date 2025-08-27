#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

/// functions in here need to be inlined to prevent multiple definition errors from the linker
namespace hlx {

    // convert offset binary to signed
    inline long convertOffsetBinary(unsigned long inVal, int inBits)
    {
      unsigned long halfMax = std::pow(2, inBits)/2;
      long outVal = inVal - halfMax;
      return outVal;
    }

    // convert N-bit 2's complement to signed
    inline int convert2sToSigned(unsigned long inVal, int inBits=13) {
      const int kBitCount = inBits; //includes sign
      const int kBitMask = (1 << kBitCount) - 1;
      const int kSignMask = (1 << (kBitCount - 1));
      const int kValMask = kSignMask - 1;

      // if you were working with any random input, you would probably want
      // to truncate the input to the maximum bit spread here
      //uint16_t truncADC = fADC&kBitMask;

      int out{};
      if ((inVal & kSignMask) != 0) { //if sign bit is set
        out = inVal - (1 << kBitCount);
      } else {
        out = inVal & kValMask;
      }
      return out;
    }

    // convert signed to N-bit 2's complement - not 100% certain about this.
    inline unsigned long convertSignedTo2s(int inVal, int inBits=13) {
      if (inBits < 1 || inBits > (int)sizeof(unsigned long) * 8) {
        return 0;
      }
      // Calculate the maximum positive value for the given number of bits
      long minPositiveValue = -static_cast<long>(pow(2, inBits - 1));
      long maxPositiveValue = static_cast<long>(pow(2, inBits - 1)) - 1;
      if (inVal < minPositiveValue || inVal > maxPositiveValue) {
        std::cerr << "convertSignedTo2sA => Value out of range for " << inBits << " bits." << std::endl;
        return 0;
      }

      // Calculate the two's complement representation
      if (inVal >= 0) {
        return static_cast<unsigned long>(inVal);
      } else {
        return static_cast<unsigned long>(pow(2, inBits)) + inVal;
      }
    }

    inline std::vector<int> convert2sToSigned(const std::vector<uint32_t>& inVal, int inBits=13) {
      std::vector<int> out;
      for (const auto &val : inVal) {
        out.push_back(convert2sToSigned(val, inBits));
      }
      return out;
    }

    /// convert dwords vector to word vector
    inline std::vector<uint16_t> convertWords32To16(const std::vector<uint32_t> &data){
      std::vector<uint16_t> data16;
      for(size_t idx = 0; idx < data.size(); idx++) {
        data16.push_back(data[idx] >> 16);
        data16.push_back(data[idx] & 0xFFFF);
      }
      return data16;
    }

    /// reverse word order inside a dword vector
    inline void reverseByWord(std::vector<uint32_t> &inData){
      for(auto &el:inData) el = (el>>16)|(el<<16);
    }

    /// modern memcpy replacement
    inline float convertUint32ToFloat(uint32_t inVal) {
      return *reinterpret_cast<float*>(&inVal);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sort and truncates a vector of numbers
    template<class T>
    std::vector<T> getSortedAndTruncated(const std::vector<T> &vec, double lowFrac=0, double highFrac=0)
    {
      std::vector<T> out(vec);
      std::sort(out.begin(), out.end()); //in-place sort.

      if(lowFrac < 0 || highFrac < 0 || lowFrac + highFrac >= 1.0) {
        std::cerr << "getSortedAndTruncated => Invalid lowFrac/highFrac values." << std::endl;
        return out;
      }
      if(lowFrac == 0 && highFrac == 0) return out; //no truncation needed

      auto loIdx = (size_t)floor(vec.size() * lowFrac + 0.5);
      auto hiIdx = (size_t)floor(vec.size() * highFrac + 0.5);

      out.erase(out.begin(), out.begin() + loIdx);
      out.erase(out.end() - hiIdx, out.end());
      return out;
    }

    /// Keith's truncation code
    template<class T>
    std::vector<T> getSortedAndRMSTruncated(const std::vector<T> &vec, double truncateRMS=0){
      std::vector<T> out(vec);
      std::sort(out.begin(), out.end()); //in-place sort.

      if(truncateRMS < 0){
        std::cerr << "getSortedAndRMSTruncated => Invalid truncateRMS value of " << truncateRMS << std::endl;
        return out;
      }
      if(out.size() < 1){
        std::cerr << "getSortedAndRMSTruncated => Input vector needs >1 entry." << std::endl;
        return out;
      }

      std::vector<T> truncated;
      double sum = accumulate(out.begin(), out.end(), 0.0);
      double mean = sum / out.size();
      double accum = 0.0;
      for_each(out.begin(), out.end(), [&](T val) { accum += (val - mean) * (val - mean); });
      double sigma = sqrt(accum / (out.size() - 1));
      copy_if(out.begin(),out.end(),back_inserter(truncated),[&](T val){return (val> mean - truncateRMS * sigma) && (val < mean + truncateRMS * sigma);});
      return truncated;
    }

    /// Memory checker from Ken
    inline double getCurrentMemoryUsageMB(int type=0)
    {
      std::ifstream fin("/proc/self/status");
      if (!fin) return 0;

      std::string matchString = "VmSize:";
      if(type==0) matchString = "VmSize:";
      if(type==1) matchString = "VmRSS:";

      double vmsize;
      char buff[256];
      while (!fin.eof()) {
        fin >> buff;
        if (buff == matchString) {
          fin >> vmsize;
          return vmsize/1024.0;
        }

      }
      return 0;
    }
}//namespace hlx