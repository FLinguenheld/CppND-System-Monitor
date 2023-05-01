#include "process.h"


using std::to_string;
using std::vector;

ProcessCPU::ProcessCPU(const string &path) : _path(path)
{}





Process::Process(string pid) : _pid(pid), _path(LinuxParser::kProcDirectory + pid + "/"), _cpu(_path),
                               _utime_0(0.0), _stime_0(0.0), _time_total_0(0.0),
                               _cpu_utilization(0.0)
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

// CPU ----
float Process::CpuUtilization() const {
    return _cpu_utilization;
}
void Process::calcul_cpu_first() {
    update_process_values(_utime_0, _stime_0);
    update_proc_value(_time_total_0);
}
void Process::calcul_cpu_second() {
    float utime_1, stime_1, time_total_1;

    update_process_values(utime_1, stime_1);
    update_proc_value(time_total_1);

    float user_util = sysconf(_SC_NPROCESSORS_ONLN) * (utime_1 - _utime_0) / (time_total_1 - _time_total_0);
    float sys_util = sysconf(_SC_NPROCESSORS_ONLN) * (stime_1 - _stime_0) / (time_total_1 - _time_total_0);

    _cpu_utilization = (user_util + sys_util) * 100;
}

void Process::update_process_values(float &utime, float &stime) {
    vector<string> fields_process = LinuxParser::parse(_path + LinuxParser::kStatFilename, vector<int>{13, 14},
                                                       " ", "0");

    utime = std::stof(fields_process[0]) / sysconf(_SC_CLK_TCK);
    stime = std::stof(fields_process[1]) / sysconf(_SC_CLK_TCK);
}
void Process::update_proc_value(float &time_total) {
    vector<string> fields_cpu = LinuxParser::parse(LinuxParser::kProcDirectory + LinuxParser::kStatFilename,
                                                   vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}, " ", "0");
    time_total = 0;
    for (auto f : fields_cpu)
        time_total += std::stod(f);
}

bool Process::operator<(Process const& a) const {
    return _cpu_utilization < a._cpu_utilization;
}
