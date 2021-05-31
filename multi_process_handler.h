#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <queue>

class MultiProcessHandler {
public:
  using ProcHandler = std::function<void()>;
  MultiProcessHandler(std::queue<ProcHandler> &q) : _proc_handlers(q) {}

  int Run();

private:
  int RecursiveFork(std::queue<ProcHandler> &q);
  int CheckSurvival();

private:
  std::queue<ProcHandler> _proc_handlers;
  std::map<pid_t, ProcHandler> _running_childs;
};
