#include <string>
#include <iomanip>

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
  //outH.insert(0, 2-outH.length(),'0'); // Based on feedback/revision #1 // Produced instance of 'std::length_error'
  string outM = to_string(MM);
  string outS = to_string(SS); 
  
  std::ostringstream stream;

  stream << std::setw(2) << std::setfill('0') << outH << ":"  // Based on forum question 155686
         << std::setw(2) << std::setfill('0') << outM << ":"
         << std::setw(2) << std::setfill('0') << outS;
  
  return stream.str();
}