#include <string>
#include <iostream>
#include  <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours;
  int minutes;
  int sec;

  hours = seconds/3600;
  minutes = (seconds % 3600) /60;
  sec = (seconds % 3600) % 60 ;

  std::ostringstream oss;
  oss << std::setfill('0')
      << std::setw(2) <<  hours<< ':' << std::setw(2) << minutes << ':' << std::setw(2) << sec;
  return oss.str();

}