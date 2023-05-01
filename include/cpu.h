#ifndef CPU_H
#define CPU_H

#include <vector>
#include "linux_parser.h"


class AbstractCPU
{
    public:
        AbstractCPU();
        float Utilization() const;
        virtual void calcul_first() = 0;
        virtual void calcul_second() = 0;

    protected:
        float _utilization;
};


/*
 * Class for the CPU, it allows to calculate the current utilization.
 * Use methods <calcul_first> then <calcul_second> after one second break to set the cpu utilization.
 */
class CPU : public AbstractCPU {
    public:
        CPU();
        void calcul_first() override;
        void calcul_second() override;

    private:
        void update_values(float &total, float &idle);
        float _total_0;
        float _idle_0;
};

#endif
