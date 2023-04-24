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


class LinuxParser
{
    public:
        enum class Path{
            // kProcDirectory,
            kCmdlineFilename,
            kCpuinfoFilename,
            kStatusFilename,
            kStatFilename,
            kUptimeFilename,
            kMeminfoFilename,
            kVersionFilename,
            kOSPath,
            kPasswordPath,
        };

        std::string parse(Path p, int position, std::string separator);
        std::string parse(Path p, const std::string &grep, int position, std::string separator);

    private:
        std::string cut_line(std::string line, int field, std::string delimiter=" ");
        std::string path(LinuxParser::Path p) const;
};

#endif
