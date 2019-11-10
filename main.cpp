#include <iostream>

#include <map>
#include <functional>
#include <utility>
#include <string>

#include "api.h"

template<class T>
class Array {
private:
  std::vector<T> data;  // Live is too hard to use pointer based arrays
                        // for `terminated` and `unexpected` hooks demonstration
public:
  explicit Array(size_t initialSize = 0) {
      data.resize(initialSize);
  }

  T& operator[](size_t index) throw() {
      if (index >= data.size()) throw std::range_error(std::to_string(index) + " >= " + std::to_string(data.size()));

      return data[index];
  }

  size_t push(T t) {
      data.push_back(t);
      return data.size() - 1;
  }

  size_t getSize() {
      return data.size();
  }
};

struct Context {
    Array<std::string> array;
};

void unexpectedHook() {
    std::cerr << "\n[UNEXPECTED] Unexpected exception was thrown.\n";
    std::terminate();
}

void terminationHook() {
    std::cerr << "\n[TERMINATION] Unexpected error occurred. Terminating...\n";
    exit(1);
}

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

    cmd(commands, "push", "Push new element to array", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) {
        std::string newElement;
        if (args.empty()) {
            newElement = handler.enterString("Enter new element string: ");
        } else {
            newElement = args[0];
        }

        std::cout << "New element added to position " << context.array.push(newElement) << "\n";
        return ERR_SUCCEED;
    });

    cmd(commands, "get", "Get element from array by index", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) -> char{
        if (args.empty()) {
            std::cout << "You should provide index to get element\n";
            return ERR_INVALID_ARGS;
        }

        size_t index = std::atol(args[0].c_str());
        std::cout << "Getting " << index << " element...";

        std::string el = context.array[index];
        std::cout << " " << el << "\n";
        return ERR_SUCCEED;
    });

    cmd (commands, "size", "Get size of the array", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) {
        if (context.array.getSize() == 0){
            std::cout << "Array is empty\n";
        } else {
            std::cout << "Size of the array: " << context.array.getSize() << "\n";
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
    std::set_unexpected(unexpectedHook);
    std::set_terminate(terminationHook);

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
