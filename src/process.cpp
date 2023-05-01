#include "process.h"

using std::to_string;


Process::Process(string pid) : _pid(pid), _path(LinuxParser::kProcDirectory + pid + "/"), _cpu(_path)
{};

int Process::Pid() const {
    return std::stoi(_pid);
}

string Process::User() const {
    auto uid = LinuxParser::parse(_path + LinuxParser::kStatusFilename, "^Uid:", 1);
    return LinuxParser::parse(LinuxParser::kPasswordPath , ":" + uid + ":", 0, ":");
}

string Process::Command() const {
    string cmd = LinuxParser::parse(_path + LinuxParser::kCmdlineFilename, 0, "");
    if (cmd.size() > 50)
    {
        cmd = cmd.substr(0, 50);
        cmd += " ...";
    }
    return cmd;
}

string Process::Ram() const {
    auto ram = LinuxParser::parse(_path + LinuxParser::kStatusFilename, "^VmSize:", 1, " ", "0");

    ram = to_string(std::stof(ram) * 0.001);
    return ram.substr(0, ram.find('.'));
}

long int Process::UpTime() const {
    auto field = std::stol(LinuxParser::parse(_path + LinuxParser::kStatFilename, 21, " ", "0"));
    auto process_start = field / sysconf(_SC_CLK_TCK);

    auto os_start = std::stol(LinuxParser::parse(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename,
                                                 0, " ", "0"));
    return os_start - process_start;
}

ProcessCPU& Process::Cpu() {
    return _cpu;
}

bool Process::operator<(Process const& a) const {
    return _cpu.Utilization() < a._cpu.Utilization();
}
