#include "multi_process_handler.h"

#include <unistd.h>

#include <iostream>

// running or sleeping ===> sleeping, waiting for parent to exit, do not restart
void doChildThings1() {
  int i = 0;
  while (i < 20) {
    std::cout << "this is child 1, pid:" << getpid() << std::endl;
    sleep(1);
    i++;
  }
}

// sleeping or running
void doChildThings2() {
  while (1) {
    std::cout << "this is child 2, pid:" << getpid() << std::endl;
    sleep(1);
  }
}

// zombie, waiting for parent to exit, restart by parent
void doChildThings3() {
  int i = 0;
  while (i < 20) {
    std::cout << "this is child 3, pid:" << getpid() << std::endl;
    sleep(1);
    i++;
  }

  exit(1);
}

int main() {

  std::queue<std::function<void()>> q;
  q.push(doChildThings1);
  q.push(doChildThings2);
  q.push(doChildThings3);

  MultiProcessHandler test_handler(q);

  int ret = test_handler.Run();
  if (ret) {
    std::cout << "Run failed, ret:" << ret << std::endl;
    return ret;
  }

  return 0;
}
