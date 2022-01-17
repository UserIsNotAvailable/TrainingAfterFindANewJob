#pragma once

#include <sys/stat.h>

#include <numeric>
#include <regex>
#include <string>

namespace MyEd {

    class FileUtil {
    public:
        static bool IsFileExists(const std::string &file_name) {
            struct stat buffer{};
            return stat(file_name.c_str(), &buffer) == 0;
        }
    };

    class StringUtil {
    public:
        // trim from start (in place)
        static void Ltrim(std::string &input_string) {
            input_string.erase(input_string.begin(),
                               std::find_if(input_string.begin(), input_string.end(), [](unsigned char ch) {
                                   return !std::isspace(ch);
                               }));
        }

        // trim from end (in place)
        static void Rtrim(std::string &input_string) {
            input_string.erase(std::find_if(input_string.rbegin(), input_string.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), input_string.end());
        }

        // trim from both ends (in place)
        static void Trim(std::string &input_string) {
            Ltrim(input_string);
            Rtrim(input_string);
        }

        // split string with specified delimiter
        static std::vector<std::string> Split(std::string input_string, const std::string &delimiter_string) {
            std::regex delimiter_regex(delimiter_string);
            std::sregex_token_iterator
                    first{input_string.begin(), input_string.end(), delimiter_regex, -1},
                    last;
            return {first, last};
        }

        // combine a vector of strings
        static std::string Combine(std::vector<std::string> vector_of_strings) {
            return std::accumulate(vector_of_strings.begin(), vector_of_strings.end(), std::string(""));
        }

        //regex match
        static bool Match(const std::string &str, const std::string &pattern) {
            return std::regex_match(str, std::regex(pattern));
        }

        static bool Match(const std::string &str, const std::string &pattern, std::smatch &sm) {
            return std::regex_match(str, sm, std::regex(pattern));
        }

        static bool Match(const char *str, const std::string &pattern, std::cmatch &cm) {
            return std::regex_match(str, cm, std::regex(pattern));
        }

        // find the n-th substring of input string
        template<typename T>
        static std::string::size_type
        NthSubstr(std::string::size_type n, const std::basic_string<T> &s, const std::basic_string<T> &p,
                  bool repeats = false) {
            std::string::size_type i = s.find(p);
            std::string::size_type adv = (repeats) ? 1 : p.length();
            std::string::size_type j;
            for (j = 1; j < n && i != std::basic_string<T>::npos; ++j) i = s.find(p, i + adv);
            if (j == n) return i;
            else return -1;
        }

        static bool ReplaceAll(std::string &str, const std::string &from, const std::string &to) {
            bool replaced = false;
            if (from.empty()) return replaced;
            size_t start_pos = 0;
            while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length();
                replaced = true;
            }
            return replaced;
        }
    };
}
