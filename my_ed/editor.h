#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>

#include "common.hpp"
#include "file.h"

namespace MyEd {

    class EditorConstants {
    public:
        // q
        static inline const char *COMMAND_QUIT_EDITOR = "^q$";
        // =
        static inline const char *COMMAND_SHOW_FILE_INFO = R"(^\=$)";
        // (.,.)p
        static inline const char *COMMAND_PRINT = R"(^([\.\$]?|[+|-]?\d*)p|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)p$)";
        // (.,.)n
        static inline const char *COMMAND_PRINT_WITH_LINE_NUM = R"(^([\.\$]?|[+|-]?\d*)n|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)n$)";
        // (.+1)z n
        static inline const char *COMMAND_SCROLL = R"(^([\.\$]?|[+|-]?\d*)z\ ?(\d*)$)";
        // (.)a
        static inline const char *COMMAND_APPEND = R"(^([\.\$]?|[+|-]?\d*)a$)";
        // (.)i
        static inline const char *COMMAND_INSERT = R"(^([\.\$]?|[+|-]?\d*)i$)";
        // (.,.)d
        static inline const char *COMMAND_DELETE = R"(^([\.\$]?|[+|-]?\d*)d|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)d$)";

        // answer yes
        static inline const char *ANSWER_YES = "^y$";
        // answer no
        static inline const char *ANSWER_NO = "^n$";


        // quit insert code mark
        static inline const char *MARK_QUIT_INSERT_MODE = R"(^\.\n|[\s\S]*\n\.\n$)";
        // current line mark
        static inline const char *MARK_CURRENT_LINE = R"(^\.$)";
        // last line mark
        static inline const char *MARK_LAST_LINE = R"(^\$$)";
        // positive number
        static inline const char *POSITIVE_NUMBER = R"(^\+(\d*)$)";
        // negative number
        static inline const char *NEGATIVE_NUMBER = R"(^\-(\d*)$)";
        // empty string
        static inline const char *EMPTY_STRING_MARK = R"(^(?![\s\S]))";
        // comma
        static inline const char *COMMA = R"(^\,$)";
        // print end
        static inline const char *END_MARK = R"(<END)";
        // line print divider
        static inline const char *LINE_PRINT_DIVIDER = R"(:)";
        // newline code
        static inline const char *NEWLINE_CODE = R"(\n)";
        //  empty string
        static inline const char *EMPTY_STRING = "";

        // Message
        static inline const char *STR_WRONG_COMMAND = "Wrong command.";

        static inline const char *STR_FILE_EDITED_BUT_NOT_SAVED_WARING = "This file is modified, are you sure to quit without saving?(y/n):";

        static inline const char *STR_SHOW_FILE_INFO_BEGIN = "============== FILE INFO ==============";
        static inline const char *STR_LINE_COUNT = "line count   : ";
        static inline const char *STR_CURRENT_LINE = "current line : ";
        static inline const char *STR_SHOW_FILE_INFO_END = "================= END =================";

        // default n of (.+1)z n
        static inline const size_t DEFAULT_SCROLL_LINES = 21;
    };

    class Editor {

    private:
        File *m_buffer;
        bool m_modified_but_not_saved;
    public:
        Editor();
        ~Editor();

        void Init();
        bool Init(const std::string &file_name);
        void Destroys();

        bool InputCommand(std::string);

    private:

        [[nodiscard]] size_t _HandleParam(const std::string &) const;
        static std::string _GetUserInputLine();

        [[nodiscard]] bool _QuitEditor() const;

        void _ShowFileInfo() const;
        void _Print(const std::smatch &smatch_params);
        void _PrintWithLineNum(const std::smatch &smatch_params);
        void _Scroll(const std::smatch &smatch_params);
        void _Append(const std::smatch &smatch_params);
        void _Insert(const std::smatch &smatch_params);
        void _Delete(const std::smatch &smatch_params);
    };
}