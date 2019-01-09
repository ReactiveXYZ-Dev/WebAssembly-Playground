#include <sys/uio.h>
#include <emscripten.h>
#include <emscripten/threading.h>
#include <thread>
#include <queue>
#include <iostream>
#include <mutex>

std::mutex cout_mutex;

unsigned int calc(int i) {
  std::queue<int> q;
  for (int i = 1; i < 130000; ++i){
    q.push(i);
  }

  uint64_t sum = 10;
  
  while (!q.empty()) {
    sum += q.front();
    q.pop();
  }

  cout_mutex.lock();
  std::cout << "Thread " << i << " result: " << sum << '\n';
  cout_mutex.unlock();

  return sum;
}

int main(void) {
  cout_mutex.lock();
  std::cout << "Hello World\n";
  cout_mutex.unlock();

  auto s = calc(0);

  cout_mutex.lock();
  if (emscripten_has_threading_support()) {
    std::cout << "With thread\n";
  } else {
    std::cout << "Without thread\n";
  }
  cout_mutex.unlock();

  for(size_t i = 1; i <= 6; i++)
  {
    std::thread t1(calc, i);
    t1.detach();
  }
  
  return 0;
}