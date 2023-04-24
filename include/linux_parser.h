#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <string>


namespace LinuxParser {

    // Paths
    const std::string kCmdlineFilename{"/proc/cmdline"};
    const std::string kCpuinfoFilename{"/proc/cpuinfo"};
    const std::string kStatusFilename{"/proc/status"};
    const std::string kStatFilename{"/proc/stat"};
    const std::string kUptimeFilename{"/proc/uptime"};
    const std::string kMeminfoFilename{"/proc/meminfo"};
    const std::string kVersionFilename{"/proc/version"};
    const std::string kOSPath{"/etc/os-release"};
    const std::string kPasswordPath{"/etc/passwd"};

    std::string parse(const std::string &path, int position, std::string delimiter=" ");
    std::string parse(const std::string &path, const std::string &grep, int position, std::string delimiter=" ");
    std::string cut_line(std::string line, int field, std::string delimiter=" ");

};  // namespace LinuxParser

#endif
