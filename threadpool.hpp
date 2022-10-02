#pragma once
#include <list>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>

struct Task;
struct TaskQueue;
struct Worker;
struct Threadpool;
//typedef std::queue<Task*,std::list<Task*>> TaskQueue;

struct Task{
    const bool delete_on_finish=true;
    Task(){}
    virtual ~Task(){}
    Task(bool delete_on_finish):delete_on_finish(delete_on_finish){}
    virtual void perform()=0;
};




class Threadpool{

    class TaskQueue{
        std::queue<Task*,std::list<Task*>> queue;
        std::mutex mtx;
    public:
        void push(Task* task){
            mtx.lock();
            queue.push(task);
            mtx.unlock();
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
        bool empty(){
            mtx.lock();
            bool ret=queue.empty();
            mtx.unlock();
            return ret;
        }
        size_t size(){
            mtx.lock();
            size_t ret=queue.size();
            mtx.unlock();
            return ret;
        }
    } queue;

    std::thread* threads;
    bool kill_threads=false;

    static void work(Threadpool& pool){
        while(!pool.kill_threads){
            Task* task=pool.queue.pop();
            if(task!=nullptr){
                task->perform();
                if(task->delete_on_finish){
                    delete task;
                }
            }else{
                std::this_thread::yield();
            }
        }
    }
public:
    const size_t thread_count;

    Threadpool(size_t thread_count):thread_count(thread_count){
        threads=new std::thread[thread_count];
        for(size_t n=0;n<thread_count;n++){
            threads[n]=std::thread(work,*this);
        }
    }

    void push(Task* task){
        queue.push(task);
    }

    size_t tasks_left(){
        return queue.size();
    }

    void kill(){
        kill_threads=true;
        for(size_t n=0;n<thread_count;n++){
            threads[n].join();
        }
    }

    void finish(){
        while(!queue.empty()){
            std::this_thread::yield();
        }
    }

    void join(){
        finish();
        kill();
    }

    ~Threadpool(){
        kill();
    }
};
