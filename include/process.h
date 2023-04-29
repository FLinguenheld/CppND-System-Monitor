#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::string;

/*
 * Basic class for Process representation
 * It contains relevant attributes as shown below
*/
class Process {
    public:
        Process(string pid);

        int Pid() const;
        string User();
        string Command();
        string Ram() const;
        long int UpTime();

        float CpuUtilization();
        void calcul_cpu_first();
        void calcul_cpu_second();

        bool operator<(Process const& a) const;

    private:
        string _pid;
        string _path;

        void update_process_values(float &utime, float &stime);
        float _utime_0;
        float _utime_1;

        float _stime_0;
        float _stime_1;

        void update_proc_value(float &time_total);
        float _time_total_0;
        float _time_total_1;

        float _cpu_utilization;
};

#endif
