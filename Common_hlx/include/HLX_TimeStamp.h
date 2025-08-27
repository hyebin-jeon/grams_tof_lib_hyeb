////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_TimeStamp.h
/// Created  : 9/20/2019 by wakely
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef HLX_TIMESTAMP_H
#define HLX_TIMESTAMP_H

#include "TTimeStamp.h"
#include <cmath>

/// Helper class for handling time relative to launch and landing times or other key times.
class HLX_TimeStamp {
  public:
    HLX_TimeStamp() = default;
    ~HLX_TimeStamp() = default;

  public:
    inline static TTimeStamp fLaunchTime24   {2024, 05, 28, 03, 21, 44};   //Official Launch Time - GMT
    inline static TTimeStamp fLandingTime24  {2024, 06, 03, 11, 47, 33};   //Official Landing Time - GMT
    inline static TTimeStamp fDAQReset24     {2024, 05, 29, 14, 05, 00};   //When the DAQ came back

    inline static TTimeStamp fRampDownStart24{2024, 06, 01, 22, 18, 48};   //Ramp down begun
    inline static TTimeStamp fRampDownEnd24  {2024, 06, 02, 00, 18, 19};   //Ramp down complete

    inline static std::vector<TTimeStamp> fBallastDrops24 = {
        TTimeStamp(2024, 05, 29, 12, 26, 0) ,
        TTimeStamp(2024, 05, 29, 12, 38, 0) ,
        TTimeStamp(2024, 05, 29, 12, 52, 0) ,
        TTimeStamp(2024, 05, 29, 13, 27, 0) ,
        TTimeStamp(2024, 05, 30, 12, 42, 41),
        TTimeStamp(2024, 05, 30, 12, 55, 13),
        TTimeStamp(2024, 05, 30, 13, 25,  3),
        TTimeStamp(2024, 05, 30, 15, 44, 57),
        TTimeStamp(2024, 05, 30, 16,  8, 33),
        TTimeStamp(2024, 05, 31, 13, 34,  0),
        TTimeStamp(2024, 05, 31, 13, 55,  0),
        TTimeStamp(2024, 05, 31, 14, 48,  0),
        TTimeStamp(2024, 06,  1, 14, 37,  0),
        TTimeStamp(2024, 06,  1, 14, 47,  0),
        TTimeStamp(2024, 06,  1, 14, 54,  0),
    };


  public:
    /// convert a TTimeStamp to a double (seconds), relative to launch or landing time
    /// \param ts - the TTimeStamp to convert
    /// \param inDays - if true, return the number of days since launch/landing
    /// \param fromLanding - if true, return the number of seconds since landing
    /// \return the number of seconds since launch or landing
    static double dT(const TTimeStamp &ts, bool inDays = false, bool fromLanding = false) {
      auto ts0 = fLaunchTime24;
      auto ts1 = fLandingTime24;
      if (fromLanding) ts0 = ts1;
      double dT = (ts - ts0); // operator overloading
      if (inDays) dT /= 86400.0;
      return dT;
    }
    /// quick version of above that defaults to days from launch - good for interactive root sessions
    static double dTD(const TTimeStamp &ts){
      return dT(ts, true, false);
    }

    /// convert a double (unix seconds + fractions of second) to a double (seconds), relative to launch or landing time
    /// \param ts - the double to convert
    /// \param inDays - if true, return the number of days since launch/landing
    /// \param fromLanding - if true, return the number of seconds since landing
    /// \return the number of seconds since launch or landing
    static double dT(double tsd, bool inDays = false, bool fromLanding = false) {
      time_t tsA = std::floor(tsd);
      int tsB = int(1e9*(tsd - tsA));
      TTimeStamp ts(tsA, tsB);
      return dT(ts, inDays, fromLanding);
    }
    /// quick version of above that defaults to days from launch - good for interactive root sessions
    static double dTD(double tsd){
      return dT(tsd, true, false);
    }
};

#endif
