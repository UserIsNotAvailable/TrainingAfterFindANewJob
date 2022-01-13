#include <iostream>
#include <string>

#include "editor.h"

static const char *FILE_OPEN_FAILED_INFO = "File does not exist, opened a new file.";

void Usage(const std::string &proc) {
    std::cout << "Usage: " << proc << " [file_name]" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        Usage(argv[0]);
        exit(1);
    }

    MyEd::Editor *ed = new MyEd::Editor;
    if (argc > 1) {
        bool is_load_success = ed->Init(argv[1]);
        if (!is_load_success) {
            std::cout << FILE_OPEN_FAILED_INFO << std::endl;
        }
    } else {
        ed->Init();
    }

    std::string command;
    do {
        std::cin >> std::ws;
        std::getline(std::cin, command);
    } while (ed->InputCommand(command));

    ed->Destroys();
    delete ed;

    return 0;
}
