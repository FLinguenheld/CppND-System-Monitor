#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

Processor& System::Cpu() {
    return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }


float System::MemoryUtilization() { 
    float total = std::stol(LinuxParser::parse(LinuxParser::kMeminfoFilename, "MemTotal", 1));
    float free = std::stol(LinuxParser::parse(LinuxParser::kMeminfoFilename, "MemFree", 1));

    return (total-free) / total;
}
long int System::UpTime() { 
    return std::stol(LinuxParser::parse(LinuxParser::kUptimeFilename, 0));
}
int System::TotalProcesses() {
    return std::stoi(LinuxParser::parse(LinuxParser::kStatFilename, "processes", 1));
}
int System::RunningProcesses() {
    return std::stoi(LinuxParser::parse(LinuxParser::kStatFilename, "procs_running", 1));
}
std::string System::Kernel() {
    return LinuxParser::parse(LinuxParser::kVersionFilename, 2);
}
std::string System::OperatingSystem() {
    return LinuxParser::parse(LinuxParser::kOSPath, "PRETTY_NAME", 1, "\"");
}
