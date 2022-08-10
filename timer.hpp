#include <chrono>
#include <string>

#define CLOCK std::chrono::high_resolution_clock

class Timer{
    CLOCK::time_point start_time;
public:
    void start(){
        start_time=CLOCK::now();
    }
    double stop(){
        return std::chrono::duration_cast<std::chrono::duration<double>>(CLOCK::now()-start_time).count();
    }

    static std::string format(double t){
        if(t<10.0/1000000000){
            return "<10 ns";
        }
        if(t<10.0/1000000){
            return std::to_string(int(t*1000000000))+" ns";
        }
        if(t<10.0/1000){
            return std::to_string(int(t*1000000))+" us";
        }
        if(t<10.0/1){
            return std::to_string(int(t*1000))+" ms";
        }
        return std::to_string(int(t))+" s";
    }
};

#undef CLOCK
