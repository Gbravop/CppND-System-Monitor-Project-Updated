#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  vector<string> memv;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
     while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        memv.push_back(value);
      }
      if (memv.size() == 3) {
          break;
      }
    }
  }
  float memu = (stof(memv[0])-stof(memv[1]))/stod(memv[0]); 
  return memu; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string value1, value2;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream) {
        linestream >> value1 >> value2;
      }
    }
  }
  return stol(value1); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, value;
  vector<string> group;
  long jiffies;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        group.push_back(value);
      }
    }
  }
  jiffies = (stol(group[13])+stol(group[14])+stol(group[15])+stol(group[16]));
  return jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) + stol(jiffies[CPUStates::kSteal_]); 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]) ; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, value;
  vector<string> jiffies;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
     while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        jiffies.push_back(value);
      }
      if (jiffies[0] == "cpu") {
          break;
      }
    }
  }
  jiffies.erase(jiffies.begin());
  return jiffies; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value); 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    if (linestream) {
        linestream >> value;
      }
    }
  return value; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  int mem;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:"){
          mem = (static_cast<int>(0.5+(stol(value)/1024)));
        }
      }
    }
  }
  return to_string(mem); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:"){
          return value;
        }
      }
    }
  }
  return value;  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string line, username, x, y;
    string uid = Uid(pid);
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            while (linestream >> username >> x >> y){
                if(uid == y){
                    return username;
                }
            }   
        }
    }
    return username; 
}

vector<string> LinuxParser::CpuUtilization(int pid) { 
  string line, value;
  vector<string> group;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        group.push_back(value);
      }
    }
  }
  return group; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  vector<string> group = CpuUtilization(pid);
  long uptime = stol(group[21])/sysconf(_SC_CLK_TCK); 
  return uptime;
}
