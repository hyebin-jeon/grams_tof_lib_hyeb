///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_Flight_Stopwatch.cpp
/// Created  : 6/8/2019 13:12 by wakely
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "HLX_Stopwatch.h"

using namespace std::chrono;

HLX_Stopwatch::HLX_Stopwatch(bool startOn)
{
  fStart     = Clock::now();
  fStop      = fStart;
  if(startOn) fIsRunning = true;
}

void HLX_Stopwatch::start()
{
  // don't let multiple starts change time
  if(!fIsRunning)fStart = Clock::now();
  fIsRunning = true;
}

void HLX_Stopwatch::stop()
{
  // don't let multiple stops change time!
  if(fIsRunning){
    fAccumulatedTime += getRunningTimeRaw();
    fStop = Clock::now();
  }
  fIsRunning = false;
}

double HLX_Stopwatch::restart()
{
  chrono::duration<double> dT = Clock::now()-fStart;
  fStart = Clock::now();
  fAccumulatedTime = 0;
  fIsRunning = true;
  return dT.count();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Super helpful for timed loops.  Note that this does not check if the timer is running or not!  That's probably
/// a bug.
/// \param inDeltaT  time to check against in seconds!
/// \param reset     if true, timer will restart when condition is true.
/// \return          true is exceeded, false if not
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool HLX_Stopwatch::isTimerExceeded(double inDeltaT, bool reset)
{
  chrono::duration<double> dT = Clock::now()-fStart;
  if(dT.count() > inDeltaT){
    if(reset){
      fStart = Clock::now();
    }
    return true;
  } else {
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Running times
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double HLX_Stopwatch::getRunningTime() const
{
  return getRunningTimeRaw()/1e9;
}

double HLX_Stopwatch::getAccumulatedTime() const
{
  return fAccumulatedTime/1e9;
}

double HLX_Stopwatch::getRunningTimeMicro() const
{
  return getRunningTimeRaw()/1e3;
}

double HLX_Stopwatch::getRunningTimeMilli() const
{
  return getRunningTimeRaw()/1e6;
}

double HLX_Stopwatch::getRunningTimeNano() const
{
  return getRunningTimeRaw()*1.0;
}

string HLX_Stopwatch::getBestRunningTime() const
{
  return getBestTimeString(getRunningTimeRaw());
}


uint64_t HLX_Stopwatch::getRunningTimeRaw() const
{
  if(fIsRunning != true){
    //cerr << "Requested Running time on a stopped stopwatch!" << endl;
    return -1;
  }
  return chrono::duration_cast<chrono::nanoseconds>(Clock::now()  - fStart).count();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Stop-Start times
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double HLX_Stopwatch::getStartStopTime() const
{
  return getStartStopTimeRaw()/1e9;
}

double HLX_Stopwatch::getStartStopTimeMicro() const
{
  return getStartStopTimeRaw()/1e3;
}

double HLX_Stopwatch::getStartStopTimeNano() const
{
  return getStartStopTimeRaw()*1.0;
}

string HLX_Stopwatch::getBestStartStopTime() const
{
  return getBestTimeString(getStartStopTimeRaw());
}

string HLX_Stopwatch::getBestAccumulatedTime() const
{
  return getBestTimeString(fAccumulatedTime);
}

uint64_t HLX_Stopwatch::getStartStopTimeRaw() const
{
  if(fIsRunning == true){
    cerr << "Requested Start/Stop time on a running stopwatch!" << endl;
    return 0;
  }
  return chrono::duration_cast<chrono::nanoseconds>(fStop - fStart).count();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Misc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HLX_Stopwatch::dump(double someQuantity) const
{
  chrono::duration<double> dT = Clock::now()-fStart;
  std::cout << "Time since last start: " << dT.count() << std::endl;
  if(someQuantity != 0) {
    std::cout << "Rate was             : " << someQuantity/dT.count() << std::endl;
  }
}





