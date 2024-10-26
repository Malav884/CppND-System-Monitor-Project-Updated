#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <pwd.h>

#include "process.h"

#include <iostream>

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

Process::Process() : pid_(0) {}
Process::Process(int pid) : pid_(pid) {}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const{ 
    std::ifstream file("/proc/" + std::to_string(pid_) + "/stat");
    if(!file.is_open()){
        return 0.0;
    }
    
    
        std::string line;
        std::getline(file,line);
        std::stringstream linestream(line);

        std::vector<long> values;
        std::string value;
        while (linestream>>value)
        {
            try{
            values.push_back(std::stol(value));
            }catch(const std::invalid_argument& e){
                std::cerr << "Invalid argument: " << value <<std::endl; // Debug output
                return 0.0;
            }catch(const std::out_of_range& e){
                std::cerr << "Out of range: " << value <<std::endl; // Debug output
                return 0.0;
            }
        }



        // Fields 14, 15, 16, and 17 are utime, stime, cutime, and cstime respectively
        long uptime = values[13];
        long stime = values[14];
        long cutime = values[15];
        long cstime = values[16];

        long total_time = uptime + stime + cutime + cstime;

        //Get system uptime

        std::ifstream uptime_file("/proc/uptime");
        string uptime_line;
        std::getline (uptime_file, uptime_line);
        std::stringstream uptime_stream(uptime_line);
        long streamed_uptime;
        uptime_stream >> streamed_uptime;

        //get process start time (field 22 in prc/<pid>/stat)
        long starttime = values[21];

        //calculate process uptime

        long herts = sysconf(_SC_CLK_TCK);
        float process_uptime = streamed_uptime - (starttime / herts);

        //calculate process CPU utilization
        float cpu_utilization = ((total_time / herts) / process_uptime); 

    

    return  cpu_utilization;
 }

// TODO: Return the command that generated this process
string Process::Command() { 
    std::ifstream file("/proc" + std::to_string(pid_) + "/cmdline");
    std::string cmdline;
    if(file.is_open())
    {
         std::getline(file,cmdline, '\0');
         if(cmdline.empty())
            {
                cmdline = "N/A";
            }
    }else
    {
        cmdline = "N/A";
    }


    return cmdline; }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    std::ifstream file("/proc" + std::to_string(pid_) + "/status");
    std::string line;
    std::string key;
    std::string value;
    while (std::getline(file, line))
    {
        std::istringstream linestream(line);
        linestream >> key >> value;
        if (key == "VmSize:") // Return the memory size in KB
        {
            return value;
        }
        else if (key == "VmRSS:")
        {
            return value;
        }
    }

    

    return string(); } // return empty string if no value found

// TODO: Return the user (name) that generated this process
string Process::User() { 
    std::string key, line;
    int uid;
    std::ifstream file("/proc" + std::to_string(pid_) + "/status");
    if(file.is_open()){
        while(std::getline(file,line)){
            std::stringstream linestream(line);
            linestream >> key;
            if(key == "Uid:")
            {
                linestream >> uid;
                break;
            }
        }
    }

    struct passwd *pw = getpwuid(uid);
    if(pw != nullptr){
        return pw->pw_name;
    }

    return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    std::string line;
    long int startTime = 0;
    std::ifstream file("/proc" + std::to_string(pid_) + "/stat");

    if(file.is_open()){
        std::getline(file,line);
        std::istringstream linestream(line);
        std::string value;
        for (int i = 0; i < 22; i++)
        {
            linestream >> value;
        }
        startTime = std::stol(value);
    }

    long int systemUptime = 0;
    std::ifstream uptime_file("/proc/uptime");
    if(uptime_file.is_open()){
        uptime_file >> systemUptime;
    }

    return systemUptime - (startTime / sysconf(_SC_CLK_TCK)); }

 

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ return this->CpuUtilization() < a.CpuUtilization(); }