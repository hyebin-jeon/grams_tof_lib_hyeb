///////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_StatTools.h
/// Created  : Sun Jan 1 2024 by S.P. Wakely
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <deque>
#include <chrono>

using namespace std;

class HLX_RunningAveragePoint {
    using hrc = std::chrono::high_resolution_clock;
  public:
    explicit HLX_RunningAveragePoint(double val){
      fValue = val;
      fTimestamp  = hrc::now();
    }
    ~HLX_RunningAveragePoint() = default;


  private:
    hrc::time_point fTimestamp{hrc::now()};
    double          fValue{0};

  public:
    double getValue() const {return fValue;}
    hrc::time_point getTimestamp() const {return fTimestamp;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Running average class - might be thread safe, might not be.  Try it and see!
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HLX_RunningAverage {
    using hrc = std::chrono::high_resolution_clock;

  public:
    explicit HLX_RunningAverage(double windowInSec, bool useEMA=false): fWindowLength(windowInSec), fUseEMA(useEMA){}
    ~HLX_RunningAverage() = default;

  public:
    double                         fWindowLength{60};
    deque<HLX_RunningAveragePoint> fValues;
    bool                           fUseEMA{false};

    std::mutex                     fMutex{};

    void removeExpiredValues() {
      lock_guard<mutex> lk(fMutex);
      auto now = hrc::now();
      auto windowInMilliseconds = chrono::milliseconds((long)(fWindowLength*1000));
      while(!fValues.empty() && (now - fValues.front().getTimestamp()) > windowInMilliseconds) {
        fValues.pop_front();
      }
    }
    void setEMA(bool useEMA){fUseEMA = useEMA;}

    double getWindowLength() const {return fWindowLength;}

    /// add all the values
    double getTotal() {
      /// always remove old shit first.
      removeExpiredValues();
      lock_guard<mutex> lk(fMutex);
      if (fValues.empty()) {
        return 0; // Handle empty case
      }
      double total{0};
      for (const auto& p : fValues) {
        total += p.getValue();
      }
      return total;
    }

    // Average of the last N values
    double getAverage() {
      /// always remove old shit first.
      removeExpiredValues();

      lock_guard<mutex> lk(fMutex);
      if (fValues.empty()) {
        return 0; // Handle empty case
      }

      if(fUseEMA){
        //calculate exponential moving average
        double alpha = 2.0 / (fWindowLength + 1.0);
        double average{0};
        for (const auto& p : fValues) {
          average = (1 - alpha) * average + alpha * p.getValue();
        }
        return average;
      } else {
        double average = 0;
        for(const auto &p: fValues){
          average += p.getValue()/fWindowLength;
        }
        return average;
      }
    }

    // Average of the actual number of entries
    double getCountAverage() {
      /// always remove old shit first.
      removeExpiredValues();

      lock_guard<mutex> lk(fMutex);
      if (fValues.empty()) {
        return 0; // Handle empty case
      }

      if(fUseEMA){
        //calculate exponential moving average
        double alpha = 2.0 / (fWindowLength + 1.0);
        double average{0};
        for (const auto& p : fValues) {
          average = (1 - alpha) * average + alpha * 1.0;
        }
        return average;
      } else {
        double average = 0;
        for(const auto &p: fValues){
          average += 1.0/fWindowLength;
        }
        return average;
      }
    }

    void addValue(double value) {
      fMutex.lock();
      fValues.emplace_back(value);
      fMutex.unlock();
      removeExpiredValues();
    }

    void reset() {
      lock_guard<mutex> lk(fMutex);
      fValues.clear();
    }
};
