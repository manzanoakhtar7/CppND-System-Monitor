#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

using namespace std;

float Processor::Utilization() {
  float utilization;
  long activeJiffies = LinuxParser::ActiveJiffies();
  long totalJiffies = LinuxParser::Jiffies();
  if (_lastActiveJiffies == -1 && _lastTotalJiffies == -1) {
    utilization = activeJiffies * 1.0 / totalJiffies;
  } else {
    utilization = (activeJiffies - _lastActiveJiffies) * 1.0 /
                  (totalJiffies - _lastTotalJiffies);
  }
  _lastActiveJiffies = activeJiffies;
  _lastTotalJiffies = totalJiffies;
  return utilization;
}