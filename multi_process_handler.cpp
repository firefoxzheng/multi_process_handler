#include "multi_process_handler.h"
#include "proc_utils.h"

int MultiProcessHandler::Run() {
  int ret = RecursiveFork(_proc_handlers);
  if (ret) {
    std::cout << "RecursiveFork failed, ret:" << ret << std::endl;
    return ret;
  }

  ret = CheckSurvival();
  if (ret) {
    std::cout << "CheckSurvival failed, ret:" << ret << std::endl;
    return ret;
  }

  return 0;
}

int MultiProcessHandler::RecursiveFork(std::queue<ProcHandler> &q) {
  if (q.empty()) {
    return 0;
  }

  pid_t pid = fork();

  if (pid == 0) {
    q.front()();
  } else if (pid > 0) {
    _running_childs[pid] = q.front();

    q.pop();
    RecursiveFork(q);
  } else {
    printf("fork error, err:%d\n", errno);
    return -1;
  }

  return 0;
}

int MultiProcessHandler::CheckSurvival() {
  while (1) {
    std::queue<ProcHandler> dead_childs;
    for (auto &c : _running_childs) {
      // 进程不存在, 需要重新拉起
      if (!ProcUtils::IsProcAlive(c.first)) {
        std::cout << "pid:" << c.first << "is dead, restart it!" << std::endl;
        dead_childs.push(c.second);
        _running_childs.erase(c.first);
      }
    }

    RecursiveFork(dead_childs);

    sleep(5);
  }

  return 0;
}
