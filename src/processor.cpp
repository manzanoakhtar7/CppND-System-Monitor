#include "processor.h"

#include <vector>
#include <string>

#include "linux_parser.h"

using namespace std;

float Processor::Utilization() {
  float utilization;
  long activeJiffies = LinuxParser::ActiveJiffies();
  long totalJiffies = LinuxParser::Jiffies();
  if (_lastActiveJiffies == -1 && _lastTotalJiffies == -1) {
    utilization = activeJiffies / totalJiffies;
  } else {
    utilization = (activeJiffies - _lastActiveJiffies) /
                  (totalJiffies - _lastTotalJiffies);
  }
  _lastActiveJiffies = activeJiffies;
  _lastTotalJiffies = totalJiffies;
  return utilization;
}