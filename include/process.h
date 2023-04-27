#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
    public:
        Process(std::string pid);
        int Pid() const;
        std::string User();
        std::string Command();
        float CpuUtilization();                  // TODO: See src/process.cpp
        std::string Ram() const;
        long int UpTime();
        bool operator<(Process const& a) const;  // TODO: See src/process.cpp

        // TODO: Declare any necessary private members
    private:
        std::string _pid;
        std::string _path;


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
