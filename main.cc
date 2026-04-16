#include <iostream>
#include <mutex>

#include "src/cli/thread_pool.h"

static int values[10000] = {0};
static int threadid = 0;
std::mutex mtx;

int task(int start, int finish) {
  // {
    // std::lock_guard lock(mtx);
    // std::cout << "Hello from thread " << ++threadid << std::endl;
    // std::cout << "Starting to write from index " << start << " to " << finish - 1 << std::endl;
  // }
  int accum = 0;
  for (; start < finish; ++start) {
    values[start] = start;
    accum += start;
  }
  return accum;
}

int main() {
  int counter = 0;
  for (int i = 0; i < 10000; ++i) {
    counter += (values[i] == 0);
  }
  std::cout << "There were " << counter << " zeroes in array entries" << std::endl;
  stdext::thread_pool tp(4);
  std::shared_ptr<stdext::task<int, int, int>> tasks[4];
  int results[4];
  for (int i = 0; i < 4; ++i) {
    tasks[i] = tp.enqueue(task, i*2500, (i + 1)*2500);
  };
  for (int i = 0; i < 4; ++i) {
    results[i] = tasks[i]->get();
  };
  for (int i = 0; i < 4; ++i) {
    std::cout << "Got " << results[i] << " as result of task " << i << std::endl;
  };
  counter = 0;
  for (int i = 0; i < 10000; ++i) {
    counter += (values[i] != 0);
  }
  std::cout << "There is " << counter << " entries in array now" << std::endl;
  return 0;
}