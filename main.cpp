#include <iostream>

#include <map>
#include <functional>
#include <utility>
#include <string>

#include "api.h"

class StackUnderflow: public std::exception { };

template<class T>
struct StackNode {
  StackNode<T> *next;
  T data;
};

template<class T>
class Stack {
private:
  StackNode<T> *top;
  size_t size;

public:
  Stack(): top(nullptr), size(0) { }

  void push(T t) {
      auto *newTop = new StackNode<T>;
      newTop->next = top;
      newTop->data = t;
      top = newTop;
      size++;
  }

  T pop() {
    if (top == nullptr) throw StackUnderflow();

    T ret = top->data;
    top = top->next;
    size--;

    return ret;
  }

  bool empty() {
      return size == 0;
  }

};

struct Context {
    Stack<std::string> stack;
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

    cmd(commands, "push", "Push value to stack", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) {
        std::string newElement;
        if (args.empty()) {
            newElement = handler.enterString("Enter new element to push to the stack: ");
        } else {
            newElement = args[0];
        }

        context.stack.push(newElement);
        std::cout << "Pushed\n";
        return ERR_SUCCEED;
    });

    cmd (commands, "pop", "Pop value from stack", [&](Context& context, std::vector<std::string>& args, InputHandler& handler) {
        try {
            std::cout << "Element popped " << context.stack.pop() << "\n";
        } catch (StackUnderflow &suf) {
            std::cout << "Stack is empty!\n";
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
