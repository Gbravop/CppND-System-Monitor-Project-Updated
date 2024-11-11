#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const { 
    return pid_; 
}

// float Process::CpuUtilization() const { 
//     return 0.0;     
// }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    // int pid = Pid();
    // System Time: (secs)
    long utime = LinuxParser::UpTime();
    // Process's Times: (clock ticks)
    vector<string> ptimes = LinuxParser::CpuUtilization(pid_);
    long putime = stol(ptimes[13]);
    long pstime = stol(ptimes[14]);
    long pcutime = stol(ptimes[15]);
    long pcstime = stol(ptimes[16]);
    long pstartime = stol(ptimes[21]);
    long total_time = putime + pstime + pcutime + pcstime;
    // Time since process started:
    long time_secs = abs(utime - (pstartime/sysconf(_SC_CLK_TCK)));
    // CPU usage percent:
    if (float(time_secs) == 0.0){
        return 0.0;
    } else {
        return float(total_time/sysconf(_SC_CLK_TCK))/float(time_secs);
    }
}

// TODO: Return the command that generated this process
string Process::Command() { 
    //int pid = Pid();
    return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    //int pid = Pid();
    return LinuxParser::Ram(pid_); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    //int pid = Pid();
    return LinuxParser::User(pid_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    // int pid = Pid();
    return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); //Based on feedback/revision #3
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    bool ans = (a.CpuUtilization()<CpuUtilization());    
    return ans; 
}

bool Process::operator>(Process const& a) const { 
    bool ans = (a.CpuUtilization()>CpuUtilization());    
    return ans; 
}