#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include "linux_parser.h"


/*
 * Class for the CPU, it allows to calculate the current utilization.
 * Use methods <calcul_first> then <calcul_second> after one second break to set the cpu utilization.
 */
class Processor {
    public:
        Processor();
        float Utilization() const;
        void calcul_first();
        void calcul_second();

    private:
        void update_values(float &total, float &idle);
        float _total_0;
        float _idle_0;
        float _cpu_utilization;
};

#endif
