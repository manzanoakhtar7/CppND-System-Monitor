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

Process::Process(int pid) : _pid(pid) {}

int Process::Pid() { return _pid; }

float Process::CpuUtilization() {
  float utilization;
  long activeJiffies = LinuxParser::ActiveJiffies(_pid);
  long totalJiffies = LinuxParser::Jiffies();
  if (_lastActiveJiffies == -1 && _lastTotalJiffies == -1) {
    utilization = activeJiffies * 1.0 / totalJiffies;
  } else {
    utilization = (activeJiffies - _lastActiveJiffies) * 1.0 /
                  (totalJiffies - _lastTotalJiffies);
  }
  _lastActiveJiffies = activeJiffies;
  _lastTotalJiffies = totalJiffies;
  _lastUtilization = utilization;
  return utilization;
}

string Process::Command() {
  if (_command.empty()) {
    _command = LinuxParser::Command(_pid);
  }
  return _command;
}

string Process::Ram() { return LinuxParser::Ram(_pid); }

string Process::User() { return LinuxParser::User(_pid); }

long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

// TODO
bool Process::operator<(Process const& a) const {
  return this->_lastUtilization < a._lastUtilization;
}