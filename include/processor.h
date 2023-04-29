#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include "linux_parser.h"


class Processor {
    public:
        /* Parse and calcul the system utilization twice to return the current cpu usage percentage */
        float Utilization();

        void calcul_cpu_first();
        void calcul_cpu_second();

    private:

        /* Parse '/proc/stat' and calcul total & idle. */
        void update_values(float &total, float &idle);

        float _total_1;
        float _idle_1;

        float _total_2;
        float _idle_2;

        float _cpu_utilization;
};

#endif
