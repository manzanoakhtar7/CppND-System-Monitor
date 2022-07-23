#include "format.h"

#include <cmath>
#include <cstdio>
#include <string>

using namespace std;

string Format::ElapsedTime(long seconds) {
  long hours, minutes;
  ldiv_t min_div = div(seconds, (long)60);
  seconds = min_div.rem;
  minutes = min_div.quot;
  ldiv_t hour_div = div(minutes, (long)60);
  minutes = hour_div.rem;
  hours = hour_div.quot;
  char formatted_time[64];
  sprintf(formatted_time, "%02ld:%02ld:%02ld", hours, minutes, seconds);
  return formatted_time;
}