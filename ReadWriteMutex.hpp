#pragma once

#include <mutex>
#include <atomic>
#include <thread>

namespace util{

  template<typename Lockable=std::mutex>
  class ReadWriteMutex{
    Lockable mutex;
    std::atomic_size_t counter=0;

    class Read{
      ReadWriteMutex* owner=nullptr;
      Read(){}
      Read(const Read& b):owner(b.owner){}
      Read(ReadWriteMutex* owner):owner(owner){}
      friend class ReadWriteMutex;
    public:
      void lock(){owner->lock_read();}
      void unlock(){owner->unlock_read();}
      bool try_lock(){return owner->try_lock_read();}
    };

    class Write{
      ReadWriteMutex* owner=nullptr;
      Write(){}
      Write(const Write& b):owner(b.owner){}
      Write(ReadWriteMutex* owner):owner(owner){}
      friend class ReadWriteMutex;
    public:
      void lock(){owner->lock_write();}
      void unlock(){owner->unlock_write();}
      bool try_lock(){return owner->try_lock_write();}
    };

  public:

    void lock_read(){
      mutex.lock();
      counter++;
      mutex.unlock();
    }

    void unlock_read(){
      counter--;
    }

    void lock_write(){
      mutex.lock();
      while(counter!=0){
        std::this_thread::yield();
      }
    }

    void unlock_write(){
      mutex.unlock();
    }

    bool try_lock_read(){
      if(mutex.try_lock()){
        counter++;
        mutex.unlock();
        return true;
      }
      return false;
    }

    bool try_lock_write(){
      if(mutex.try_lock()){
        if(counter==0){
          return true;
        }
        mutex.unlock();
        return false;
      }
      return false;
    }

    Read read{this};
    Write write{this};

  };

}
