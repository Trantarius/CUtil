#pragma once
#include <list>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include "YieldLock.hpp"

namespace util{



  class ThreadPool{
  public:
    typedef std::function<void()> task_t;

  private:

    class Worker{
      std::thread thread;
      std::atomic_bool dying=false;
      ThreadPool* pool;

      static void work_loop(Worker* self){
        while(!self->dying){
          self->pool->mutex.lock();
          if(self->pool->has_next()){
            task_t job=self->pool->get_next();
            self->pool->running_jobs++;
            self->pool->mutex.unlock();

            job();

            std::scoped_lock lock(self->pool->mutex);
            self->pool->running_jobs--;

          }else{
            self->pool->mutex.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(10));
          }
        }
      }

    public:
      Worker(ThreadPool* pool):
      pool(pool),thread(work_loop,this){}

      ~Worker(){
        dying=true;
        thread.join();
      }
    };

    std::queue<task_t> jobs;
    Worker** workers;
    int64_t running_jobs=0;
    YieldLock mutex;

    bool has_next(){
      return !jobs.empty();
    }
    task_t get_next(){
      task_t ret=jobs.front();
      jobs.pop();
      return ret;
    }

  public:
    const int64_t thread_count;

    ThreadPool(int64_t thread_count): thread_count(thread_count) {
      workers=new Worker*[thread_count];
      for(int n=0;n<thread_count;n++){
        workers[n]=new Worker(this);
      }
    }

    ~ThreadPool(){
      for(int n=0;n<thread_count;n++){
        delete workers[n];
      }
      delete [] workers;
    }


    void add(task_t act){
      std::scoped_lock lock(mutex);
      jobs.push(act);
    }

    int64_t job_count(){
      std::scoped_lock lock(mutex);
      return jobs.size() + running_jobs;
    }
  };


}
