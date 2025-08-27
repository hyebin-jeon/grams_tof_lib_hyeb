///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_Stopwatch.h
/// Created  : 6/8/2019 13:12 by wakely
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdio>
#include <chrono>
#include <iostream>
#include <fmt/format.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief A simple utility class for timing intervals.
/// clock starts upon instantiation.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HLX_Stopwatch {

    // doing this lets us redefine clock in 1 go.
    using Clock = std::chrono::high_resolution_clock;

  public:
    explicit HLX_Stopwatch(bool startOn=true);
    ~HLX_Stopwatch()= default;

  private:
    uint64_t           fAccumulatedTime{}; //only accumulates on stops.
    Clock::time_point  fStart;
    Clock::time_point  fStop;
    bool               fIsRunning{false};

  public:
    void start();
    void stop();
    bool isRunning(){return fIsRunning;}

    double restart(); //restart returns current elapsed time and restarts

    /// Methods for getting elapsed time without stopping
    double   getRunningTime() const;
    double   getAccumulatedTime() const;
    double   getRunningTimeMilli() const;
    double   getRunningTimeMicro() const;
    double   getRunningTimeNano() const;
    uint64_t getRunningTimeRaw() const;
    string   getBestRunningTime() const;

    /// Elapsed time AFTER stop has been called.
    double   getStartStopTime()      const;
    double   getStartStopTimeMicro() const;
    double   getStartStopTimeNano() const;
    uint64_t getStartStopTimeRaw() const;
    string   getBestStartStopTime() const;
    string   getBestAccumulatedTime() const;

    /// returns true if timer is exceeded, and optionally resets if exceeded
    bool isTimerExceeded(double inDeltaT, bool reset=true);

    void dump(double someQuantity=0) const;

    /// accepts time literals (e.g., 1ms)
    template<class T>
    void addStartTimeOffset(std::chrono::duration<T> offsetDuration){
      fStart = fStart + offsetDuration;
    }
    // accepts doubles, assumed to be seconds
    void addStartTimeOffset(double offsetSeconds){
      auto offset_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(offsetSeconds));
      fStart = fStart + offset_ns;
    }


    /// takes a quantity and returns a rate (Hz) based on current running time
    template<typename T>
    double getRate(T someQuantity) const {
      if(getRunningTime()!=0){
        return double(someQuantity)/getRunningTime();
      }
      return -1;
    }
    template<typename T>
    double getAccumulatedRate(T someQuantity) const {
      if(getAccumulatedTime()!=0){
        return double(someQuantity)/getAccumulatedTime();
      }
      return -1;
    }

    // get a nice time string.
    static string getBestTimeString(uint64_t inNanoseconds){
      if(inNanoseconds == (uint64_t)-1) return "-NaN-";
      long double dT = inNanoseconds;
      string out = fmt::format("{:2.3f} ns", dT);
      if(dT>1e3)     out = fmt::format("{:2.3f} μs", dT/1e3);
      if(dT>1e6)     out = fmt::format("{:2.3f} ms", dT/1e6);
      if(dT>1e9)     out = fmt::format("{:2.3f} s", dT/1e9);
      if(dT>90e9)    out = fmt::format("{:2.3f} min", dT/60e9);
      if(dT>3.6e12)  out = fmt::format("{:2.3f} hrs", dT/3.6e12);
      if(dT>8.64e13) out = fmt::format("{:2.3f} days", dT/8.64e13);
      if(dT>4.648e14) out = fmt::format("{:2.3f} weeks", dT/4.648e14);
      return out;
    }


};



