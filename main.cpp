#include "MiniGit.hpp"
#include <iostream>
#include <sstream>

int main() {
    MiniGit mgit;
    std::string command;

    std::cout << "Welcome to MiniGit!\n";
    std::cout << "Enter command (init, add <file>, commit -m <msg>, log , branch , exit): ";

    while (std::getline(std::cin, command)) {
        if (command == "init") {
            mgit.init();
        } else if (command.rfind("add ", 0) == 0) {
            std::string filename = command.substr(4);
            mgit.add(filename);
        } else if (command.rfind("commit -m ", 0) == 0) {
            std::string message = command.substr(10);
            if (!message.empty() && message.front() == '"' && message.back() == '"') {
                message = message.substr(1, message.size() - 2); // remove quotes
            }
            mgit.commit(message);
        }else if (command == "log") {
    mgit.log();
}else if (command.rfind("branch ", 0) == 0) {
    std::string name = command.substr(7);
    mgit.branch(name);
}else if (command.rfind("checkout ", 0) == 0) {
    std::string name = command.substr(9);
    mgit.checkout(name);
}else if (command.rfind("merge ", 0) == 0) {
    std::string name = command.substr(6);
    mgit.merge(name);
}else if (command.rfind("diff ", 0) == 0) {
    std::istringstream ss(command);
    std::string cmd, c1, c2;
    ss >> cmd >> c1 >> c2;
    if (!c1.empty() && !c2.empty()) {
        mgit.diff(c1, c2);
    } else {
        std::cout << "Usage: diff <commit1> <commit2>\n";
    }
}else if (command == "exit") {
            break;
        } else {
            std::cout << "Unknown command.\n";
        }

        std::cout << "\nEnter command (init, add <file>, commit -m <msg>, exit): ";
    }

    return 0;
}
