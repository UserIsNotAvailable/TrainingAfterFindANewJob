#include <iostream>
#include <memory>
#include <string>

#include "editor.h"

static const char *FILE_OPEN_FAILED_INFO = "File does not exist, opened a new file.";
static const char *DEFAULT_COMMAND = "p";

void Usage(const std::string &proc) {
    std::cout << "Usage: " << proc << " [file_name]" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        Usage(argv[0]);
        exit(1);
    }

    std::unique_ptr<MyEd::Editor> up_ed(new MyEd::Editor);
    if (argc > 1) {
        bool is_load_success = up_ed->Init(argv[1]);
        if (!is_load_success) {
            std::cout << FILE_OPEN_FAILED_INFO << std::endl;
        }
    } else {
        up_ed->Init();
    }

    std::string command;
    do {
        std::cin.clear();
        std::getline(std::cin, command);
        if (std::cin.eof()) {
            break;
        }
        if (command == MyEd::EditorConstants::EMPTY_STRING) {
            command = DEFAULT_COMMAND;
        }
    } while (up_ed->InputCommand(command));

    up_ed->Destroys();

    return 0;
}
