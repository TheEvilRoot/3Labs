#include <iostream>

#include "dev/developer.h"

#include <map>
#include <functional>

#include "dev/developer.h"
#include "dev/webdeveloper.h"
#include "dev/nativedeveloper.h"
#include "dev/serverdeveloper.h"
#include "dev/frontenddeveloper.h"
#include "dev/fullstackdeveloper.h"
#include "dev/api.h"

typedef std::vector<Developer *> Developers;
struct Command {
  std::function<const char(Developers&, std::vector<std::string>&, InputHandler&)> func;
  std::string description;
};
typedef std::map<std::string, Command> CommandsMap;

bool isValidDeveloperType(std::string type) {
    return
        type == "developer" ||
        type == "webdeveloper" ||
        type == "frontenddeveloper" ||
        type == "nativedeveloper" ||
        type == "serverdeveloper" ||
        type == "fullstackdeveloper";
}

void printValidDeveloperTypes() {
    std::cout << "Valid types: \n";
    std::cout << "\twebdeveloper\n";
    std::cout << "\tfrontenddeveloper\n";
    std::cout << "\tserverdeveloper\n";
    std::cout << "\tnativedeveloper\n";
    std::cout << "\tfullstackdeveloper\n";
    std::cout << "\tdeveloper\n";
}

void init(CommandsMap& commands) {
    commands.insert({ "help" , { [&](Developers& developer, std::vector<std::string>& args, InputHandler& handler) {
        std::cout << "Help:\n";
        for (auto [cmdName, cmd] : commands) {
            std::cout << "\t" << cmdName << " - " << cmd.description << "\n";
        }
        return ERR_SUCCEED;
    }, "Display help message" } });

    commands.insert({ "exit" , { [&](Developers&, std::vector<std::string>&, InputHandler& handler) {
        std::cout << "See you later calculator!\n";
      return ERR_EXIT_CODE;
    }, "Exit the program" } });

    commands.insert( { "add", { [&](Developers& developers, std::vector<std::string>& args, InputHandler& handler) -> char {
        if (args.size() > 0) {
            auto type = args[0];
            if (type == "developer") {
                developers.push_back(new Developer());
            } else if (type == "webdeveloper") {
                developers.push_back(new WebDeveloper());
            } else if (type == "frontenddeveloper") {
                developers.push_back(new FrontendDeveloper());
;           } else if (type == "nativedeveloper") {
                developers.push_back(new NativeSoftwareDeveloper());
            } else if (type == "serverdeveloper") {
                developers.push_back(new ServerSoftwareDeveloper());
            } else if (type == "fullstackdeveloper") {
                developers.push_back(new FullStackDeveloper());
            } else {
                std::cout << "Type " << type << " is not valid developer type.\n";
                printValidDeveloperTypes();
                return ERR_INVALID_ARGS;
            }
            developers[developers.size() - 1]->input(handler);
            return ERR_SUCCEED;
        } else {
            std::cout << "Not enough arguments. Please, specify type of developer to add.\n";
            printValidDeveloperTypes();
            return ERR_INVALID_ARGS;
        }
    }, "Add a new developer" } });

    commands.insert({ "list", { [&](Developers& developers, std::vector<std::string>&, InputHandler&) {
        if (developers.empty()) {
            std::cout << "Your developers list is empty now. Enter add <type> to add new developer\n";
        } else {
            for (const auto &dev : developers) {
                std::cout << dev->toString() << "\n\n";
            }
            std::cout << "Total " << developers.size() << " developers\n";
        }
        return ERR_SUCCEED;
    }, "Print all developers" } });

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

char run(Developers& developers, CommandsMap& commands, int lastExitCode, InputHandler& handler) {
    std::string inString = handler.enterString("[" + std::to_string(lastExitCode) + "] # ");
    if (inString.length() ==  0) return ERR_INPUT_EMPTY;
    std::vector<std::string> tokens = handler.splitString(inString, ' ');

    if (commands.find(tokens[0]) == commands.end()) {
        return ERR_NO_SUCH_CMD;
    }

    auto command = commands[tokens[0]];
    tokens.erase(tokens.begin());

    return command.func(developers, tokens, handler);
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

    std::cout.setf(std::ios::boolalpha);

    Developers developers = { };
    I18N i18n;
    CommandsMap commands;
    InputHandler handler(i18n);

    init(i18n);
    init(commands);

    int lastExitCode = 0;
    while (handleExitCode(
            run(developers, commands, lastExitCode, handler),
            lastExitCode,
            i18n
            ));
    return 0;
}
