#include "processor.h"


Processor::Processor() : _total_0(0.0), _idle_0(0.0), _cpu_utilization(0.0)
{}

float Processor::Utilization() const {
    return _cpu_utilization;
}

void Processor::calcul_first() {
    update_values(_total_0, _idle_0);
}
void Processor::calcul_second()
{
    float total_1, idle_1;
    update_values(total_1, idle_1);

    float totald = total_1 - _total_0;
    float idled = idle_1 - _idle_0;

    _cpu_utilization = (totald - idled) / totald;
}

void Processor::update_values(float &total, float &idle)
{
    auto vec = LinuxParser::parse(LinuxParser::kProcDirectory + LinuxParser::kStatFilename,
                                  "^cpu", {1, 2, 3, 4, 5, 6, 7}, " ", "0");
    std::vector<float> values;

    for (auto v : vec)
        values.push_back(std::stof(v));

    // Calculations according to:
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    idle = values[3] + values[4];
    auto NonIdle = values[0] + values[1] + values[2] + values[5] + values[6] + values[7];
    total = idle + NonIdle;
}
