#include "processor.h"


float Processor::Utilization() const {
    return _cpu_utilization;
}

void Processor::calcul_first() {
    update_values(_total_1, _idle_1);
}
void Processor::calcul_second()
{
    update_values(_total_2, _idle_2);

    float totald = _total_2 - _total_1;
    float idled = _idle_2 - _idle_1;

    _cpu_utilization = (totald - idled) / totald;
}

void Processor::update_values(float &total, float &idle)
{
    auto vec = LinuxParser::parse(LinuxParser::kStatFilename, "^cpu", {1, 2, 3, 4, 5, 6, 7}, " ", "0");
    std::vector<float> values;

    for (auto v : vec)
        values.push_back(std::stof(v));

    // Calculations according to:
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    idle = values[3] + values[4];
    auto NonIdle = values[0] + values[1] + values[2] + values[5] + values[6] + values[7];
    total = idle + NonIdle;
}
