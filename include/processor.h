#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:

    Processor();
    Processor(int user, int nice, int system, int idle, int iowait, int irq,
                        int softirq, int steal, int guest, int guest_nice);

  float Utilization();

  // TODO: Declare any necessary private members
 private:
    int _user;
    int _nice;
    int _system;
    int _idle;
    int _iowait;
    int _irq;
    int _softirq;
    int _steal;
    int _guest;
    int _guest_nice;
};

#endif
