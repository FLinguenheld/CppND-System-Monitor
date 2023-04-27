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
{
};

int Process::Pid() const {
    return std::stoi(_pid);
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {

    // try {
    //     std::vector<std::string> fields = LinuxParser::parse(_path + "/stat",
    //                                                          std::vector<int>{13, 14, 15, 16, 21});

    //     auto uptime = std::stof(LinuxParser::parse("/proc/uptime", 0));

    //     auto utime = std::stof(fields[0]);
    //     auto stime  = std::stof(fields[1]);
    //     auto cutime = std::stof(fields[2]);
    //     auto cstime = std::stof(fields[3]);
    //     auto starttime = std::stof(fields[4]);

    //     auto Hertz = sysconf(_SC_CLK_TCK);

    //     auto total_time = utime + stime;
    //     total_time = total_time + cutime + cstime;
    //     auto seconds = uptime - (starttime / Hertz);
    //     auto cpu_usage = (total_time / Hertz) / seconds;

    //     return cpu_usage;
    // }
    // catch (const std::bad_alloc &ia) {
    //     return 0.0;
    // }
    // catch (const std::invalid_argument &ia) {
    //     return 0.0;
    // }

    update_process_values(_utime_0, _stime_0);
    update_proc_value(_time_total_0);
    usleep(50000);
    update_process_values(_utime_1, _stime_1);
    update_proc_value(_time_total_1);

    auto user_util = sysconf(_SC_NPROCESSORS_ONLN) * (_utime_1 - _utime_0) / (_time_total_1 - _time_total_0);
    auto sys_util = sysconf(_SC_NPROCESSORS_ONLN) * (_stime_1 - _stime_0) / (_time_total_1 - _time_total_0);

    _cpu_utilization = user_util + sys_util;
    return _cpu_utilization;
}


void Process::update_process_values(float &utime, float &stime)
{
    std::vector<std::string> fields_process = LinuxParser::parse(_path + "/stat", std::vector<int>{13, 14},
                                                                 " ", "0");
    utime = std::stof(fields_process[0]);
    stime = std::stof(fields_process[1]);
}

void Process::update_proc_value(float &time_total)
{
    std::vector<std::string> fields_cpu = LinuxParser::parse("/proc/stat",
                                                             std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9},
                                                             " ", "0");
    time_total = 0;
    for (auto f : fields_cpu)
        time_total += std::stof(f);
}











string Process::Command() {
    return LinuxParser::parse(_path + "/cmdline", 0);
}

string Process::Ram() const {
    auto ram = LinuxParser::parse(_path + "/status", "^VmSize:", 1, " ", "0");

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
    auto field = std::stol(LinuxParser::parse(_path + "/stat", 21, " ", "0"));
    auto process_start = field / sysconf(_SC_CLK_TCK);

    auto os_start = std::stol(LinuxParser::parse(LinuxParser::kUptimeFilename, 0, " ", "0"));

    return os_start - process_start;
}

bool Process::operator<(Process const& a) const {
    // return Pid() < a.Pid();
    return _cpu_utilization > a._cpu_utilization;
}
