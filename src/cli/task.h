#ifndef STDEXT_TASK_H_
#define STDEXT_TASK_H_

#include <functional>
#include <future>

namespace stdext {
  class functor {
   public:
    virtual ~functor() = default;

    virtual void operator()() = 0;
  };

  template <typename R, typename... Args>
  class task : public functor {
   public:
    template <typename F>
    task(F&& functor, Args&&... args)
        : task_(std::bind(functor, std::forward<Args>(args)...)) {}
    virtual ~task() = default;

    void operator()() override { task_(); }

    R get() { return task_.get_future().get(); }

   private:
    std::packaged_task<R()> task_;
  };
}  // namespace stdext

#endif  // STDEXT_TASK_H_
