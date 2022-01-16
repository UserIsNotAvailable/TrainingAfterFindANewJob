#include "file.h"

namespace MyEd {
    File::File() : m_buffer(FileConstant::DEFAULT_LINE_COUNT + 1, FileConstant::FILE_DELIMITER),
                   m_current_line_num(FileConstant::DEFAULT_CURRENT_LINE_NUM),
                   m_file_name(FileConstant::DEFAULT_FILE_NAME) {}

    File::File(const std::string &lines) : m_buffer(FileConstant::DEFAULT_LINE_COUNT + 1,
                                                    FileConstant::FILE_DELIMITER),
                                           m_file_name(FileConstant::DEFAULT_FILE_NAME) {
        InsertOneOrMultiplyLines(FileConstant::DEFAULT_CURRENT_LINE_NUM + 1, lines);
        m_current_line_num = FileConstant::DEFAULT_CURRENT_LINE_NUM;
    }

    File::File(std::istream &input_stream) : m_buffer(FileConstant::DEFAULT_LINE_COUNT + 1,
                                                      FileConstant::FILE_DELIMITER),
                                             m_file_name(FileConstant::DEFAULT_FILE_NAME) {
        InsertOneOrMultiplyLines(FileConstant::DEFAULT_CURRENT_LINE_NUM + 1, input_stream);
        m_current_line_num = FileConstant::DEFAULT_CURRENT_LINE_NUM;
    }

    File::File(const File &another_file) {
        this->m_buffer = another_file.m_buffer;
        this->m_file_name = another_file.m_file_name;
        m_current_line_num = another_file.m_current_line_num;
    }

    File::File(File &&another_file) noexcept {
        this->m_buffer = std::move(another_file.m_buffer);
        this->m_file_name = std::move(another_file.m_file_name);
        m_current_line_num = another_file.m_current_line_num;
        another_file.Clear();
    }

    ////////////////////////////////// Public //////////////////////////////////
    //meta info
    size_t File::GetLineCount() const {
        return m_buffer.size() - 1;
    }

    size_t File::GetCurrentLineNum() const {
        return m_current_line_num;
    }

    void File::SetCurrentLineNum(size_t new_num) {
        m_current_line_num = new_num;
    }

    [[nodiscard]] const std::string &File::GetFileName() const {
        return m_file_name;
    }

    void File::SetFileName(const std::string &new_file_name) {
        m_file_name = new_file_name;
    }

    bool File::IsEmptyFile() const {
        return GetLineCount() == FileConstant::DEFAULT_LINE_COUNT;
    }

    //C
    File &File::LoadFrom(const std::string &input_string) {
        Clear();
        InsertOneOrMultiplyLines(FileConstant::DEFAULT_CURRENT_LINE_NUM + 1, input_string);
        m_current_line_num = FileConstant::DEFAULT_CURRENT_LINE_NUM;
        return *this;
    }

    File &File::LoadFrom(std::istream &input_stream) {
        Clear();
        InsertOneOrMultiplyLines(FileConstant::DEFAULT_CURRENT_LINE_NUM + 1, input_stream);
        m_current_line_num = FileConstant::DEFAULT_CURRENT_LINE_NUM;
        return *this;
    }

    File &File::LoadFrom(const File &another_file) {
        Clear();
        InsertOneOrMultiplyLines(FileConstant::DEFAULT_CURRENT_LINE_NUM + 1, another_file);
        m_current_line_num = another_file.GetCurrentLineNum();
        return *this;
    }


    File &File::operator=(const std::string &input_string) {
        return LoadFrom(input_string);
    }

    File &File::operator=(std::istream &input_stream) {
        return LoadFrom(input_stream);
    }

    File &File::operator=(const File &another_file) {
        return LoadFrom(another_file);
    }

    size_t File::InsertOneOrMultiplyLines(size_t line_num, const std::string &input_lines) {
        ValidateInsertParam(line_num);
        m_current_line_num = line_num - 1;
        std::vector<std::string> lines = StringUtil::Split(input_lines, FileConstant::FILE_DELIMITER);
        for (auto ritr = lines.rbegin(); ritr != lines.rend(); ++ritr) {
            _InsertLine(line_num, std::move(*ritr) + FileConstant::FILE_DELIMITER);
            ++m_current_line_num;
        }
        return lines.size();
    }

    size_t File::InsertOneOrMultiplyLines(size_t line_num, std::istream &input_stream) {
        ValidateInsertParam(line_num);
        std::string tmp(std::istreambuf_iterator<char>(input_stream), {});
        return InsertOneOrMultiplyLines(line_num, tmp);
    }

    size_t File::InsertOneOrMultiplyLines(size_t line_num, const File &another_file) {
        ValidateInsertParam(line_num);
        m_current_line_num = line_num - 1;
        for (size_t i = another_file.GetLineCount(); i > FileConstant::DEFAULT_CURRENT_LINE_NUM; --i) {
            _InsertLine(line_num, another_file._GetLine(i));
            ++m_current_line_num;
        }
        return another_file.GetLineCount();
    }

    File &File::Append(const std::string &input_string) {
        InsertOneOrMultiplyLines(GetLineCount() + 1, input_string);
        return *this;
    }

    File &File::Append(std::istream &input_stream) {
        InsertOneOrMultiplyLines(GetLineCount() + 1, input_stream);
        return *this;
    }

    File &File::Append(const File &another_file) {
        InsertOneOrMultiplyLines(GetLineCount() + 1, another_file);
        return *this;
    }

    File &File::operator<<(const std::string &input_string) {
        return Append(input_string);
    }

    File &File::operator<<(std::istream &input_stream) {
        return Append(input_stream);
    }

    File &File::operator<<(const File &another_file) {
        return Append(another_file);
    }

    File &File::operator+=(const std::string &input_string) {
        return Append(input_string);
    }

    File &File::operator+=(std::istream &input_stream) {
        return Append(input_stream);
    }

    File &File::operator+=(const File &another_file) {
        return Append(another_file);
    }

    //R
    const File &File::SaveTo(std::string &output_string) const {
        output_string.clear();
        output_string.append(_GetAll());
        return *this;
    }

    const File &File::SaveTo(std::ostream &output_stream) const {
        output_stream << _GetAll();
        return *this;
    }

    const File &File::SaveTo(File &another_file) const {
        another_file.Clear();
        another_file.InsertOneOrMultiplyLines(FileConstant::DEFAULT_CURRENT_LINE_NUM + 1, _GetAll());
        return *this;
    }

    const File &File::operator>>(std::string &output_string) const {
        return SaveTo(output_string);
    }

    const File &File::operator>>(std::ostream &output_stream) const {
        return SaveTo(output_stream);
    }

    const File &File::operator>>(File &another_file) const {
        return SaveTo(another_file);
    }

    std::string File::GetLine(size_t line_num) {
        ValidateReadUpdateDeleteParam(line_num);
        m_current_line_num = line_num;
        return _GetLine(line_num);
    }

    std::string File::operator[](size_t line_num) {
        return GetLine(line_num);
    }

    std::vector<std::string> File::GetLinesFromTo(size_t line_from, size_t line_to) {
        ValidateReadUpdateDeleteParams(line_from, line_to);
        m_current_line_num = line_to;
        return _GetLinesFromTo(line_from, line_to);
    }

    std::string File::GetAll() {
        m_current_line_num = GetLineCount();
        return _GetAll();
    }

    std::string File::operator*() {
        return GetAll();
    }

    //D
    void File::EraseLine(size_t line_num) {
        ValidateReadUpdateDeleteParam(line_num);
        _EraseLine(line_num);
        if (GetLineCount() < line_num) {
            m_current_line_num = GetLineCount();
        } else {
            m_current_line_num = line_num;
        }
    }

    void File::EraseLinesFromTo(size_t line_from, size_t line_to) {
        ValidateReadUpdateDeleteParams(line_from, line_to);
        while (line_to >= line_from) {
            EraseLine(line_to);
            --line_to;
        }
    }

    void File::Clear() {
        m_buffer.clear();
        _AutoResize(FileConstant::DEFAULT_LINE_COUNT + 1);
        m_current_line_num = FileConstant::DEFAULT_CURRENT_LINE_NUM;
    }

    // parameters validating
    void File::ValidateInsertParam(size_t line_num) const {
        if (line_num <= FileConstant::DEFAULT_CURRENT_LINE_NUM || line_num >= m_buffer.max_size()) {
            throw std::out_of_range(FileConstant::EXCEPTION_MESSAGE_LINE_NUM_OUT_OF_RANGE);
        }
    }

    void File::ValidateInsertParams(size_t line_from, size_t line_to) const {
        ValidateInsertParam(line_from);
        ValidateInsertParam(line_to);
        if (line_from > line_to) {
            throw std::out_of_range(FileConstant::EXCEPTION_MESSAGE_BAD_LINE_NUM_ORDER);
        }
    }

    void File::ValidateReadUpdateDeleteParam(size_t line_num) const {
        if (line_num <= FileConstant::DEFAULT_CURRENT_LINE_NUM || line_num > GetLineCount()) {
            throw std::out_of_range(FileConstant::EXCEPTION_MESSAGE_LINE_NUM_OUT_OF_RANGE);
        }
    }

    void File::ValidateReadUpdateDeleteParams(size_t line_from, size_t line_to) const {
        ValidateReadUpdateDeleteParam(line_from);
        ValidateReadUpdateDeleteParam(line_to);
        if (line_from > line_to) {
            throw std::out_of_range(FileConstant::EXCEPTION_MESSAGE_BAD_LINE_NUM_ORDER);
        }
    }
    ////////////////////////////////// Private //////////////////////////////////

    void File::_AutoResize(size_t expected_new_line_num) {
        ValidateInsertParam(expected_new_line_num);
        if (expected_new_line_num <= m_buffer.size()) {
            return;
        }
        m_buffer.resize(expected_new_line_num, FileConstant::FILE_DELIMITER);
    }

    void File::_InsertLine(size_t line_num, const std::string &new_line) {
        ValidateInsertParam(line_num);
        _AutoResize(line_num);
        m_buffer.insert(m_buffer.begin() + line_num, new_line);
    }

    void File::_InsertLine(size_t line_num, std::string &&new_line) {
        ValidateInsertParam(line_num);
        _AutoResize(line_num);
        m_buffer.insert(m_buffer.begin() + line_num, new_line);
    }

    const std::string &File::_GetLine(size_t line_num) const {
        ValidateReadUpdateDeleteParam(line_num);
        return m_buffer[line_num];
    }

    std::vector<std::string> File::_GetLinesFromTo(size_t line_from, size_t line_to) const {
        ValidateReadUpdateDeleteParams(line_from, line_to);
        std::vector<std::string> tmp;
        while (line_from <= line_to) {
            tmp.push_back(_GetLine(line_from));
            ++line_from;
        }
        return tmp;
    }

    std::string File::_GetAll() const {
        std::string tmp;
        std::vector<std::string> all_lines = _GetLinesFromTo(1, GetLineCount());
        for (auto &line: all_lines) {
            tmp.append(line);
        }
        return tmp;
    }

    void File::_EraseLine(size_t line_num) {
        ValidateReadUpdateDeleteParam(line_num);
        m_buffer.erase(m_buffer.begin() + line_num);
    }

    ////////////////////////////////// Friend Function ans Operator //////////////////////////////////

    std::string &operator<<(std::string &output_string, const File &file) {
        file.SaveTo(output_string);
        return output_string;
    }

    std::ostream &operator<<(std::ostream &output_stream, const File &file) {
        file.SaveTo(output_stream);
        return output_stream;
    }

    File &operator<<(File &file1, const File &file2) {
        file2.SaveTo(file1);
        return file1;
    }

    File Concatenate(const File &file1, const File &file2) {
        File tmp(file1);
        tmp.Append(file2);
        tmp.m_current_line_num = FileConstant::DEFAULT_CURRENT_LINE_NUM;
        return tmp;
    }

    File operator+(const File &file1, const File &file2) {
        return Concatenate(file1, file2);
    }

    const std::string &operator>>(const std::string &input_string, File &file) {
        file.LoadFrom(input_string);
        return input_string;
    }

    std::string &operator>>(std::string &input_string, File &file) {
        file.LoadFrom(input_string);
        return input_string;
    }

    std::istream &operator>>(std::istream &input_stream, File &file) {
        file.LoadFrom(input_stream);
        return input_stream;
    }

    const File &operator>>(const File &file1, File &file2) {
        file2.LoadFrom(file1);
        return file1;
    }

    File &operator>>(File &file1, File &file2) {
        file2.LoadFrom(file1);
        return file1;
    }
}
