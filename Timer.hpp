#pragma once
#include <chrono>
#include <string>

namespace util{

  class Timer{
    std::chrono::high_resolution_clock::time_point start_time;
  public:
    void start(){
      start_time=std::chrono::high_resolution_clock::now();
    }
    double stop(){
      return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-start_time).count();
    }

    static std::string format(double t){
      if(t<10.0/1000000000){
        return "<10 ns";
      }
      if(t<10.0/1000000){
        return std::to_string(int(t*1000000000))+" ns";
      }
      if(t<10.0/1000){
        return std::to_string(int(t*1000000))+" μs";
      }
      if(t<10.0/1){
        return std::to_string(int(t*1000))+" ms";
      }
      return std::to_string(int(t))+" s";
    }
  };

}
