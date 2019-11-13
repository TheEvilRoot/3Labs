#include <iostream>

#include <map>
#include <functional>
#include <fstream>
#include <utility>
#include <string>

#include "api.h"
#include "student.h"

struct FileContext {
  std::fstream file;
  std::string fileName;
};

typedef std::function<const char(FileContext&, std::vector<std::string>&, InputHandler&)> CommandCallback;

struct Command {
  CommandCallback func;
  std::string description;
};
typedef std::map<std::string, Command> CommandsMap;

void cmd(CommandsMap &commands, const std::string& name, std::string description, CommandCallback callback) {
    commands.insert({
        name,{ std::move(callback), std::move(description) }
    });
}

void init(CommandsMap& commands) {
    cmd(commands, "help", "Display help message", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler){
        return ERR_SUCCEED;
    });

    cmd(commands, "exit", "Exit the program", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        return ERR_EXIT_CODE;
    });

    cmd(commands, "test", "test", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        std::ofstream file("file.bin");
        std::ofstream tfile("file.txt");

        Student st(123, "Arya Stark", "KSiS", 2);
        st.toBinaryFile(file);
        st.toTextFile(tfile);

        file.flush();
        tfile.flush();

        file.close();
        tfile.close();

      return ERR_SUCCEED;
    });

    cmd(commands, "wb", "Write new student to binary database", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        std::cout << "Enter new student's data: \n";
        Student st = Student::fromInputHandler(handler);

        std::cout << "Writing student \n" << st << "\n ... to binary database\n";

        std::ofstream file("database.bin", std::ios::binary | std::ios::app);

        if(!file.is_open()) {
            std::cout << "Unable to open file...\n";
            return 99;
        }

        st.toBinaryFile(file);

        std::cout << "Done.\n";
        return ERR_SUCCEED;
    });

    cmd(commands, "rb", "Read binary database", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        std::ifstream file("file.bin", std::ios::binary);
        if(!file.is_open()) {
          std::cout << "Unable to open file...\n";
          return 99;
        }

        if (args.empty() || args[0] != "-r") {
            while (!file.eof()) {
                try {
                    Student st = Student::fromBinary(file);
                    std::cout << st << "\n";
                } catch (...) {
                    std::cout << "Unable to get student...\n";
                    break;
                }
            }
        } else {
            // TODO: I don't know!
        }
        return ERR_SUCCEED;
    });

}

void init(I18N& i18n) {
    i18n.insert({ ERR_SUCCEED, "Succeed" });
    i18n.insert({ ERR_INVALID_VALUE, "Value you entered is invalid" });
    i18n.insert({ ERR_LOWER_BORDER_EXCEED, "Value you just entered lower than we can proceed" });
    i18n.insert({ ERR_UPPER_BORDER_EXCEED, "Value you just entered higher than we can proceed" });
    i18n.insert({ ERR_INPUT_EMPTY, "Enter something..." });
    i18n.insert({ ERR_NO_SUCH_CMD, "No such command" });
    i18n.insert({ ERR_INVALID_ARGS, "Invalid arguments for this command" });
    i18n.insert({ ERR_EXIT_CODE, "Succeed" });
}

char run(FileContext& context, CommandsMap& commands, int lastExitCode, InputHandler& handler) {
    std::string inString = handler.enterString("[" + std::to_string(lastExitCode) + "] # ");
    if (inString.length() ==  0) return ERR_INPUT_EMPTY;
    std::vector<std::string> tokens = handler.splitString(inString, ' ');

    if (commands.find(tokens[0]) == commands.end()) {
        return ERR_NO_SUCH_CMD;
    }

    auto command = commands[tokens[0]];
    tokens.erase(tokens.begin());

    return command.func(context, tokens, handler);
}

bool handleExitCode(const char exitCode, int& lastExitCode, I18N& i18n) {
    switch(exitCode) {
        case ERR_SUCCEED:
            lastExitCode = 0;
        case ERR_INPUT_EMPTY:
            return true;
        case ERR_EXIT_CODE:
            return false;
        default: {}
    }
    if (i18n.find(exitCode) != i18n.end()) {
        std::cout << "[EE] (" << exitCode << ") " << i18n[exitCode] << "\n";
    } else {
        std::cout << "[UEE] Unexpected (just like spanish inquisition) error " << exitCode << "\n";
    }

    lastExitCode = exitCode;
    return true;
}

int main() {
    CommandsMap commands;
    I18N i18n;
    FileContext context;
    InputHandler handler(i18n);
    int lastExitCode = 0;

    init(i18n);
    init(commands);

    while (handleExitCode(run(context, commands, lastExitCode, handler), lastExitCode, i18n));

    return 0;
}
