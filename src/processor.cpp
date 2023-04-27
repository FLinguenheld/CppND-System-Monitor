#include "processor.h"


float Processor::Utilization()
{
    update_values(_total_1, _idle_1);
    usleep(50000);
    update_values(_total_2, _idle_2);

    float totald = _total_2 - _total_1;
    float idled = _idle_2 - _idle_1;

    return (totald - idled) / totald;
}

void Processor::update_values(float &total, float &idle)
{
    // Parse /proc/stat
    auto vec = LinuxParser::parse(LinuxParser::kStatFilename, "^cpu", {1, 2, 3, 4, 5, 6, 7}, " ", "0");
    std::vector<float> values;

    for (auto v : vec)
        values.push_back(std::stof(v));

    // Calculs according to:
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    idle = values[3] + values[4];
    auto NonIdle = values[0] + values[1] + values[2] + values[5] + values[6] + values[7];
    total = idle + NonIdle;
}
