#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  float utilization;
  if ((UpTime() - lastUpdated_) > 0) {
    long activeJiffies = LinuxParser::ActiveJiffies(pid_);
    long totalJiffies = LinuxParser::Jiffies();
    if (lastActiveJiffies_ == -1 && lastTotalJiffies_ == -1) {
      utilization = activeJiffies * 1.0 / totalJiffies;
    } else {
      utilization = (activeJiffies - lastActiveJiffies_) * 1.0 /
                    (totalJiffies - lastTotalJiffies_);
    }
    lastActiveJiffies_ = activeJiffies;
    lastTotalJiffies_ = totalJiffies;
    lastUtilization_ = utilization;
  } else {
    utilization = lastUtilization_;
  }
  lastUpdated_ = UpTime();
  return utilization;
}

string Process::Command() {
  if (command_.empty()) {
    command_ = LinuxParser::Command(pid_);
  }
  return command_;
}

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
  return this->lastUtilization_ < a.lastUtilization_;
}