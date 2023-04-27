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
        float CpuUtilization() const;                  // TODO: See src/process.cpp
        std::string Ram() const;
        long int UpTime();
        bool operator<(Process const& a) const;  // TODO: See src/process.cpp

        // TODO: Declare any necessary private members
    private:
        std::string _pid;
        std::string _path;
};

#endif
