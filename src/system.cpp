#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <map>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::map;

// TODO: Return the system's CPU
Processor& System::Cpu() {
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> pids = LinuxParser::Pids();

    // Based on Lecture Video 4.14
    set<int> e_pids;
    for (Process const& process:processes_){
        e_pids.insert(process.Pid());
    } 

    // Based on Lecture Video 4.14
    for (int pid:pids){
        if (e_pids.find(pid) == e_pids.end()){
            processes_.emplace_back(pid);
        }     
    }
    
    // Based on Lecture Video 4.14
    for (auto& process : processes_){
        process.CpuUtilization(); // Update process's CPU utilization ?? Cause of segmetation fault
    }
    std::sort(processes_.begin(),processes_.end(),std::less<Process>()); 
    
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() const { 
    return LinuxParser::Kernel(); 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() const {     
    return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() const {
    return LinuxParser::OperatingSystem(); 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() const {
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() const { 
    return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() const { 
    return LinuxParser::UpTime(); 
}