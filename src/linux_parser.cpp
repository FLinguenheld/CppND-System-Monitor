#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


std::vector<std::string> LinuxParser::cut_line(std::string line, std::vector<int> desired_fields, std::string delimiter)
{
    std::vector<std::string> results;
    auto delim_positions = std::vector<size_t>{0};
    auto highest_field = std::max_element(desired_fields.begin(), desired_fields.end());

    // Remove mutliple spaces if the delimiter is a space
    if (delimiter == " ") {
        std::string line_cleaned;
        for (auto c : line) {
            if (c != ' ' || line_cleaned.back() != ' ')
                line_cleaned.push_back(c);
        }
        line = line_cleaned;
    }

    // Get all delimiter positions (get rid of useless fields)
    size_t s(0);
    while (s != std::string::npos && delim_positions.size() < *highest_field + 2) {
        s = line.find(delimiter, s+1);
        delim_positions.push_back(s);
    }

    // Feed from desired
    for (auto f : desired_fields) {
        if (f >= delim_positions.size()) {
            results.push_back(std::string());
        } else {
            auto start = f == 0 ? 0 : delim_positions[f] + delimiter.length();
            auto length = f == 0 ? delim_positions[f+1] : delim_positions[f+1] - delim_positions[f] - 1;
            results.push_back(line.substr(start, length));
        }
    }

    return results;
}

std::string LinuxParser::cut_line(std::string line, int field, std::string delimiter)
{
    return cut_line(line, std::vector<int>{field}, delimiter)[0];
}


std::string LinuxParser::parse(const std::string &path, int position, std::string delimiter)
{
    std::ifstream ifs(path);

    std::string line;
    std::getline(ifs, line);

    return cut_line(line, position, delimiter);
}

std::string LinuxParser::parse(const std::string &path, const std::string &grep, int position, std::string delimiter)
{
    std::ifstream ifs(path);

    std::string line;
    while (std::getline(ifs, line)) {

        if ( line.find(grep) == 0 )
            return cut_line(line, position, delimiter);
    }

    return std::string();
}
