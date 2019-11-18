#include <iostream>

#include <map>
#include <functional>
#include <fstream>
#include <utility>
#include <string>

#include "api.h"
#include "train.h"

using namespace Railway;

struct Context {
  std::vector<Driver *> drivers;
  std::vector<Conductor *> conductors;
  std::vector<RailwayCarriage *> carriages;
  std::vector<Locomotive *> locomotives;
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
        return ERR_SUCCEED;
    });

    cmd(commands, "exit", "Exit the program", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) {
        return ERR_EXIT_CODE;
    });
    cmd(commands, "build", "Build the train", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) {
        if (context.drivers.empty()) {
            std::cout << "You don't have any drivers for train. Let's create a new one\n";
            context.drivers.push_back(new Driver(handler));
        }
        if (context.locomotives.empty()) {
            std::cout << "You don't have any locomotives. Let's create a new one\n";
            context.locomotives.push_back(new Locomotive(context.drivers, handler));
        }
        size_t carriagesCount = handler.handleInput("How much carriage hook to train? ", (size_t) 0, std::numeric_limits<size_t>::max());
        if (context.conductors.size() < carriagesCount) {
            std::cout << "You don't have so much conductors. Let's create: \n";
            for (size_t i = 0; i < carriagesCount - context.conductors.size() + 1; i++) {
                context.conductors.push_back(new Conductor(handler));
            }
        }
        if (context.carriages.size() < carriagesCount) {
            std::cout << "You don't have so much carriages. Let's create: \n";
            for (size_t i = 0; i < carriagesCount - context.carriages.size() + 1; i++) {
                std::cout << "Which conductor should we use for this carriage: \n";
                for (const auto& c : context.conductors) {
                    std::cout << "\t" << c->toString() << "\n";
                }
                size_t index = handler.enterIndexOf("(0-" + std::to_string(context.conductors.size() - 1) + ")> ", context.conductors);
                context.carriages.push_back(new RailwayCarriage(context.conductors[index]));
            }
        }

        auto *train = new Train(context.locomotives[0], context.carriages);

        std::cout << "\tYour train:\n" << train->toString() << "\n";

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
