#include "processor.h"

Processor::Processor()
{};

Processor::Processor(int user, int nice, int system, int idle, int iowait, int irq,
                    int softirq, int steal, int guest, int guest_nice) :
                        _user(user), _nice(nice), _system(system), _idle(idle), _iowait(iowait), _irq(irq),
                        _softirq(softirq), _steal(steal), _guest(guest), _guest_nice(guest_nice)
{};

float Processor::Utilization() {

    // Guest time is already accounted in usertime
    _user = _user - _guest;
    _nice = _nice - _guest;
    // Fields existing on kernels >= 2.6
    // (and RHEL's patched kernel 2.4...)
    unsigned long long int idlealltime = _idle + _iowait;
    unsigned long long int systemalltime = _system + _irq + _softirq;
    unsigned long long int virtalltime = _guest + _guest_nice;
    unsigned long long int totaltime = _user + _nice + systemalltime + idlealltime + _steal + virtalltime;

    return (totaltime - _idle) / totaltime;

}
