#include "system.h"


vector<Process>& System::Processes() {
    return _processes;
}
CPU& System::Cpu() {
    return _cpu;
}

// --
void System::Update_cpu_and_processes(int break_usec)
{
    _processes.clear();
    auto folder = std::filesystem::path(LinuxParser::kProcDirectory);

    for (auto const& dir_entry : std::filesystem::directory_iterator{folder}) 
    {
        if ( dir_entry.is_directory() )
        {
            auto filename = dir_entry.path().filename().string();
            if (std::all_of(filename.begin(), filename.end(), isdigit)) {
                auto new_process = Process(filename);

                if (!new_process.Command().empty())
                {
                    new_process.Cpu().calcul_first();
                    _processes.push_back(new_process);
                }
            }
        }
    }

    _cpu.calcul_first();

    // Break then finish calculating cpu utilizations
    usleep(break_usec);

    for (auto &p : _processes)
        p.Cpu().calcul_second();

    std::sort(_processes.rbegin(), _processes.rend());

    _cpu.calcul_second();
}

// --
float System::MemoryUtilization() const { 
    float total = std::stof(LinuxParser::parse(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename,
                                               "^MemTotal", 1, " ", "0.0"));
    float free = std::stof(LinuxParser::parse(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename,
                                              "^MemFree", 1, " ", "0.0"));
    return (total-free) / total;
}
long int System::UpTime() const { 
    return std::stol(LinuxParser::parse(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename, 0, " ", "0"));
}
int System::TotalProcesses() const {
    return std::stoi(LinuxParser::parse(LinuxParser::kProcDirectory + LinuxParser::kStatFilename,
                                        "^processes", 1, " ", "0"));
}
int System::RunningProcesses() const {
    return std::stoi(LinuxParser::parse(LinuxParser::kProcDirectory + LinuxParser::kStatFilename,
                                        "^procs_running", 1, " ", "0"));
}

std::string System::Kernel() const {
    return LinuxParser::parse(LinuxParser::kProcDirectory + LinuxParser::kVersionFilename, 2);
}
std::string System::OperatingSystem() const {
    return LinuxParser::parse(LinuxParser::kOSPath, "^PRETTY_NAME", 1, "\"");
}
