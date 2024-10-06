#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  std::string key;
  float value, memTotal = 0.0, memFree = 0.0;
  std::ifstream file("/proc/meminfo");
  if (file.is_open())
  {
    string line;
    while(std::getline(file,line))
      {
        std::stringstream linestream(line);
        linestream >> key >> value;
        if(key == "MemTotal:")
         {
            memTotal = value;
         }else if (key == "MemFree:")
         {
            memFree = value;
         }
         if (memTotal != 0.0 && memFree != 0.0)
         {
              break;
         }
  
       }
  }
  return (memTotal - memFree) / memTotal;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  std::ifstream file("/proc/uptime");
  string line;
  long uptime = 0;
  if(file.is_open())
  {
    std::getline(file,line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{ 
  std::string line, key;
  long value, jiffies = 0;

  std::ifstream file("/proc/stat");
  std::getline(file,line);
  std::istringstream linestream(line);
  linestream >> key;
  while(linestream >> value)
  {
    jiffies += value;
  }

  return jiffies;

}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  std::string line;
  long utime, stime, cutime, cstime, value;
  std::ifstream file("/proc/" + std::to_string(pid) + "/stat");
  if(file.is_open())
  {
    std::getline(file,line);
    std::stringstream linestream(line);
    std::vector<long> values;
    while(linestream >> value)
    {
      values.push_back(value);
    }
  

  if(values.size() < 22)
   {
     utime = values[13];
     stime = values[14];
     cutime = values[15];
     cstime = values[16];
     return utime + stime + cutime + cstime;
   }

  }

  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  std::string line, key;
  long active_jiffies = 0, value;
  std::vector<long> values;
  
  std::ifstream file("/proc/stat");
  if(file.is_open())
  {
    std::getline(file,line);
    std::istringstream linestream(line);
    linestream >> key;
    while(linestream >> value)
    {
      values.push_back(value);
    }
  }

  if(values.size() >= 8)
  {
    active_jiffies = values[0] + values[1] + values[2] + values[5] + values[6] + values[7];
  }
  return active_jiffies; 
   
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 

  
  std::string line, key;
  long ideal_jiffies = 0, value;
  std::vector<long> values;
  
  std::ifstream file("/proc/stat");
  if(file.is_open())
  {
    std::getline(file,line);
    std::istringstream linestream(line);
    linestream >> key;
    while(linestream >> value)
    {
      values.push_back(value);
    }
  }

  if(values.size() >= 5)
  {
    ideal_jiffies = values[3] + values[4];
  }
  return ideal_jiffies; 
   

 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

  std::string line,key;
  std::vector<string> cpu_values;
  string value;

  std::ifstream file("/proc/stat");
  if(file.is_open())
  {
    std::getline(file,line);
    std::istringstream linestream(line);
    linestream >> key;
    while(linestream >> value)
    {
      cpu_values.push_back(value);
    }
  }
  return cpu_values;
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

  std::string key, line;
  int total_processes = 0;
  std::ifstream file("/proc/stat");
  if(file.is_open())
  {
    while(std::getline(file,line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "processes")
      {
        linestream >> total_processes;
        break;
      }
    }
  }
  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { std::string key, line;
  int running_processes = 0;
  std::ifstream file("/proc/stat");
  if(file.is_open())
  {
    while(std::getline(file,line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "procs_running")
      {
        linestream >> running_processes;
        break;
      }
    }
  }
  return running_processes;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 

  std::string line;
  std::ifstream file("/proc/" + std::to_string(pid) + "/cmdline");

  if(file.is_open())
  {
    std::getline(file,line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::string line, key;
  std::ifstream file("/proc/" + std::to_string(pid) + "/status");

  if(file.is_open())
  {
    while(std::getline(file,line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "VmSize:")
      {
        linestream >> key;
        return key;
      }
    }
  }
  return std::string();
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::string line, key;
  std::ifstream file("/proc/" + std::to_string(pid) + "/status");

  if(file.is_open())
  {
    while(std::getline(file,line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "Uid:")
      {
        linestream >> key;
        return key;
      }
    }
  }
  return std::string();
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string uid = Uid(pid);
  std::string line, key, username, userid, x;
  std::ifstream file("/etc/passwd");

  if(file.is_open())
  {
    while(std::getline(file,line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> x >> userid;
      if(userid == uid)
      {
        return username;
      }
    }
  }
  return std::string();

}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::string line;
  long start_time = 0;
  std::ifstream file("/proc/" + std::to_string(pid) + "/stat");

  if(file.is_open())
  {
    std::getline(file, line);
    std::istringstream linestream(line);
    std::string value;
    std::vector<std::string> values;
    while(linestream >> value)
    {
      values.push_back(value);
    }

    if (values.size() >= 22)
    {
      start_time = std::stol(values[21]);
    }
    
  }

  long clock_ticks = sysconf(_SC_CLK_TCK);
  long system_uptime = LinuxParser::UpTime();
  long process_uptime = system_uptime - (start_time / clock_ticks);

  return process_uptime;
  
 }
