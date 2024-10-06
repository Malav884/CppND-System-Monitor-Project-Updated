#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

// headers included for Kernal Version
#include <fstream>
#include <sstream>

#include <filesystem>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"//..

#include <iostream>

using std::set;
using std::size_t;
using std::string;
using std::vector;

namespace fs = std::filesystem;
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes()
{ 
    
    processes_.clear();

    for ( const auto& entry : fs::directory_iterator("/proc"))
    {
        if(entry.is_directory())
      {
        const string dir_name = entry.path().filename().string();
      //  std::cout << "Directory: " << dir_name << std::endl; // Debug output
        if(std::all_of(dir_name.begin(), dir_name.end(), isdigit))
         {
            int pid = std::stoi(dir_name);
         //    std::cout << "PID: " << pid << std::endl; // Debug output
           try{ processes_.emplace_back(pid);}
           catch(const std::exception& e){
            std::cerr << "Error processing PID " << pid << ": " <<e.what() << std::endl;
            continue;
           }
           
         }
      }

    }  
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 

    std::string line_stored;
    std::ifstream input_stream{"/proc/version"};
    if (input_stream.is_open())
    {
        std::getline(input_stream, line_stored);
        std::istringstream line_stream(line_stored);
        
        /*so far we have stored the streamed line into a string 'line_stroed''
            now we will parse the string to get the kernel version using the >> exctraction
            for >> extraction, by default whitespace is the delimeter!
        */

       // we declare string variables to strore appropriate data
        std::string os, version, kernel;
        // line stream retrieved from /proc/version :- Linux version 6.8.0-44-generic (buildd@bos03-arm64-050) (aarch64-linux-gnu-gcc-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #44~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC Thu Aug 22 15:43:41 UTC 2
        line_stream >> os >> version >> kernel;

        return kernel;

    }
    
    return "";

    
    }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    
    std::string line_stored, key;
    float mem_total = 0.0;
    float mem_available = 0.0;
    std::ifstream input_stream("/proc/meminfo");
    if(input_stream.is_open())
    {
        while(std::getline(input_stream, line_stored))
        {
            std::istringstream line_stream(line_stored);
            line_stream >> key;
            if(key == "MemTotal:")
            {
                line_stream >> mem_total;

            }else if(key == "MemAvailable:")
            {
                line_stream >> mem_available;
            }

            if(mem_available != 0.0 && mem_total != 0.0)
            {
                break;
            }
        }
    }
    if(mem_total == 0.0)
    {
        return 0.0;
    }

    return (mem_total - mem_available) / mem_total;

}
// TODO: Return the operating system name
std::string System::OperatingSystem() {
    
    std::string line_stored, key, value;
    std::ifstream input_stream{"/etc/os-release"};
    if(input_stream.is_open()){
        while (std::getline(input_stream, line_stored))
        {
            std::istringstream line_stream(line_stored);
           // line_stream >> key;
            if (std::getline(line_stream, key , '=') && key == "PRETTY_NAME")
            {
               std::getline(line_stream >> std::ws, value);
               value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
                
                return value;
            }
            
        }
        
    }
    return ""; 
    }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() 
    {
        std::string line_stored, key;
        int running_processes = 0;
        std::ifstream input_stream("/proc/stat");
        if(input_stream.is_open())
        {
            while(std::getline(input_stream, line_stored))
          {
                std::istringstream line_stream(line_stored);
                line_stream >> key;

            if(key == "procs_running")
            {
                line_stream >> std::ws >> running_processes;
                return running_processes;

            }
          }

        }
        
    
        return 0; 
    }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    std::string line_stored, key;
    int processes = 0;
    std::ifstream input_stream{"/proc/stat"};
    if(input_stream.is_open())
         {
        
             while(std::getline(input_stream, line_stored))
             {
              std::istringstream line_stream(line_stored);
              line_stream >> key;
              if (key == "processes")
                {
                   line_stream >> std::ws >>processes;
                    return processes;
                }

             }
            

         }
    
    return 0; 
    }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() 
{ 
    std::string line_stored;
    double uptime = 0.0;
    std::ifstream input_stream("/proc/uptime");
    if(input_stream.is_open())
    {
        std::getline(input_stream, line_stored);
        std::istringstream line_stream(line_stored);
        line_stream >> uptime;
        
    }
    return static_cast<long int>(uptime);    
}