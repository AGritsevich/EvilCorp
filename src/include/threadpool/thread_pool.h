#pragma once

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "defines.h"

class ThreadPool {
public:

  typedef std::function<void()> fn_type;

  class Worker {    
  public:

    Worker()
      :enabled(true),fqueue()
      ,thread(&Worker::thread_fn, this)
    {}

    ~Worker() {
      enabled = false;
      cv.notify_one();	
      thread.join();
    }  

    void appendFn(fn_type fn) {
      std::unique_lock<std::mutex> locker(mutex);
      fqueue.push(fn);			
      cv.notify_one();
    }	

    size_t getTaskCount() { 
      std::unique_lock<std::mutex> locker(mutex);
      return fqueue.size();		
    }

    bool isEmpty() { 
      std::unique_lock<std::mutex> locker(mutex);
      return fqueue.empty();	
    }

  private:
    
    bool					enabled;
    std::condition_variable cv;
    std::queue<fn_type>		fqueue;		
    std::mutex				mutex;		
    std::thread				thread;			

    void thread_fn() {
      while (enabled) {
        std::unique_lock<std::mutex> locker(mutex);
        cv.wait(locker, [&](){ return !fqueue.empty() || !enabled; });				
        while(!fqueue.empty()) {
          fn_type fn = fqueue.front();					
          locker.unlock();
          fn();
          locker.lock();
          fqueue.pop();
        }				
      }
    }
  };

  typedef std::shared_ptr<Worker> worker_ptr;

  ThreadPool(size_t threads = 1)
  {
    if (threads == 0) {
      threads = 1;
    }

    for (size_t i = 0; i < threads; i++) {
      worker_ptr pWorker(new Worker);
      workers_.push_back(pWorker);
    }

    it_last_used_worker_ = workers_.begin();
  }

  ~ThreadPool() {}

  template<typename  _R, typename  _FN, typename... _ARGS>
  std::shared_ptr<AData<_R>> runAsync(_FN _fn, _ARGS... _args) {
    std::function<_R()> rfn = std::bind(_fn,_args...);  
    std::shared_ptr<AData<_R>> pData(new AData<_R>());
    fn_type fn = [=]() {
      pData->data = rfn();
      pData->ready = true;
    };
    auto pWorker = getFreeWorker();
    pWorker->appendFn(fn);
    return pData;
  }

  template<typename  _FN, typename... _ARGS>
  void runAsync(_FN _fn, _ARGS... _args) {
    auto pWorker = getFreeWorker();
    pWorker->appendFn(std::bind(_fn,_args...));
  }

  void waitFinishAllTasks() { // TODO optimize
    auto free_workers = 0;
    do {
      std::chrono::milliseconds timeout(100);
      std::this_thread::sleep_for(timeout);
      free_workers = 0;
      for (auto w:workers_) {
        free_workers += w->isEmpty() == true ? 1 : 0;
      }
    }while(free_workers != workers_.size());
  }

private:

  worker_ptr getFreeWorker() {
    worker_ptr pWorker;
    if(++it_last_used_worker_ == workers_.end()) {
      it_last_used_worker_ = workers_.begin();
    }

    return *it_last_used_worker_;
  }
 
  std::vector<worker_ptr> workers_; 
  std::vector<worker_ptr>::iterator it_last_used_worker_;
};

#endif // SRC_LIBRARY_THREADPOOL_H