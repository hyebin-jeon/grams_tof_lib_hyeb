#include <iostream>
#include <cstring>

#include <chrono>
#include <thread>
#include <mutex>

#include "HLX_Logger.h"
#include "HLX_Stopwatch.h"

int main()
{

  HLX_Stopwatch inner;
  int cnt{0};
  while(!inner.isTimerExceeded(123e-6, false)){
    cnt++;
  }
  inner.stop();
  inner.dump();

  cout << inner.getStartStopTime() << endl;
  cout << inner.getStartStopTimeMicro() << endl;
  cout << inner.getStartStopTimeNano() << endl;
  cout << "count was: " << cnt << endl;

  return 0;
}
