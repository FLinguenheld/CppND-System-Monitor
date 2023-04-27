#include "format.h"


std::string Format::ElapsedTime(long seconds) {

    int minutes = seconds / 60;
    int hours = minutes / 60;
    return to_string(hours) + ":" + to_string(minutes%60) + ":" + to_string(seconds%60);
}

std::string Format::to_string(int val)
{
    std::string txt = std::to_string(val);
    if (txt.length() == 1)
        txt = "0" + txt;

    return txt;
}
