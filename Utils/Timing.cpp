#include "Timing.h"
#include <thread>
using namespace std;
using namespace std::chrono;
void Sleep(double seconds){
  std::chrono::duration<double > d(seconds);
  std::this_thread::sleep_for(d);
}

StopWatch::StopWatch(){

  Reset();
}

void StopWatch::Reset(){
  setPoint = high_resolution_clock::now();
  onStop = setPoint;
  running = false;
}

void StopWatch::Start(){
  running = true;
}

void StopWatch::Stop(){
  running = false;
  onStop = high_resolution_clock::now();
}

double StopWatch::ElapsedSeconds(){
  high_resolution_clock::time_point tp = high_resolution_clock::now();
  if(running == false){
    tp = onStop;
  }
  duration<double> time_span = duration_cast<duration<double>>(tp - setPoint);
  return time_span.count();
}

double StopWatch::ElapsedMilliseconds(){
  return ElapsedSeconds()*1000.0;
}
