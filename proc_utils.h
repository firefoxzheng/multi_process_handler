#pragma once

#include <unistd.h>

#include <fstream>
#include <map>

class ProcUtils {

public:
  static int GetProcStatus(const pid_t pid, std::map<std::string, std::string> &proc_stat);
  static bool IsProcAlive(const pid_t pid);
};
