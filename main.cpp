#include <iostream>

#include <map>
#include <functional>
#include <utility>
#include <string>

#include "api.h"
#include "employee.h"

struct Context {
    std::vector<Employee *> employees;
};

typedef std::function<const char(Context&, std::vector<std::string>&, InputHandler&)> CommandCallback;

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
    cmd(commands, "help", "Display help message", [&](Context& context, std::vector<std::string>& args, InputHandler& handler){
        std::cout << "Help: \n";
        for (const auto& [cmdName, cmd] : commands) {
            std::cout << "\t" << cmdName << " - " << cmd.description << "\n";
        }
        return ERR_SUCCEED;
    });

    cmd(commands, "exit", "Exit the program", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) {
        return ERR_EXIT_CODE;
    });
    cmd(commands, "add", "Add employee", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) -> char {
        if (args.empty()) {
            std::cout << "You should pass type of employee: programmer, designer or tester\n";
            return ERR_INVALID_ARGS;
        }

        auto type = args[0];
        Employee *employee;
        if (type == "programmer") {
            employee = new Programmer(handler);
        } else if (type == "designer") {
            employee = new Designer(handler);
        } else if (type == "tester") {
            employee = new Tester(handler);
        } else {
            std::cout << "Invalid type: " << type << ". Use programmer, designer or tester to add one\n";
            return ERR_INVALID_ARGS;
        }

        context.employees.push_back(employee);
        return ERR_SUCCEED;
    });
    cmd(commands, "list", "Print all employees", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) -> char {
        if (context.employees.empty()) {
            std::cout << "There's no employees. What we gonna do now? (⊙_◎)\n";
        } else {
            std::cout << "Employees: \n";
            for (const auto& emp: context.employees) {
                std::cout << "\t" << emp->toString() << "\n";
            }
        }
        return ERR_SUCCEED;
    });
    cmd(commands, "work", "Make some employee do their jobs", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) -> char {
        if (context.employees.empty()) {
          std::cout << "There's no employees. What we gonna do now? (⊙_◎)\n";
          return ERR_SUCCEED;
        }

        for (size_t i = 0; i < context.employees.size(); i++) {
            std::cout << i << ". " << context.employees[i]->toString() << "\n";
        }

        size_t index = handler.handleInput(std::string("Choose employee to do job (0-") + std::to_string(context.employees.size() - 1) + "): ", (size_t) 0, context.employees.size() - 1);
        context.employees[index]->doJob();
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

char run(Context& context, CommandsMap& commands, int lastExitCode, InputHandler& handler) {
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
    Context context;
    InputHandler handler(i18n);
    int lastExitCode = 0;

    init(i18n);
    init(commands);

    while (handleExitCode(run(context, commands, lastExitCode, handler), lastExitCode, i18n));

    return 0;
}
