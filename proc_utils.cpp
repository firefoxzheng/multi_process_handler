#include "proc_utils.h"

std::string &trim(std::string &s) {
  if (s.empty()) {
    return s;
  }

  s.erase(0, s.find_first_not_of(" "));
  s.erase(s.find_last_not_of(" ") + 1);
  return s;
}

int ProcUtils::GetProcStatus(const pid_t pid, std::map<std::string, std::string> &proc_stat) {
  std::string status_file_path = "/proc/" + std::to_string(pid) + "/status";
  std::ifstream status_file(status_file_path);
  std::string line = "";
  while (getline(status_file, line) && !line.empty()) {
    std::string k = line.substr(0, line.find(":"));
    std::string v = line.substr(line.find(":") + 1);
    proc_stat[trim(k)] = trim(v);
  }

  return 0;
}

bool ProcUtils::IsProcAlive(const pid_t pid) {
  std::string file_path = "/proc/" + std::to_string(pid);
  if (access(file_path.c_str(), F_OK) != 0) {
    return false;
  }

  std::map<std::string, std::string> proc_stat;
  int ret = GetProcStatus(pid, proc_stat);
  if (ret) {
    return true;
  }

  std::string stat_desc = proc_stat["State"];
  if (stat_desc.find("stop") != std::string::npos ||
      stat_desc.find("zombie") != std::string::npos ||
      stat_desc.find("dead") != std::string::npos) {
    return false;
  }

  return true;
}
