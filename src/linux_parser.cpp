#include "linux_parser.h"


vector<string> LinuxParser::parse(const string &path, string grep, vector<int> desired_fields,
                                  string delimiter, string default_val)
{
    bool on_start = grep[0] == '^';
    if (on_start)
        grep = grep.substr(1, grep.size()-1);

    std::ifstream ifs(path);
    if (!ifs.fail())
    {
        string line;
        while (std::getline(ifs, line)) {

            if ( (on_start && line.find(grep) == 0) || (!on_start && line.find(grep) != std::string::npos) )
                return cut_line(line, desired_fields, delimiter, default_val);
        }
    }

    return vector<string> {desired_fields.size(), default_val};
}
string LinuxParser::parse(const string &path, string grep, int desired_field, string delimiter, string default_val)
{
    return LinuxParser::parse(path, grep, vector<int>{desired_field}, delimiter, default_val)[0];
}


vector<string> LinuxParser::parse(const string &path, vector<int> desired_fields,
                                  string delimiter, string default_val)
{
    std::ifstream ifs(path);
    if (!ifs.fail())
    {
        string line;
        std::getline(ifs, line);

        return cut_line(line, desired_fields, delimiter, default_val);
    }

    return vector<string> {desired_fields.size(), default_val};
}
string LinuxParser::parse(const string &path, int desired_field, string delimiter, string default_val)
{
    return LinuxParser::parse(path, vector<int>{desired_field}, delimiter, default_val)[0];
}


vector<string> LinuxParser::cut_line(string line, vector<int> desired_fields, string delimiter, string default_val)
{
    vector<string> results;
    auto delim_positions = vector<size_t>{0};
    auto highest_field = std::max_element(desired_fields.begin(), desired_fields.end());

    // \0 causes troubles (with /proc/pid/cmdline for instance)
    std::replace(line.begin(), line.end(), '\0', ' ');

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

    if (delimiter.empty()) {        // Take the entire line
            delim_positions.push_back(line.size());
    } else {
        // Get all delimiter positions (get rid of useless fields)
        size_t s(0);
        while (s != string::npos && int(delim_positions.size()) < *highest_field + 2) {
            s = line.find(delimiter, s+1);
            delim_positions.push_back(s);
        }
    }

    // Feed from desired
    for (auto f : desired_fields) {
        if (f >= int(delim_positions.size())) {
            results.push_back(default_val);
        } else {
            auto start = f == 0 ? 0 : delim_positions[f] + delimiter.length();
            auto length = f == 0 ? delim_positions[f+1] : delim_positions[f+1] - delim_positions[f] - 1;
            results.push_back(line.substr(start, length));
        }
    }

    return results;
}

string LinuxParser::cut_line(string line, int desired_field, string delimiter, string default_val)
{
    return cut_line(line, vector<int>{desired_field}, delimiter, default_val)[0];
}
