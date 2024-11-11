#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long HH = (seconds - (seconds%3600))/3600;
  long MM = ((seconds%3600)-((seconds%3600)%60))/60;
  long SS = (seconds%3600)%60;
  string outH = to_string(HH);
  string outM = to_string(MM); 
  string outS = to_string(SS);  
  return outH + ":" + outM + ":" + outS;
}