#include "processor.h"
#include <fstream>
#include <sstream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::ifstream file("/proc/stat");
    if(!file.is_open()){
        throw std::runtime_error("Could not open /proc/stat");
    }

    std::string line;
    if(!std::getline(file,line)){
        throw std::runtime_error("Could not read /proc/stat");
    }
    
    std::istringstream ss(line);
    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    
    if(!(ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice)){
        throw std::runtime_error("Could not parse /proc/stat");
    }

   // ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

    long idle_time = idle + iowait;
    long non_idle_time = user + nice + system + irq + softirq + steal;
    long total_time = idle_time + non_idle_time;

    if(total_time == 0){
        throw std::runtime_error("Total time is zero");
    }
    
    
    return static_cast<float> (total_time - idle_time) / total_time;
     }