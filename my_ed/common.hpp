#pragma once

#include <sys/stat.h>

#include <regex>
#include <string>

namespace MyEd {

    class FileUtil {
    public:
        static inline bool IsFileExists(const std::string &file_name) {
            struct stat buffer{};
            return (stat(file_name.c_str(), &buffer) == 0);
        }
    };

    class StringUtil {
    public:
        // trim from start (in place)
        static inline void Ltrim(std::string &input_string) {
            input_string.erase(input_string.begin(),
                               std::find_if(input_string.begin(), input_string.end(), [](unsigned char ch) {
                                   return !std::isspace(ch);
                               }));
        }

        // trim from end (in place)
        static inline void Rtrim(std::string &input_string) {
            input_string.erase(std::find_if(input_string.rbegin(), input_string.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), input_string.end());
        }

        // trim from both ends (in place)
        static inline void Trim(std::string &input_string) {
            Ltrim(input_string);
            Rtrim(input_string);
        }

        // split string with specified delimiter
        static inline std::vector<std::string> Split(std::string input_string, const std::string &delimiter_string) {
            std::smatch match;
            std::regex delimiter_regex(delimiter_string);
            std::vector<std::string> splits;

            while (regex_search(input_string, match, delimiter_regex)) {
                size_t position = match.position();
                splits.push_back(std::move(input_string.substr(0, position)));
                input_string = input_string.substr(position + match.length());
            }
            splits.push_back(std::move(input_string));
            return splits;
        }

        //regex match
        static inline bool Match(const std::string &str, const std::string &pattern) {
            return std::regex_match(str, std::regex(pattern));
        }

        static inline bool Match(const std::string &str, const std::string &pattern, std::smatch &sm) {
            return std::regex_match(str, sm, std::regex(pattern));
        }

        static inline bool Match(const char *str, const std::string &pattern, std::cmatch &cm) {
            return std::regex_match(str, cm, std::regex(pattern));
        }
    };

}
