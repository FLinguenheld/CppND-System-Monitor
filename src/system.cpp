#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include <filesystem>
#include <algorithm>
// #include <cctype>


#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/


// Calculs for processes and cpu !
void System::Update_cpu_and_processes()
{

    cpu_.calcul_cpu_first();


    processes_.clear();
    auto folder = std::filesystem::path("/proc/");

    for (auto const& dir_entry : std::filesystem::directory_iterator{folder}) 
    {
        if ( dir_entry.is_directory() )
        {
            auto filename = dir_entry.path().filename().string();
            if (std::all_of(filename.begin(), filename.end(), isdigit)) {
                auto p = Process(filename);

                if (!p.Command().empty() && !p.Ram().empty())
                {
                    p.calcul_cpu_first(); // Cpu % has to be calculed twice after a break
                    processes_.push_back(p);
                }
            }
        }
    }

    // Break then finish calculating cpu utilization for all processes
    usleep(1200000);
    for (auto &p : processes_)
        p.calcul_cpu_second();

    std::sort(processes_.rbegin(), processes_.rend());

    cpu_.calcul_cpu_second();

}




// TODO: Return a container composed of the system's processes
// vector<Process>& System::Processes() { return processes_; }
vector<Process>& System::Processes() {
    return processes_;
}

Processor& System::Cpu() {
    return cpu_;
}

float System::MemoryUtilization() { 
    float total = std::stof(LinuxParser::parse(LinuxParser::kMeminfoFilename, "^MemTotal", 1, " ", "0"));
    float free = std::stof(LinuxParser::parse(LinuxParser::kMeminfoFilename, "^MemFree", 1, " ", "0"));

    return (total-free) / total;
}
long int System::UpTime() { 
    return std::stol(LinuxParser::parse(LinuxParser::kUptimeFilename, 0, " ", "0"));
}
int System::TotalProcesses() {
    return std::stoi(LinuxParser::parse(LinuxParser::kStatFilename, "^processes", 1, " ", "0"));
}
int System::RunningProcesses() {
    return std::stoi(LinuxParser::parse(LinuxParser::kStatFilename, "^procs_running", 1, " ", "0"));
}
std::string System::Kernel() {
    return LinuxParser::parse(LinuxParser::kVersionFilename, 2);
}
std::string System::OperatingSystem() {
    return LinuxParser::parse(LinuxParser::kOSPath, "^PRETTY_NAME", 1, "\"");
}
