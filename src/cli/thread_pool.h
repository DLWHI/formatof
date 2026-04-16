#ifndef STDEXT_THREAD_POOL_H_
#define STDEXT_THREAD_POOL_H_

#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include "task.h"

namespace stdext {
  class thread_pool {
   public:
    thread_pool(size_t num_threads) {
      for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back(std::thread([this] {
          while (true) {
            std::shared_ptr<functor> task;
            {
              std::unique_lock<std::mutex> lock(queue_lock_);
              condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
              if (stop_ && tasks_.empty()) {
                return;
              }
              task = std::move(tasks_.front());
              tasks_.pop();
            }
            (*task)();
          }
        }));
      }
    }

    virtual ~thread_pool() {
      {
        std::unique_lock<std::mutex> lock(queue_lock_);
        stop_ = true;
      }
      condition_.notify_all();
      for (std::thread& worker : workers_) {
        worker.join();
      }
    }

    template <typename F, typename... Args>
    std::shared_ptr<task<typename std::result_of<F(Args...)>::type, Args...>>
    enqueue(F&& f, Args&&... args) {
      auto task_ptr = new task<typename std::result_of<F(Args...)>::type, Args...>(
          std::forward<F>(f), std::forward<Args>(args)...);
      tasks_.emplace(task_ptr);

      return std::static_pointer_cast<
          task<typename std::result_of<F(Args...)>::type, Args...>>(tasks_.back());
    }

    void stop() noexcept { stop_ = true; }

   private:
    std::vector<std::thread> workers_;
    std::queue<std::shared_ptr<functor>> tasks_;
    std::mutex queue_lock_;
    std::condition_variable condition_;
    bool stop_ = false;
  };
}  // namespace stdext

#endif  // STDEXT_THREAD_POOL_H_
