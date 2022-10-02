#pragma once
#include <list>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>

struct Task;
struct Threadpool;
//typedef std::queue<Task*,std::list<Task*>> TaskQueue;

class Task{
    bool delete_on_finish=true;
public:
    Task(){}
    virtual ~Task(){}
    Task(bool delete_on_finish):delete_on_finish(delete_on_finish){}
    bool will_delete_on_finish(){return delete_on_finish;}
    virtual void perform()=0;
};




class Threadpool{
    std::queue<Task*,std::list<Task*>> queue;
    std::mutex mtx;
    std::thread* threads;
    bool* engaged;
    bool kill_threads=false;

    static void work(Threadpool* pool,int t_idx){
        while(!pool->kill_threads){
            Task* task=pool->pop();
            if(task!=nullptr){
                pool->engaged[t_idx]=true;
                task->perform();
                if(task->will_delete_on_finish()){
                    delete task;
                }
            }else{
                pool->engaged[t_idx]=false;
                std::this_thread::yield();
            }
        }
    }

    Task* pop(){
        mtx.lock();
        if(queue.empty()){
            mtx.unlock();
            return nullptr;
        }else{
            Task* ret=queue.front();
            queue.pop();
            mtx.unlock();
            return ret;
        }
    }

    size_t size(){
        mtx.lock();
        size_t ret=queue.size();
        mtx.unlock();
        return ret;
    }

    bool any_engaged(){
        for(int n=0;n<thread_count;n++){
            if(engaged[n]){return true;}
        }
        return false;
    }
public:
    const size_t thread_count;

    Threadpool(size_t thread_count):thread_count(thread_count){
        threads=new std::thread[thread_count];
        engaged=new bool[thread_count];
        for(size_t n=0;n<thread_count;n++){
            threads[n]=std::thread(work,this,n);
            engaged[n]=true;
        }
    }

    void push(Task* task){
        mtx.lock();
        queue.push(task);
        mtx.unlock();
    }

    size_t tasks_left(){
        mtx.lock();
        size_t ret=queue.size();
        mtx.unlock();
        return ret;
    }

    bool empty(){
        mtx.lock();
        bool ret=queue.empty();
        mtx.unlock();
        return ret;
    }

    void kill(){
        kill_threads=true;
        for(size_t n=0;n<thread_count;n++){
            threads[n].join();
        }
    }

    void finish(){
        while(!queue.empty() || any_engaged()){
            std::this_thread::yield();
        }
    }

    void join(){
        finish();
        kill();
    }

    ~Threadpool(){
        kill();
        delete [] threads;
        delete [] engaged;
    }
};
