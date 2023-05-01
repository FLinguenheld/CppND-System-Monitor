#include "process_cpu.h"

using std::vector;


ProcessCPU::ProcessCPU(const string &pid_path) : AbstractCPU(), _pid_path(pid_path),
                                                 _utime_0(0.0), _stime_0(0.0), _time_total_0(0.0)
{}

void ProcessCPU::calcul_first() {
    update_process_values(_utime_0, _stime_0);
    update_proc_value(_time_total_0);
}
void ProcessCPU::calcul_second() {
    float utime_1, stime_1, time_total_1;

    update_process_values(utime_1, stime_1);
    update_proc_value(time_total_1);

    float user_util = sysconf(_SC_NPROCESSORS_ONLN) * (utime_1 - _utime_0) / (time_total_1 - _time_total_0);
    float sys_util = sysconf(_SC_NPROCESSORS_ONLN) * (stime_1 - _stime_0) / (time_total_1 - _time_total_0);

    _utilization = (user_util + sys_util) * 100;
}

void ProcessCPU::update_process_values(float &utime, float &stime) {
    vector<string> fields_process = LinuxParser::parse(_pid_path + LinuxParser::kStatFilename, vector<int>{13, 14},
                                                       " ", "0");

    utime = std::stof(fields_process[0]) / sysconf(_SC_CLK_TCK);
    stime = std::stof(fields_process[1]) / sysconf(_SC_CLK_TCK);
}
void ProcessCPU::update_proc_value(float &time_total) {
    vector<string> fields_cpu = LinuxParser::parse(_proc_path, vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}, " ", "0");

    time_total = 0;
    for (auto f : fields_cpu)
        time_total += std::stod(f);
}
