#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <unistd.h>
#include "linux_parser.h"


class Processor {
    public:
        /* Parse and calcul the system utilization twice to return the current cpu usage percentage */
        float Utilization();

    private:

        /* Parse '/proc/stat' and calcul total & idle. */
        void update_values(float &total, float &idle);

        float _total_1;
        float _idle_1;

        float _total_2;
        float _idle_2;
};

#endif
