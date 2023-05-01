#ifndef SYSTEM_H
#define SYSTEM_H

#include "cpu.h"
#include "process.h"

#include <filesystem>
#include <algorithm>

using std::string;
using std::vector;


/*
 * Class to manage cpu, processes and get general os information.
 * Use <Update_cpu_and_processes> to refresh current cpu utilizations.
 * This method has to do a break (at least one second) to get two measures.
 */
class System {
    public:
        std::vector<Process>& Processes();
        CPU& Cpu();

        void Update_cpu_and_processes(int break_usec=1500000);

        float MemoryUtilization() const;
        long UpTime() const;
        int TotalProcesses() const;
        int RunningProcesses() const;

        std::string Kernel() const;
        std::string OperatingSystem() const;

    private:
        CPU _cpu;
        std::vector<Process> _processes = {};
};

#endif
