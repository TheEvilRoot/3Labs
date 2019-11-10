#include <iostream>

#include <map>
#include <functional>
#include <fstream>
#include <utility>
#include <string>

#include "api.h"

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

bool writeToOutFile(const std::string& content) {
    std::ofstream file("out.txt", std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "Failed to open output file (out.txt)\n";
        return false;
    }

    file << content;
    file.close();
    return true;
}

void init(CommandsMap& commands) {
    cmd(commands, "help", "Display help message", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler){
        return ERR_SUCCEED;
    });

    cmd(commands, "exit", "Exit the program", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        return ERR_EXIT_CODE;
    });

    cmd(commands, "open", "Open file", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) -> char{
        if (context.file.is_open()) {
            std::cout << "File " << context.fileName << " already opened. Use 'close' command to close file.\n";
            return ERR_SUCCEED;
        }

        if (args.empty()) {
            std::cout << "You should provide filename to open file\n";
            std::cout << "open <file>\n";
            return ERR_INVALID_ARGS;
        }

        auto fileName = args[0];
        context.file.open(fileName, std::fstream::in);

        if (!context.file.is_open()) {
            std::cout << "Failed to open file " << fileName << "\n";
            return ERR_SUCCEED;
        }

        std::cout << "File opened!\n";
        context.file.seekg(0, std::fstream::end);
        std::cout << "File length: " << context.file.tellg() << "\n";
        context.file.seekg(0, std::fstream::beg);

        return ERR_SUCCEED;
    });

    cmd (commands, "close", "Close file", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) -> char {
        if (!context.file.is_open()) {
            std::cout << "File not opened yet\n";
            return ERR_SUCCEED;
        }

        context.file.close();
        context.fileName = "";

        std::cout << "File closed\n";
        return ERR_SUCCEED;
    });

    cmd (commands, "count", "Count specified symbols in file", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) -> char {
        if (!context.file.is_open()) {
          std::cout << "File not opened yet\n";
          return ERR_SUCCEED;
        }

        char charOfInterest = 0;
        if (args.empty()) {
            auto str = handler.enterString("Enter symbol to count in file: ");
            if (str.empty()) return ERR_INVALID_ARGS;
            charOfInterest = str[0];
        } else {
            charOfInterest = args[0][0];
        }
        // asserts charOfInterest is not 0

        context.file.clear();
        context.file.seekg(0, std::fstream::beg);
        size_t count = 0;
        while (!context.file.eof()) {
            int i = 0;
            if ((i = context.file.get()) < 0) { break; }
            if (i  == charOfInterest) count++;
        }

        std::cout << "Symbol '" << charOfInterest << "' appears in file " << count << " times\n";

        if (writeToOutFile(std::to_string(count))) {
          std::cout << "Result successfully stored to out.txt\n";
        }

        return ERR_SUCCEED;
    });

    cmd (commands, "words", "Count words in file", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) -> char {
      if (!context.file.is_open()) {
          std::cout << "File not opened yet\n";
          return ERR_SUCCEED;
      }

      context.file.clear();
      context.file.seekg(0, std::fstream::beg);

      size_t count = 0;
      while (!context.file.eof()) {
          int i = 0;
          if ((i = context.file.get()) < 0) { break; }
          if (!((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z') || (i >= '0' &&  i <= '9'))) count++;
      }

      std::cout << "In file " << count + 1 << " words\n";

      if (writeToOutFile(std::to_string(count + 1))) {
          std::cout << "Result successfully stored to out.txt\n";
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
