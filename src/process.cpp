#include <unistd.h>
#include <cctype>
#include <iomanip>
#include <ctime>

#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(std::string pid) : _pid(pid), _path("/proc/" + pid)
{};

int Process::Pid() const {
    return std::stoi(_pid);
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {

    try {
        std::vector<std::string> fields = LinuxParser::parse(_path + "/stat",
                                                             std::vector<int>{13, 14, 15, 16, 21});

        auto uptime = std::stof(LinuxParser::parse("/proc/uptime", 0));

        auto utime = std::stof(fields[0]);
        auto stime  = std::stof(fields[1]);
        auto cutime = std::stof(fields[2]);
        auto cstime = std::stof(fields[3]);
        auto starttime = std::stof(fields[4]);

        auto Hertz = sysconf(_SC_CLK_TCK);

        auto total_time = utime + stime;
        total_time = total_time + cutime + cstime;
        auto seconds = uptime - (starttime / Hertz);
        auto cpu_usage = (total_time / Hertz) / seconds;

        return cpu_usage;
    }
    catch (const std::bad_alloc &ia) {
        return 0.0;
    }
    catch (const std::invalid_argument &ia) {
        return 0.0;
    }
}

string Process::Command() {
    return LinuxParser::parse(_path + "/cmdline", 0);
}

string Process::Ram() const {
    auto ram = LinuxParser::parse(_path + "/status", "^VmSize:", 1);

    if ( ram.length() ){
        ram = to_string(std::stoi(ram) * 0.001);
        return ram.substr(0, ram.find('.'));
    }

    return string();
}

string Process::User() {
    auto uid = LinuxParser::parse(_path + "/status", "^Uid:", 1);
    return LinuxParser::parse("/etc/passwd", ":" + uid + ":", 0, ":");
}

long int Process::UpTime() {
    // auto field = std::stol(LinuxParser::parse(_path + "/stat", 21));
    auto field = std::stol(LinuxParser::parse(_path + "/stat", 13));
    auto sec = field / sysconf(_SC_CLK_TCK);

    return sec;
}

bool Process::operator<(Process const& a) const {
    return CpuUtilization() < a.CpuUtilization();
}
