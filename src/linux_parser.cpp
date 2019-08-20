#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>
#include "log.h"

using std::stof;
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
    linestream >> os >>version >> kernel;
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
        if(pid>10000){
          pids.push_back(pid);

        }
        
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  string line;
  string key;
  string value;
  float MemTotal;
  float MemFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {

        if (key == "MemTotal:") {
          MemTotal=stoi(value);
        }

        if (key == "MemFree:") {
          MemFree=stoi(value);
        }

      }
    }
  }
  long MemoUsed = MemTotal - MemFree;
  return MemoUsed/MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {   
  long  uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >>idletime;
    
  }
  return uptime; 
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

long int LinuxParser::CpuUtilization(int pid) { 
string line;
  string mpid;
  string comm;
  string state;
  string ppid;
  string pgrp;
  string session;
  string tty_nr;
  string tpgid;
  string flags;
  string minflt;
  string cminflt;
  string majflt;
  string cmajflt;
  string utime;
  string stime;
  string cutime;
  string cstime;
  string priority;
  string nice;
  string num_threads;
  string itrealvalue;
  string starttime;
  string vsize;
  string rss;
  string rsslim;

  long cpu_utime;
  long cpu_stime;
  long cpu_starttime;
  long uptime = LinuxParser::UpTime();

  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> mpid >> comm >> state >> ppid >> pgrp >> session >>
             tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >>
             cmajflt >> utime >> stime >> cutime >> cstime >> priority >>
             nice >> num_threads >> itrealvalue >> starttime >> vsize >> rss >>
             rsslim) {
        cpu_utime = stod(utime);
        cpu_stime = stod(stime);
        cpu_starttime = stod(starttime);
      }
    }
  }

  long total_time = cpu_utime + cpu_stime;
  long seconds = uptime - (cpu_starttime / sysconf(_SC_CLK_TCK));
  long cpu_usage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
  return cpu_usage;
}

// TODO: Read and return the total number of processes
long int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  long int  value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string line;
  string key;
  long int  value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> value) {

        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string value;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        return value;
      }
    }
  }

  return "";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  int  MemTotal;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                           LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          MemTotal = stoi(value);
        }
      }
    }
  }

  return std::to_string(MemTotal / 1000);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
    string userid;
  {
    string line;
    string key;
    string value;
    std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +
                             LinuxParser::kStatusFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "Uid:") {
            userid = value;
          }
        }
      }
    }
  }
  return userid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 

  string userid=Uid(pid);
  string username;

  {
    string line;
    std::ifstream filestream("/etc/passwd");
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        string tmpusername;
        string tmp1;
        string tmpuserid;
        while (linestream >> tmpusername >> tmp1 >> tmpuserid) {
          if (tmpuserid == userid) {
            username = tmpusername;
            return username;
          }
        }
      }
    }
  }

  return "not found";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
   string line;
  string mpid;
  string comm;
  string state;
  string ppid;
  string pgrp;
  string session;
  string tty_nr;
  string tpgid;
  string flags;
  string minflt;
  string cminflt;
  string majflt;
  string cmajflt;
  string utime;
  string stime;
  string cutime;
  string cstime;
  string priority;
  string nice;
  string num_threads;
  string itrealvalue;
  string starttime;
  string vsize;
  string rss;
  string rsslim;
  float cpu_starttime;

  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) +LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if (linestream >> mpid >> comm >> state >> ppid >> pgrp >> session >>
             tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >>
             cmajflt >> utime >> stime >> cutime >> cstime >> priority >>
             nice >> num_threads >> itrealvalue >> starttime >> vsize >> rss >>
             rsslim) {
        cpu_starttime = stof(starttime);
      }
    }
  }
  return cpu_starttime/sysconf(_SC_CLK_TCK);
}