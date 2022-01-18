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
        constexpr static inline const char *COMMAND_QUIT_EDITOR = "^q$";
        // Q
        constexpr static inline const char *COMMAND_QUIT_EDITOR_UNCONDITIONALLY = "^Q$";
        // =
        constexpr static inline const char *COMMAND_SHOW_FILE_INFO = R"(^\=$)";
        // (.,.)p
        constexpr static inline const char *COMMAND_PRINT = R"(^([\.\$]?|[+|-]?\d*)p?|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)p?$)";
        // (.,.)n
        constexpr static inline const char *COMMAND_PRINT_WITH_LINE_NUM = R"(^([\.\$]?|[+|-]?\d*)n|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)n$)";
        // (.+1)z n
        constexpr static inline const char *COMMAND_SCROLL = R"(^([\.\$]?|[+|-]?\d*)z\ ?(\d*)$)";
        // (.)a
        constexpr static inline const char *COMMAND_APPEND = R"(^([\.\$]?|[+|-]?\d*)a$)";
        // (.)i
        constexpr static inline const char *COMMAND_INSERT = R"(^([\.\$]?|[+|-]?\d*)i$)";
        // (.,.)d
        constexpr static inline const char *COMMAND_DELETE = R"(^([\.\$]?|[+|-]?\d*)d|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)d$)";
        // (.,.)c
        constexpr static inline const char *COMMAND_CHANGE = R"(^([\.\$]?|[+|-]?\d*)c|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)c$)";
        // (.,.)m(.)
        constexpr static inline const char *COMMAND_MOVE = R"(^([\.\$]?|[+|-]?\d*)m([\.\$]?|[+|-]?\d*)|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)m([\.\$]?|[+|-]?\d*)$)";
        // (.,.)t(.)
        constexpr static inline const char *COMMAND_COPY = R"(^([\.\$]?|[+|-]?\d*)t([\.\$]?|[+|-]?\d*)|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)t([\.\$]?|[+|-]?\d*)$)";
        // (.,.+1)j
        constexpr static inline const char *COMMAND_JOIN = R"(^([\.\$]?|[+|-]?\d*)j|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)j$)";
        // (.,$)w file
        constexpr static inline const char *COMMAND_WRITE = R"(^([\.\$]?|[+|-]?\d*)w\ ([\s\S]*)|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)w\ ([\s\S]*)$)";
        // e file
        constexpr static inline const char *COMMAND_EDIT = R"(^e\ ([\s\S]*)$)";
        // E file
        constexpr static inline const char *COMMAND_EDIT_UNCONDITIONALLY = R"(^E\ ([\s\S]*)$)";
        // ($)r file
        constexpr static inline const char *COMMAND_READ_AND_APPEND = R"(^([\.\$]?|[+|-]?\d*)r\ ([\s\S]*)$)";
        // (.,.)s/search/replacement/
        // (.,.)s/search/replacement/g
        // (.,.)s/search/replacement/n
        constexpr static inline const char *COMMAND_SEARCH_AND_REPLACE = R"(^([\.\$]?|[+|-]?\d*)s/([\s\S]*)/([\s\S]*)/(g|[1-9]\d*|\s*)|([\.\$]?|[+|-]?\d*)(,)([\.\$]?|[+|-]?\d*)s/([\s\S]*)/([\s\S]*)/(g|[1-9]\d*|\s*)$)";
        // u
        constexpr static inline const char *COMMAND_UNDOES = R"(^u$)";

        // answer yes
        constexpr static inline const char *ANSWER_YES = "^y$";
        // answer no
        constexpr static inline const char *ANSWER_NO = "^n$";


        // quit insert code mark
        constexpr static inline const char *MARK_QUIT_INSERT_MODE = R"(^(\.\n)|([\s\S]*?)\n\.\n$)";
        // current line mark
        constexpr static inline const char *MARK_CURRENT_LINE = R"(^\.$)";
        // last line mark
        constexpr static inline const char *MARK_LAST_LINE = R"(^\$$)";
        // positive number
        constexpr static inline const char *POSITIVE_NUMBER = R"(^\+(\d*)$)";
        // negative number
        constexpr static inline const char *NEGATIVE_NUMBER = R"(^\-(\d*)$)";
        // empty string
        constexpr static inline const char *EMPTY_STRING_MARK = R"(^(?![\s\S]))";
        // comma
        constexpr static inline const char *COMMA = R"(^\,$)";
        // line print divider
        constexpr static inline const char *LINE_PRINT_DIVIDER = R"(:)";
        // newline code
        constexpr static inline const char *NEWLINE_CODE = R"(\n)";
        // empty string
        constexpr static inline const char *EMPTY_STRING = "";
        // period
        constexpr static inline const char *PERIOD = R"(\.)";
        // global
        constexpr static inline const char *GLOBAL = R"(^g$)";

        // Message
        constexpr static inline const char *STR_WRONG_COMMAND = "Wrong command.";

        constexpr static inline const char *STR_QUIT_WHEN_FILE_EDITED_BUT_NOT_SAVED_WARING = "This file is modified, are you sure to quit without saving it?(y/n):";
        constexpr static inline const char *STR_LOAD_NEW_WHEN_FILE_EDITED_BUT_NOT_SAVED_WARING = "This file is modified, are you sure to load a new file without saving it?(y/n):";
        constexpr static inline const char *STR_FILE_DOESNT_EXIST_WARING = "File doesn't exist.";

        constexpr static inline const char *STR_SHOW_FILE_INFO_BEGIN = "============== FILE INFO ==============";
        constexpr static inline const char *STR_FILE_NAME = "file name   :";
        constexpr static inline const char *STR_LINE_COUNT = "line count  :";
        constexpr static inline const char *STR_CURRENT_LINE = "current line:";
        constexpr static inline const char *STR_MODIFIED_BUT_NOT_SAVED = "modified    :";
        constexpr static inline const char *STR_SHOW_FILE_INFO_END = "================= END =================";

        // default n of (.+1)z n
        constexpr static inline const size_t DEFAULT_SCROLL_LINES = 22;
    };

    class Editor {

    private:
        File *m_buffer;
        File *m_buffer_prev;
    public:
        Editor();
        ~Editor();

        void Init();
        bool Init(const std::string &file_name);

        void Destroys();

        bool InputCommand(std::string);

    private:

        [[nodiscard]] size_t HandleParam_(const std::string &str_param) const;
        static bool GetUserInputLine_(std::string &ret);

        [[nodiscard]] bool QuitEditor_() const;
        [[nodiscard]] bool QuitEditorUnconditionally_() const;

        void ShowFileInfo_() const;
        void Print_(const std::smatch &, std::ostream &);
        void PrintWithLineNum_(const std::smatch &);
        void Scroll_(const std::smatch &);
        void Append_(const std::smatch &);
        void Insert_(const std::smatch &);
        void Delete_(const std::smatch &);
        void Change_(const std::smatch &);
        void Move_(const std::smatch &);
        void Copy_(const std::smatch &);
        void Join_(const std::smatch &);
        void Write_(const std::smatch &);
        void Edit_(const std::smatch &);
        void EditUnconditionally_(const std::smatch &);
        void ReadAndAppend_(const std::smatch &);
	void SearchAndReplace_(const std::smatch &);
        void SavePrev_(const File &);
        void Undoes_();
    };
}
