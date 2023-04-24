#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


std::string LinuxParser::cut_line(std::string line, int field, std::string delimiter)
{
    size_t s(0);
    auto delim_field = std::vector<size_t>{0};

    // Remove mutliple spaces if the delimiter is a space
    if (delimiter == " ") {
        std::string line_cleaned;
        for (auto c : line) {
            if (c != ' ' || line_cleaned.back() != ' ')
                line_cleaned.push_back(c);
        }
        line = line_cleaned;
    }

    // Get all delimiter positions
    while (s != std::string::npos && delim_field.size() < field + 2) {
        s = line.find(delimiter, s+1);
        delim_field.push_back(s);
    }

    if (field >= delim_field.size()) {
        return std::string();
    } else {
        auto start = field == 0 ? 0 : delim_field[field] + delimiter.length();
        auto length = field == 0 ? delim_field[field+1] : delim_field[field+1] - delim_field[field] - 1;
        return line.substr(start, length);
    }
}

std::string LinuxParser::parse(Path p, int position, std::string separator)
{
    std::ifstream ifs(path(p));

    std::string line;
    std::getline(ifs, line);

    return cut_line(line, position, separator);
}

std::string LinuxParser::parse(Path p, const std::string &grep, int position, std::string separator)
{
    std::ifstream ifs(path(p));

    std::string line;
    while (std::getline(ifs, line)) {

        if ( line.find(grep) == 0 )
            return cut_line(line, position, separator);
    }

    return std::string();
}


std::string LinuxParser::path(LinuxParser::Path p) const
{
    switch (p) {
        // case Path::kProcDirectory : return "/proc/";
        //     break;
        case Path::kCmdlineFilename : return "/proc/cmdline";
            break;
        case Path::kCpuinfoFilename : return "/proc/cpuinfo";
            break;
        case Path::kStatusFilename : return "";
            break;
        case Path::kStatFilename : return "/proc/stat";
            break;
        case Path::kUptimeFilename : return "/proc/uptime";
            break;
        case Path::kMeminfoFilename : return "/proc/meminfo";
            break;
        case Path::kVersionFilename : return "/proc/version";
            break;
        case Path::kOSPath : return "/etc/os-release";
            break;
        case Path::kPasswordPath : return "/etc/passwd";
            break;
    }
}
