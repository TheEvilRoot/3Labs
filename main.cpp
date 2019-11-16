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

char writeFile(InputHandler& handler, bool binary) {
    std::cout << "Enter new student's data: \n";
    Student st = Student::fromInputHandler(handler);

    std::cout << "Writing student \n" << st << "\n ... to " <<  (binary ? "binary" : "text") << " database\n";

    std::ofstream file(std::string("database.") + (binary ? "bin" : "txt"), (binary ? std::ios::binary : 0) | std::ios::app);

    if(!file.is_open()) {
        std::cout << "Unable to open file...\n";
        return 99;
    }

    if (binary)
        st.toBinaryFile(file);
    else st.toTextFile(file);

    std::cout << "Done.\n";

    return ERR_SUCCEED;
}

char reversedBinary(std::ifstream& file, InputHandler& handler) {
    Student st(0, "", "", 0);
    try {
         st = Student::fromBinary(file);
    } catch(std::range_error&) {
        return ERR_SUCCEED;
    }
    reversedBinary(file, handler);
    std::cout << st << "\n";
    return ERR_SUCCEED;
}

char reversedText(std::ifstream& file, InputHandler& handler) {
    // 1. Go to the end of the file skipping \n and \0 (-2 position from the end)
    int i = -2;
    file.seekg(i, std::ios::end);

    do {
        // 2. Go back until \n or until start of file (it fill be first (last for us) student)
        int c = 0;
        while (file.tellg() > 0 && (c = file.get()) != '\n') file.seekg(--i, std::ios::end);
        // 3. Set position
        file.seekg(i, std::ios::end);
        // 4. Read student
        Student st = Student::fromText(file, false);
        std::cout << st << "\n";

        // 5. Go back to 1 position from start of current student
        file.seekg(--i, std::ios::end);
        // Until reach start of the file
    } while (file.tellg() > 0);

    return ERR_SUCCEED;
}

char reversedTextBinary(std::ifstream& file, InputHandler& handler) {
    // 1. Go to the end of the file skipping \n and \0 (-2 position from the end)
    int i = -2;
    file.seekg(i, std::ios::end);

    do {
        // 2. Go back until \n or until start of file (it fill be first (last for us) student)
        int c = 0;
        int fix = 0;
        while (!(fix = !(file.tellg() > 0)) && (c = file.get()) != '\n') file.seekg(--i, std::ios::end);
        // 3. Set position with some magic
        file.seekg(i + 1 - fix, std::ios::end);
        // 4. Read student
        Student st = Student::fromText(file, true);
        std::cout << st << "\n";

        // 5. Go back to 1 position from start of current student
        file.seekg(--i, std::ios::end);
        // Until reach start of the file
    } while (file.tellg() > 0);
    return ERR_SUCCEED;
}

char readFile(InputHandler& handler, bool reversed, bool binary, const std::function<Student(std::ifstream&)>& fromFunction , char(*reverseFunction)(std::ifstream&, InputHandler&)) {
    std::ifstream file(std::string("database.") + (binary ? "bin" : "txt"), (binary ? std::ios::binary : std::ios::in));
    if(!file.is_open()) {
        std::cout << (binary ? "Binary" : "Text") << " database not exists yet or file not accessible\n";
        return ERR_SUCCEED;
    }
    if (!reversed) {
        while (!file.eof()) {
            try {
                Student st = fromFunction(file);
                std::cout << st << "\n";
            } catch(...) {
                break;
            }
        }
    } else {
        return reverseFunction(file, handler);
    }
    return ERR_SUCCEED;
}

void init(CommandsMap& commands) {
    cmd(commands, "help", "Display help message", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler){
        return ERR_SUCCEED;
    });

    cmd(commands, "exit", "Exit the program", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        return ERR_EXIT_CODE;
    });

    cmd(commands, "write", "Write new student to binary database. write -t|-b", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        char mode = 0;
        for (const auto& arg : args) {
            if (arg == "-t") mode = 't';
            if (arg == "-b") mode = 'b';
        }

        if (mode == 0) {
            std::cout << "Use -t to use text-based database and -b to binary database\n";
            return ERR_INVALID_ARGS;
        }

        return writeFile(handler, mode == 'b');
    });

    cmd(commands, "read", "Read database. read -t|-b|-B -r", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        bool reversed = false;
        char mode = 't';

        for (const auto& arg : args) {
            if (arg == "-r") reversed = true;
            if (arg == "-b") mode = 'b';
            if (arg == "-B") mode = 'B';
        }

        return readFile(handler, reversed, mode == 'b', [&](std::ifstream& file) {
           if (mode == 'b') return Student::fromBinary(file);
           return Student::fromText(file, mode == 'B');
        }, mode == 'b' ? reversedBinary : (mode == 'B' ? reversedTextBinary : reversedText));
    });

    cmd(commands, "drop", "Drop database -t|-b", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        char mode = 't';

        for (const auto& arg : args) {
          if (arg == "-t") mode = 't';
          if (arg == "-b") mode = 'b';
        }

        if (mode == 't') {
            remove("database.txt");
        } else {
            remove("database.bin");
        }
        std::cout << (mode == 't' ? "Text-based" : "Binary-based") << " database has been dropped\n";
        return ERR_SUCCEED;
    });

    cmd(commands, "code", "Get code of the character", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        for (const auto& arg : args) {
            for (const auto& c : arg) {
                std::cout << (int) c << " ";
            }
            std::cout << '\n';
        }
        return ERR_SUCCEED;
    });
    cmd(commands, "char", "Get char from code", [&](FileContext& context, std::vector<std::string>& args, InputHandler& handler) {
        for (const auto& arg : args) {
            int code = std::stoi(arg);
            std::cout << "'" << (char) code << "'\n";
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
