#pragma once

#include <cassert>

#include <deque>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "common.hpp"

namespace MyEd {

    class FileConstant {
    public:
        constexpr static const size_t DEFAULT_CURRENT_LINE_NUM = 0;
        constexpr static const size_t DEFAULT_LINE_COUNT = 0;
        constexpr static const bool DEFAULT_MODIFY_STATUS = false;
        constexpr static inline const char *DEFAULT_FILE_NAME = "[new file]";
        constexpr static inline const char *FILE_DELIMITER = "\n";

        constexpr static inline const char *EXCEPTION_MESSAGE_LINE_NUM_OUT_OF_RANGE = "Line number must greater than 1 and less or equal than last line.";
        constexpr static inline const char *EXCEPTION_MESSAGE_BAD_LINE_NUM_ORDER = "The first line number must less or equal than the second one.";
    };

    class File {
        friend std::string &operator<<(std::string &, const File &);
        friend std::ostream &operator<<(std::ostream &, const File &);
        friend File &operator<<(File &, const File &);
        friend File Concatenate(const File &, const File &);
        friend File operator+(const File &, const File &);

    private:
        std::deque<std::string> m_buffer;
        size_t m_current_line_num;
        std::string m_file_name;
        bool m_modified_but_not_saved;
    public:
        File();
        explicit File(const std::string &);
        explicit File(std::istream &);
        File(const File &);
        File(File &&) noexcept;

//        ~File();

        //meta
        [[nodiscard]] size_t GetLineCount() const;

        [[nodiscard]] size_t GetCurrentLineNum() const;
        void SetCurrentLineNum(size_t);

        [[nodiscard]] const std::string &GetFileName() const;
        void SetFileName(const std::string &);

        [[nodiscard]] bool GetModifyStatus() const;
        void SetModifyStatus(bool);

        [[nodiscard]] bool IsEmptyFile() const;

        //C
        File &LoadFrom(const std::string &);
        File &LoadFrom(std::istream &);
        File &LoadFrom(const File &);
        File &operator=(const std::string &);
        File &operator=(std::istream &);
        File &operator=(const File &);

        size_t InsertOneOrMultiplyLines(size_t, const std::string &);
        size_t InsertOneOrMultiplyLines(size_t, std::istream &);
        size_t InsertOneOrMultiplyLines(size_t, const File &);

        File &Append(const std::string &);
        File &Append(std::istream &);
        File &Append(const File &);
        File &operator<<(const std::string &);
        File &operator<<(std::istream &);
        File &operator<<(const File &);
        File &operator+=(const std::string &);
        File &operator+=(std::istream &);
        File &operator+=(const File &);

        //R
        const File &SaveTo(std::string &) const;
        const File &SaveTo(std::ostream &) const;
        const File &SaveTo(File &) const;
        const File &operator>>(std::string &) const;
        const File &operator>>(std::ostream &) const;
        const File &operator>>(File &) const;

        std::string GetLine(size_t);
        std::string operator[](size_t);
        std::vector<std::string> GetLinesFromTo(size_t, size_t);
        std::string GetAll();
        std::string operator*();

        //U
        //TODO
//        File Split(size_t);

        //D
        void EraseLine(size_t);
        void EraseLinesFromTo(size_t, size_t);
        void Clear();

        //Validate
        void ValidateInsertParam(size_t line_num) const;
        void ValidateInsertParams(size_t line_from, size_t line_to) const;
        void ValidateReadUpdateDeleteParam(size_t line_num) const;
        void ValidateReadUpdateDeleteParams(size_t line_from, size_t line_to) const;

    private:

        void AutoResize_(size_t expected_new_line_num);
        void InsertLine_(size_t line_num, const std::string &new_line);
        void InsertLine_(size_t line_num, std::string &&new_line);

        [[nodiscard]] const std::string &GetLine_(size_t line_num) const;
        [[nodiscard]] std::vector<std::string> GetLinesFromTo_(size_t line_from, size_t line_to) const;
        [[nodiscard]] std::string GetAll_() const;

        void EraseLine_(size_t line_num);
    };

    std::string &operator<<(std::string &, const File &);
    std::ostream &operator<<(std::ostream &, const File &);
    File &operator<<(File &, const File &);

    File Concatenate(const File &, const File &);
    File operator+(const File &, const File &);

    const std::string &operator>>(const std::string &, File &);
    std::string &operator>>(std::string &, File &);
    std::istream &operator>>(std::istream &, File &);
    const File &operator>>(const File &, File &);
    File &operator>>(File &, File &);
}