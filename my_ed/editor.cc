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
                // =
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_SHOW_FILE_INFO)) {
                // (.,.)p
                _ShowFileInfo();
            } else if (StringUtil::Match(command, EditorConstants::COMMAND_PRINT, smatch_params)) {
                _Print(smatch_params);
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
                // others
            } else {
                std::cout << EditorConstants::STR_WRONG_COMMAND << std::endl;
            }
        } catch (std::out_of_range &ex) {
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

    std::string Editor::_GetUserInputLine() {
        std::string ret;
        // read from input stream until getting a single "\n\." or "xxx\n.\n"
        do {
            char ch;
            std::cin.get(ch);
            ret += ch;
            if (StringUtil::Match(ret, EditorConstants::MARK_QUIT_INSERT_MODE)) {
                break;
            }
        } while (true);

        // erase the last ".\n"
        if (!ret.empty()) {
            ret.pop_back();
            ret.pop_back();
        }
        return ret;
    }

    bool Editor::_QuitEditor() const {
        if (m_modified_but_not_saved) {
            std::string answer;
            do {
                std::cout << EditorConstants::STR_FILE_EDITED_BUT_NOT_SAVED_WARING << std::flush;
                std::cin >> answer;
                if (StringUtil::Match(answer, EditorConstants::ANSWER_YES)) {
                    return false;
                } else if (StringUtil::Match(answer, EditorConstants::ANSWER_NO)) {
                    return true;
                }
            } while (true);
        }
        return false;
    }

    void Editor::_ShowFileInfo() const {
        std::cout << EditorConstants::STR_SHOW_FILE_INFO_BEGIN << std::endl
                  << EditorConstants::STR_LINE_COUNT << m_buffer->GetLineCount() << std::endl
                  << EditorConstants::STR_CURRENT_LINE << m_buffer->GetCurrentLineNum() << std::endl
                  << EditorConstants::STR_SHOW_FILE_INFO_END << std::endl;
    }

    void Editor::_Print(const std::smatch &smatch_params) {
        // (line_from,line_to)p
        if (StringUtil::Match(smatch_params[3], EditorConstants::COMMA)) {
            size_t line_from = _HandleParam(smatch_params[2]);
            size_t line_to = _HandleParam(smatch_params[4]);
            m_buffer->ValidateReadUpdateDeleteParams(line_from, line_to);
            std::vector<std::string> lines = m_buffer->GetLinesFromTo(line_from, line_to);
            for (auto &line: lines) {
                std::cout << line;
            }
            // (line)p
        } else {
            size_t line_num = _HandleParam(smatch_params[1]);
            m_buffer->ValidateReadUpdateDeleteParam(line_num);
            std::cout << m_buffer->GetLine(line_num);
        }
        std::cout << EditorConstants::END_MARK << std::endl;
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
        std::cout << EditorConstants::END_MARK << std::endl;
    }

    void Editor::_Scroll(const std::smatch &smatch_params) {
        // assume first param note entered, scrow from next line of current line
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
        size_t line_to = line_from + EditorConstants::DEFAULT_SCROLL_LINES;

        // if n is entered
        if (!StringUtil::Match(smatch_params[2], EditorConstants::EMPTY_STRING_MARK)) {
            line_to = line_from + _HandleParam(smatch_params[2]) - 1;
        }
        if (line_to > m_buffer->GetLineCount()) {
            line_to = m_buffer->GetLineCount();
        }
        std::vector<std::string> lines = m_buffer->GetLinesFromTo(line_from, line_to);
        for (auto &line: lines) {
            std::cout << line;
        }
        std::cout << EditorConstants::END_MARK << std::endl;
    }

    void Editor::_Append(const std::smatch &smatch_params) {
        size_t line_num = _HandleParam(smatch_params[1]) + 1;
        m_buffer->ValidateInsertParam(line_num);
        std::string input_lines = _GetUserInputLine();
        if (!StringUtil::Match(input_lines, EditorConstants::EMPTY_STRING_MARK)) {
            // a "\n" will insert two empty lines, so turn it to "" before insertion
            if (StringUtil::Match(input_lines, EditorConstants::NEWLINE_CODE)) {
                input_lines = EditorConstants::EMPTY_STRING;
            }
            m_buffer->InsertOneOrMultiplyLines(line_num, input_lines);
        }
        m_modified_but_not_saved = true;
    }

    void Editor::_Insert(const std::smatch &smatch_params) {
        size_t line_num = _HandleParam(smatch_params[1]);
        m_buffer->ValidateInsertParam(line_num);
        std::string input_lines = _GetUserInputLine();
        if (!StringUtil::Match(input_lines, EditorConstants::EMPTY_STRING_MARK)) {
            // a "\n" will insert two empty lines, so turn it to "" before insertion
            if (StringUtil::Match(input_lines, EditorConstants::NEWLINE_CODE)) {
                input_lines = EditorConstants::EMPTY_STRING;
            }
            m_buffer->InsertOneOrMultiplyLines(line_num, input_lines);
        }
        m_modified_but_not_saved = true;
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
}