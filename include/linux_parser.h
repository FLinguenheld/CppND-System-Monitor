#ifndef LINUX_PARSER_H
#define LINUX_PARSER_H

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using std::string;
using std::vector;

namespace LinuxParser {

    // Paths
    const string kCmdlineFilename{"/proc/cmdline"};
    const string kCpuinfoFilename{"/proc/cpuinfo"};
    const string kStatusFilename{"/proc/status"};
    const string kStatFilename{"/proc/stat"};
    const string kUptimeFilename{"/proc/uptime"};
    const string kMeminfoFilename{"/proc/meminfo"};
    const string kVersionFilename{"/proc/version"};
    const string kOSPath{"/etc/os-release"};
    const string kPasswordPath{"/etc/passwd"};

    /*
     * Open the file with the <path>, find the first line starting with <grep> and cut it with the <delimiter>.
     * Then return all <desired fields>.
     * Set empty strings if grep failed or desired_field(s) failed.
    */
    vector<string> parse(const string &path, const string &grep, vector<int> desired_fields, string delimiter=" ");
    string parse(const string &path, const string &grep, int desired_field, string delimiter=" ");

    /* Overload for single line files. */
    vector<string> parse(const string &path, vector<int> desired_fields, string delimiter=" ");
    string parse(const string &path, int desired_field, string delimiter=" ");

    /* Cut the given <line> with the <delimiter> and return all <desired_fields>. */
    vector<string> cut_line(string line, vector<int> desired_fields, string delimiter=" ");
    string cut_line(string line, int desired_field, string delimiter=" ");

};  // namespace LinuxParser

#endif
