#include "editor.h"

namespace MyEd {
    Editor::Editor()
            : m_buffer(nullptr), m_modified_but_not_saved(false) {}

    Editor::~Editor() {
        delete m_buffer;
        m_modified_but_not_saved = false;
    }

    void Editor::Init() {
        if (m_buffer == nullptr) {
            m_buffer = new File();
        }
    }

    bool Editor::Init(const std::string &file_name) {
        if (m_buffer == nullptr) {
            m_buffer = new File();
        }

        std::ifstream stream(file_name);
        if (stream.good()) {
            stream >> *m_buffer;
            m_buffer->SetFileName(file_name);
            return true;
        } else {
            return false;
        }
    }

    void Editor::Destroys() {
        delete m_buffer;
        m_buffer = nullptr;
        m_modified_but_not_saved = false;
    }

    bool Editor::InputCommand(std::string command) {
        StringUtil::Trim(command);
        std::smatch smatch_params;
        try {
            // q
            if (StringUtil::Match(command, EditorConstants::COMMAND_QUIT_EDITOR)) {
                return _QuitEditor();
                // Q
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_QUIT_EDITOR_UNCONDITIONALLY)) {
                return _QuitEditorUnconditionally();
                // =
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_SHOW_FILE_INFO)) {
                _ShowFileInfo();
                // (.,.)p
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_PRINT, smatch_params)) {
                _Print(smatch_params, std::cout);
                // (.,.)n
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_PRINT_WITH_LINE_NUM, smatch_params)) {
                _PrintWithLineNum(smatch_params);
                // (.+1)z n
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_SCROLL, smatch_params)) {
                _Scroll(smatch_params);
                // (.)a
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_APPEND, smatch_params)) {
                _Append(smatch_params);
                // (.)i
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_INSERT, smatch_params)) {
                _Insert(smatch_params);
                // (.,.)d
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_DELETE, smatch_params)) {
                _Delete(smatch_params);
                // (.,.)c
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_CHANGE, smatch_params)) {
                _Change(smatch_params);
                // (.,.)m(.)
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_MOVE, smatch_params)) {
                _Move(smatch_params);
                // (.,.)t(.)
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_COPY, smatch_params)) {
                _Copy(smatch_params);
                // (.,.+1)j
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_JOIN, smatch_params)) {
                _Join(smatch_params);
                // (.,$)w file
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_WRITE, smatch_params)) {
                _Write(smatch_params);
                // e file
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_EDIT, smatch_params)) {
                _Edit(smatch_params);
                // E file
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_EDIT_UNCONDITIONALLY, smatch_params)) {
                _EditUnconditionally(smatch_params);
                // ($)r file
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_READ_AND_APPEND, smatch_params)) {
                _ReadAndAppend(smatch_params);
                // others
            } else {
                std::cout << EditorConstants::STR_WRONG_COMMAND << std::endl;
            }
        } catch (const std::out_of_range &ex) {
            std::cout << ex.what() << std::endl;
        } catch (const std::runtime_error &ex) {
            std::cout << ex.what() << std::endl;
        }
        return true;
    }

    size_t Editor::_HandleParam(const std::string &str_param) const {
        size_t ret;
        std::smatch smatch_n;// "n" of "+n" or "-n"
        // . or ""
        if (StringUtil::Match(str_param, EditorConstants::MARK_CURRENT_LINE) ||
            StringUtil::Match(str_param, EditorConstants::EMPTY_STRING_MARK)) {
            ret = m_buffer->GetCurrentLineNum();
            // $
        } else if (StringUtil::Match(str_param, EditorConstants::MARK_LAST_LINE)) {
            ret = m_buffer->GetLineCount();
            // +n
        } else if (StringUtil::Match(str_param, EditorConstants::POSITIVE_NUMBER, smatch_n)) {
            size_t num_n = 1; // default n when str_param is only a "+"

            // str_param with an n
            if (!StringUtil::Match(smatch_n[1], EditorConstants::EMPTY_STRING_MARK)) {
                std::stringstream ss;
                ss << smatch_n[1];
                ss >> num_n;
            }
            ret = m_buffer->GetCurrentLineNum() + num_n;
            // -n
        } else if (StringUtil::Match(str_param, EditorConstants::NEGATIVE_NUMBER, smatch_n)) {
            size_t num_n = 1; // default n when str_param is only a "-"

            // str_param with an n
            if (!StringUtil::Match(smatch_n[1], EditorConstants::EMPTY_STRING_MARK)) {
                std::stringstream ss;
                ss << smatch_n[1];
                ss >> num_n;
            }
            ret = m_buffer->GetCurrentLineNum() - num_n;
            // n
        } else {
            std::stringstream ss;
            ss << str_param;
            ss >> ret;
        }
        return ret;
    }

    bool Editor::_GetUserInputLine(std::string &ret) {
        std::string tmp;
        std::smatch smatch_quit_mark;
        // read from input stream until getting a single "\.\n" or a "xxx\n.\n"
        do {
            char ch;
            std::cin.get(ch);
            tmp += ch;
            if (StringUtil::Match(tmp, EditorConstants::MARK_QUIT_INSERT_MODE, smatch_quit_mark)) {
                break;
            }
        } while (true);

        // "\.\n", which means user quit edit mode without changing the file.
        if (!StringUtil::Match(smatch_quit_mark[1], EditorConstants::EMPTY_STRING_MARK)) {
            return false;
            // "xxx\n\.\n", user input must be handled.
        } else {
            ret += smatch_quit_mark[2];
            return true;
        }
    }

    bool Editor::_QuitEditor() const {
        if (m_modified_but_not_saved) {
            std::string answer;
            do {
                std::cout << EditorConstants::STR_QUIT_WHEN_FILE_EDITED_BUT_NOT_SAVED_WARING << std::flush;
                std::getline(std::cin, answer);
                if (StringUtil::Match(answer, EditorConstants::ANSWER_YES)) {
                    return _QuitEditorUnconditionally();
                } else if (StringUtil::Match(answer, EditorConstants::ANSWER_NO)) {
                    return true;
                }
            } while (true);
        }
        return false;
    }

    bool Editor::_QuitEditorUnconditionally() const {
        return false;
    }

    void Editor::_ShowFileInfo() const {
        std::cout << EditorConstants::STR_SHOW_FILE_INFO_BEGIN << std::endl
                  << EditorConstants::STR_FILE_NAME << m_buffer->GetFileName() << std::endl
                  << EditorConstants::STR_LINE_COUNT << m_buffer->GetLineCount() << std::endl
                  << EditorConstants::STR_CURRENT_LINE << m_buffer->GetCurrentLineNum() << std::endl
                  << EditorConstants::STR_MODIFIED_BUT_NOT_SAVED << m_modified_but_not_saved << std::endl
                  << EditorConstants::STR_SHOW_FILE_INFO_END << std::endl;
    }

    void Editor::_Print(const std::smatch &smatch_params, std::ostream &output_stream) {
        // (line_from,line_to)p
        if (StringUtil::Match(smatch_params[3], EditorConstants::COMMA)) {
            size_t line_from = _HandleParam(smatch_params[2]);
            size_t line_to = _HandleParam(smatch_params[4]);
            m_buffer->ValidateReadUpdateDeleteParams(line_from, line_to);
            std::vector<std::string> lines = m_buffer->GetLinesFromTo(line_from, line_to);
            output_stream << StringUtil::Combine(lines);
            // (line)p
        } else {
            size_t line_num = _HandleParam(smatch_params[1]);
            m_buffer->ValidateReadUpdateDeleteParam(line_num);
            output_stream << m_buffer->GetLine(line_num);
        }
    }

    void Editor::_PrintWithLineNum(const std::smatch &smatch_params) {
        // (line_from,line_to)p
        if (StringUtil::Match(smatch_params[3], EditorConstants::COMMA)) {
            size_t line_from = _HandleParam(smatch_params[2]);
            size_t line_to = _HandleParam(smatch_params[4]);
            m_buffer->ValidateReadUpdateDeleteParams(line_from, line_to);
            std::vector<std::string> lines = m_buffer->GetLinesFromTo(line_from, line_to);
            for (auto &line: lines) {
                std::cout << line_from << EditorConstants::LINE_PRINT_DIVIDER << line;
                ++line_from;
            }
            // (line)p
        } else {
            size_t line_num = _HandleParam(smatch_params[1]);
            m_buffer->ValidateReadUpdateDeleteParam(line_num);
            std::cout << line_num << EditorConstants::LINE_PRINT_DIVIDER << m_buffer->GetLine(line_num);
        }
    }

    void Editor::_Scroll(const std::smatch &smatch_params) {
        // assume first param note entered, scroll from next line of current line
        size_t line_from = m_buffer->GetCurrentLineNum() + 1;
        // if at the last line of the file
        if (line_from > m_buffer->GetLineCount()) {
            // stick to the last line
            line_from = m_buffer->GetLineCount();
        }

        // if first param (scrow from) was entered
        if (!StringUtil::Match(smatch_params[1], EditorConstants::EMPTY_STRING_MARK)) {
            line_from = _HandleParam(smatch_params[1]);
        }
        m_buffer->ValidateReadUpdateDeleteParam(line_from);

        // default value
        size_t line_to = line_from + EditorConstants::DEFAULT_SCROLL_LINES - 1;

        // if n is entered
        if (!StringUtil::Match(smatch_params[2], EditorConstants::EMPTY_STRING_MARK)) {
            line_to = line_from + _HandleParam(smatch_params[2]) - 1;
        }
        if (line_to > m_buffer->GetLineCount()) {
            line_to = m_buffer->GetLineCount();
        }
        std::vector<std::string> lines = m_buffer->GetLinesFromTo(line_from, line_to);
        std::cout << StringUtil::Combine(lines);
    }

    void Editor::_Append(const std::smatch &smatch_params) {
        size_t line_num = _HandleParam(smatch_params[1]) + 1;
        m_buffer->ValidateInsertParam(line_num);
        std::string input_lines;
        if (_GetUserInputLine(input_lines)) {
            m_buffer->InsertOneOrMultiplyLines(line_num, input_lines);
            m_modified_but_not_saved = true;
        }
    }

    void Editor::_Insert(const std::smatch &smatch_params) {
        size_t line_num = _HandleParam(smatch_params[1]);
        m_buffer->ValidateInsertParam(line_num);
        std::string input_lines;
        if (_GetUserInputLine(input_lines)) {
            m_buffer->InsertOneOrMultiplyLines(line_num, input_lines);
            m_modified_but_not_saved = true;
        }
    }

    void Editor::_Delete(const std::smatch &smatch_params) {
        // (line_from,line_to)d
        if (StringUtil::Match(smatch_params[3], EditorConstants::COMMA)) {
            size_t line_from = _HandleParam(smatch_params[2]);
            size_t line_to = _HandleParam(smatch_params[4]);
            m_buffer->ValidateReadUpdateDeleteParams(line_from, line_to);
            m_buffer->EraseLinesFromTo(line_from, line_to);
            // (line)d
        } else {
            size_t line_num = _HandleParam(smatch_params[1]);
            m_buffer->ValidateReadUpdateDeleteParam(line_num);
            m_buffer->EraseLine(line_num);
        }
        m_modified_but_not_saved = true;
    }

    void Editor::_Change(const std::smatch &smatch_params) {
        _Delete(smatch_params);
        std::string input_lines;
        if (_GetUserInputLine(input_lines)) {
            m_buffer->InsertOneOrMultiplyLines(m_buffer->GetCurrentLineNum(), input_lines);
            m_modified_but_not_saved = true;
        }
    }

    void Editor::_Move(const std::smatch &smatch_params) {
        // (line_src_from, line_src_to)m(line_dst)
        if (StringUtil::Match(smatch_params[4], EditorConstants::COMMA)) {
            size_t line_src_from = _HandleParam(smatch_params[3]);
            size_t line_src_to = _HandleParam(smatch_params[5]);
            size_t line_dst = _HandleParam(smatch_params[6]) + 1;
            m_buffer->ValidateReadUpdateDeleteParams(line_src_from, line_src_to);
            m_buffer->ValidateInsertParam(line_dst);
            std::vector<std::string> lines = m_buffer->GetLinesFromTo(line_src_from, line_src_to);
            m_buffer->EraseLinesFromTo(line_src_from, line_src_to);
            if (line_dst > line_src_from && line_dst <= line_src_to + 1) {
                line_dst = line_src_from;
            } else if (line_dst > line_src_to + 1) {
                line_dst -= (line_src_to - line_src_from + 1);
            }
            m_buffer->InsertOneOrMultiplyLines(line_dst, StringUtil::Combine(lines));
            // (line_src)m(line_dst)
        } else {
            size_t line_src = _HandleParam(smatch_params[1]);
            size_t line_dst = _HandleParam(smatch_params[2]) + 1;
            m_buffer->ValidateReadUpdateDeleteParam(line_src);
            m_buffer->ValidateInsertParam(line_dst);
            std::string line = m_buffer->GetLine(line_src);
            m_buffer->EraseLine(line_src);
            if (line_dst > line_src) {
                line_dst -= 1;
            }
            m_buffer->InsertOneOrMultiplyLines(line_dst, line);

        }
        m_modified_but_not_saved = true;
    }

    void Editor::_Copy(const std::smatch &smatch_params) {
        // (line_src_from, line_src_to)t(line_dst)
        if (StringUtil::Match(smatch_params[4], EditorConstants::COMMA)) {
            size_t line_src_from = _HandleParam(smatch_params[3]);
            size_t line_src_to = _HandleParam(smatch_params[5]);
            size_t line_dst = _HandleParam(smatch_params[6]) + 1;
            m_buffer->ValidateReadUpdateDeleteParams(line_src_from, line_src_to);
            m_buffer->ValidateInsertParam(line_dst);
            std::vector<std::string> lines = m_buffer->GetLinesFromTo(line_src_from, line_src_to);
            m_buffer->InsertOneOrMultiplyLines(line_dst, StringUtil::Combine(lines));
            // (line_src)t(line_dst)
        } else {
            size_t line_src = _HandleParam(smatch_params[1]);
            size_t line_dst = _HandleParam(smatch_params[2]) + 1;
            m_buffer->ValidateReadUpdateDeleteParam(line_src);
            m_buffer->ValidateInsertParam(line_dst);
            std::string line = m_buffer->GetLine(line_src);
            m_buffer->InsertOneOrMultiplyLines(line_dst, line);

        }
        m_modified_but_not_saved = true;
    }

    void Editor::_Join(const std::smatch &smatch_params) {
        size_t line_from;
        size_t line_to;
        // (line_from,line_to)j
        if (StringUtil::Match(smatch_params[4], EditorConstants::COMMA)) {
            line_from = _HandleParam(smatch_params[2]);
            if (StringUtil::Match(smatch_params[4], EditorConstants::PERIOD) ||
                StringUtil::Match(smatch_params[4], EditorConstants::EMPTY_STRING_MARK)) {
                line_to = line_from + 1;
            } else {
                line_to = _HandleParam(smatch_params[4]);
            }
            // (line_num)j
        } else {
            line_from = _HandleParam(smatch_params[1]);
            line_to = line_from + 1;
        }
        m_buffer->ValidateReadUpdateDeleteParams(line_from, line_to);
        std::vector<std::string> lines = m_buffer->GetLinesFromTo(line_from, line_to);
        m_buffer->EraseLinesFromTo(line_from, line_to);
        m_buffer->InsertOneOrMultiplyLines(line_from, StringUtil::Combine(
                StringUtil::Split(StringUtil::Combine(lines), EditorConstants::NEWLINE_CODE)));
        m_modified_but_not_saved = true;
    }

    void Editor::_Write(const std::smatch &smatch_params) {
        size_t line_from;
        size_t line_to;
        std::string path;
        // (line_from,line_to)w file
        if (StringUtil::Match(smatch_params[4], EditorConstants::COMMA)) {
            // line_from
            if (StringUtil::Match(smatch_params[3], EditorConstants::EMPTY_STRING_MARK)) {
                line_from = 1;
            } else {
                line_from = _HandleParam(smatch_params[3]);
            }
            // line_to
            if (StringUtil::Match(smatch_params[5], EditorConstants::EMPTY_STRING_MARK)) {
                line_to = m_buffer->GetLineCount();
            } else {
                line_to = _HandleParam(smatch_params[5]);
            }
            // file
            path = smatch_params[6];
            // (line_num)w file
        } else {
            // line_from
            if (StringUtil::Match(smatch_params[1], EditorConstants::EMPTY_STRING_MARK)) {
                line_from = 1;
            } else {
                line_from = _HandleParam(smatch_params[1]);
            }

            // line_to
            line_to = m_buffer->GetLineCount();

            // file
            path = smatch_params[2];
        }

        m_buffer->ValidateReadUpdateDeleteParams(line_from, line_to);
        size_t current_line_num = m_buffer->GetCurrentLineNum();
        std::vector<std::string> lines = m_buffer->GetLinesFromTo(line_from, line_to);
        // to keep current line num same as before
        m_buffer->SetCurrentLineNum(current_line_num);
        std::ofstream of_stream(path);
        of_stream << StringUtil::Combine(lines);
        m_buffer->SetFileName(path);
        m_modified_but_not_saved = false;
    }

    void Editor::_Edit(const std::smatch &smatch_params) {
        if (!FileUtil::IsFileExists(smatch_params[1])) {
            throw std::runtime_error(EditorConstants::STR_FILE_DOESNT_EXIST_WARING);
        }

        if (!m_modified_but_not_saved) {
            _EditUnconditionally(smatch_params);
            return;
        }

        std::string answer;
        do {
            std::cout << EditorConstants::STR_LOAD_NEW_WHEN_FILE_EDITED_BUT_NOT_SAVED_WARING << std::flush;
            std::getline(std::cin, answer);
            if (StringUtil::Match(answer, EditorConstants::ANSWER_YES)) {
                _EditUnconditionally(smatch_params);
                return;
            } else if (StringUtil::Match(answer, EditorConstants::ANSWER_NO)) {
                return;
            }
        } while (true);
    }

    void Editor::_EditUnconditionally(const std::smatch &smatch_params) {
        std::string in_file_path = smatch_params[1];
        if (!FileUtil::IsFileExists(in_file_path)) {
            throw std::runtime_error(EditorConstants::STR_FILE_DOESNT_EXIST_WARING);
        }
        std::ifstream if_stream(in_file_path);
        if_stream >> *m_buffer;
        m_buffer->SetCurrentLineNum(m_buffer->GetLineCount());
        m_buffer->SetFileName(in_file_path);
        m_modified_but_not_saved = false;
    }

    void Editor::_ReadAndAppend(const std::smatch &smatch_params) {
        size_t line_num = _HandleParam(smatch_params[1]) + 1;
        m_buffer->ValidateInsertParam(line_num);

        std::string in_file_path = smatch_params[2];
        if (!FileUtil::IsFileExists(in_file_path)) {
            throw std::runtime_error(EditorConstants::STR_FILE_DOESNT_EXIST_WARING);
        }
        std::ifstream if_stream(in_file_path);
        m_buffer->InsertOneOrMultiplyLines(line_num, if_stream);
        m_modified_but_not_saved = true;
    }
}