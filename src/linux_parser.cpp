#include "linux_parser.h"


vector<string> LinuxParser::parse(const string &path, string grep,
                                  vector<int> desired_fields, string delimiter)
{
    bool on_start = grep[0] == '^';
    if (on_start)
        grep = grep.substr(1, grep.size()-1);

    string line;
    std::ifstream ifs(path);
    while (std::getline(ifs, line)) {

        if ( (on_start && line.find(grep) == 0) || (!on_start && line.find(grep) != std::string::npos) )
            return cut_line(line, desired_fields, delimiter);
    }

    return vector<string> {desired_fields.size(), string()};
}
string LinuxParser::parse(const string &path, string grep, int desired_field, string delimiter)
{
    return LinuxParser::parse(path, grep, vector<int>{desired_field}, delimiter)[0];
}


vector<string> LinuxParser::parse(const string &path, vector<int> desired_fields, string delimiter)
{
    std::ifstream ifs(path);

    string line;
    std::getline(ifs, line);

    return cut_line(line, desired_fields, delimiter);
}
string LinuxParser::parse(const string &path, int desired_field, string delimiter)
{
    return LinuxParser::parse(path, vector<int>{desired_field}, delimiter)[0];
}


vector<string> LinuxParser::cut_line(string line, vector<int> desired_fields, string delimiter)
{
    vector<string> results;
    auto delim_positions = vector<size_t>{0};
    auto highest_field = std::max_element(desired_fields.begin(), desired_fields.end());

    // Remove mutliple spaces if the delimiter is a space
    if (delimiter == " ") {
        string line_cleaned;
        std::replace(line.begin(), line.end(), '\t', ' ');

        for (auto c : line) {
            if (c != ' ' || line_cleaned.back() != ' ')
                line_cleaned.push_back(c);
        }
        line = line_cleaned;
    }

    // Get all delimiter positions (get rid of useless fields)
    size_t s(0);
    while (s != string::npos && int(delim_positions.size()) < *highest_field + 2) {
        s = line.find(delimiter, s+1);
        delim_positions.push_back(s);
    }

    // Feed from desired
    for (auto f : desired_fields) {
        if (f >= int(delim_positions.size())) {
            results.push_back(string());
        } else {
            auto start = f == 0 ? 0 : delim_positions[f] + delimiter.length();
            auto length = f == 0 ? delim_positions[f+1] : delim_positions[f+1] - delim_positions[f] - 1;
            results.push_back(line.substr(start, length));
        }
    }

    return results;
}

string LinuxParser::cut_line(string line, int desired_field, string delimiter)
{
    return cut_line(line, vector<int>{desired_field}, delimiter)[0];
}
