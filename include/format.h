#ifndef FORMAT_H
#define FORMAT_H

#include <string>


namespace Format {

    /* Convert <times> to string with format HH:MM:SS */
    std::string ElapsedTime(long times);
    std::string to_string(int val);

};  // namespace Format

#endif
