#ifndef PROCESS_CPU_H
#define PROCESS_CPU_H

#include <string>
#include <unistd.h>

#include "cpu.h"

using std::string;


/*
 * Class for CPU Process representation
 * Use methods <calcul_first> then <calcul_second> after one second break to set the current utilization.
*/
class ProcessCPU : public AbstractCPU {

    public:
        ProcessCPU(const string &pid_path);

        void calcul_first() override;
        void calcul_second() override;

    private:
        string _pid_path;

        void update_process_values(float &utime, float &stime);
        void update_proc_value(float &time_total);
        float _utime_0;
        float _stime_0;
        float _time_total_0;
};

#endif
