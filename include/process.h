#ifndef PROCESS_H
#define PROCESS_H

#include "process_cpu.h"

using std::string;


/*
 * Basic class for Process representation
 * It contains relevant attributes as shown below
*/
class Process {
    public:
        Process(string pid);

        int Pid() const;
        string User() const;
        string Command() const;
        string Ram() const;
        long int UpTime() const;

        ProcessCPU& Cpu();

        bool operator<(Process const& a) const;

    private:
        string _pid;
        string _path;

        ProcessCPU _cpu;
};

#endif
