#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line, key, value;
  float memTotal, memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = stof(value);
        } else if (key == "MemFree:") {
          memFree = stof(value);
        }
      }
    }
  }
  return (memTotal - memFree) / memTotal;
}

long LinuxParser::UpTime() {
  string line, upTime, idleTime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> upTime) {
        return stol(upTime);
      }
    }
  }
  return 0;
}

long LinuxParser::Jiffies() {
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal,
      guest, guest_nice;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> cpu >> user >> nice >> system >> idle >> iowait >>
          irq >> softirq >> steal >> guest >> guest_nice) {
        return stol(cpu) + stol(user) + stol(nice) + stol(system) + stol(idle) +
               stol(iowait) + stol(irq) + stol(softirq) + stol(steal) +
               stol(guest) + stol(guest_nice);
      }
    }
  }
  return 0;
}

long LinuxParser::ActiveJiffies(int pid) {
  string line, proc_id, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags,
      minflt, cminflt, majflt, cmajflt, utime, stime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> proc_id >> comm >> state >> ppid >> pgrp >> session >>
          tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >>
          utime >> stime) {
        return stol(utime) + stol(stime);
      }
    }
  }
  return 0;
}

long LinuxParser::ActiveJiffies() {
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal,
      guest, guest_nice;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> cpu >> user >> nice >> system >> idle >> iowait >>
          irq >> softirq >> steal >> guest >> guest_nice) {
        return stol(cpu) + stol(user) + stol(nice) + stol(system) + stol(irq) +
               stol(softirq) + stol(steal) + stol(guest) + stol(guest_nice);
      }
    }
  }
  return 0;
}

long LinuxParser::IdleJiffies() {
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal,
      guest, guest_nice;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> cpu >> user >> nice >> system >> idle >> iowait >>
          irq >> softirq >> steal >> guest >> guest_nice) {
        return stol(idle) + stol(iowait);
      }
    }
  }
  return 0;
}

vector<string> LinuxParser::CpuUtilization() {
  return {to_string(ActiveJiffies()), to_string(IdleJiffies()),
          to_string(Jiffies())};
}

int LinuxParser::TotalProcesses() {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  string line, cmd;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> cmd) {
        return cmd;
      }
    }
  }
  return 0;
}

string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return to_string(stof(value) / 1000);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return 0;
}

string LinuxParser::User(int pid) {
  string line, name, pass, test_uid;
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> pass >> test_uid) {
        if (test_uid == uid) {
          return name;
        }
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid) {
  string line, proc_id, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags,
      minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime, priority,
      nice, num_threads, itrealvalue, starttime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> proc_id >> comm >> state >> ppid >> pgrp >> session >>
          tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >>
          utime >> stime >> cutime >> cstime >> priority >> nice >>
          num_threads >> itrealvalue >> starttime) {
        return UpTime() - (stol(starttime)/sysconf(_SC_CLK_TCK));
      }
    }
  }
  return 0;
}
