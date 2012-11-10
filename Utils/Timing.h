#pragma once
#include <chrono>

void Sleep(double seconds);

class StopWatch{
  std::chrono::high_resolution_clock::time_point setPoint;
 std::chrono::high_resolution_clock::time_point onStop;
 bool running;
 public:
  StopWatch();
  void Reset();
  void Start();
  void Stop();

  double ElapsedSeconds();
  double ElapsedMilliseconds();

};
