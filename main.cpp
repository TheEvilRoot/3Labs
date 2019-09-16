#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
#include <string>

// Input error constants
const int ERR_SUCCEED = 0;
const int ERR_INVALID_VALUE = 1;
const int ERR_LOWER_BORDER_EXCEED = 2;
const int ERR_UPPER_BORDER_EXCEED = 3;

// Shell error constants
const char ERR_INPUT_EMPTY = 4;
const char ERR_NO_SUCH_CMD = 5;
const char ERR_INVALID_ARGS = 6;
const char ERR_INVALID_ID = 7;
const char ERR_EXIT_CODE = 8;

const char * localizeError(char error) {
  switch (error) {
    case ERR_SUCCEED:
      return "Succeed";
    case ERR_INVALID_VALUE:
      return "Invalid value";
    case ERR_UPPER_BORDER_EXCEED:
      return "Upper border exceeded";
    case ERR_LOWER_BORDER_EXCEED:
      return "Lower border exceeded";
    default:
      return "Unexpected error";
  }
}

template <typename T>
std::pair<char, T> enter(const char *message, T lower, T upper) {
  T value;
  char error = ERR_SUCCEED;
  std::cout << message;
  auto isFailed = !(std::cin >> value);

  if (isFailed) {
    if (std::cin.fail()) {
      std::cin.clear();
    }
    error = ERR_INVALID_VALUE;
  } else {
    if (value < lower) {
      error = ERR_LOWER_BORDER_EXCEED;
    }
    if (value > upper) {
      error = ERR_UPPER_BORDER_EXCEED;
    }
  }
  std::cin.ignore(std::numeric_limits<int>::max(), '\n');

  return std::pair<char, T>(error, value);
}

template <typename T>
T handleInput(const char *message, T lower, T upper, const char * (*localize)(char) = &localizeError) {
    std::pair<char, T> res;

    while (true) {
        res = enter(message, lower, upper);
        if (res.first != ERR_SUCCEED) {
            std::cout << "[EE] (" << (int) res.first << ") " << localize(res.first) << "\n";
        } else break;
    }
    return res.second;
}

std::string enterString(const char *message, bool includeWhitespaces = true) {
    std::string string;
    char c;

    std::cout << message;
    while ((c = std::cin.get())) {
        if (c == '\n') break;

        if (!includeWhitespaces && c == ' ') {
            std::cin.ignore(std::numeric_limits<int>::max(), '\n');
            break;
        }
        string += c;
    }

    return string;
}

class Student {
private:
    int id;
    std::string name;
    float rate;
public:
    static int GLOBAL_ID;

    Student(std::string name, float rate) {
        this->name = name;
        this->rate = rate;

        this->id = GLOBAL_ID++;
    }

    Student() {
        this->id = GLOBAL_ID++;
        this->name = enterString("Enter student's name: ");
        this->rate = handleInput<float>("Enter student's rate[0-10]: ", 0, 10);
    }

    int getID() {
        return id;
    }

    std::string getName() {
        return name;
    }

  friend void printStudent(Student &st);
  friend class Dean;
};

class Dean {
public:
    void  changeRate(Student &st, float newRate) {
        st.rate = newRate;
    }
};

void printStudent(Student &st) {
    std::cout << "Student [" << st.id << "] " << st.name << " has rate " << st.rate << std::endl;
}

int Student::GLOBAL_ID = 0;

bool askContinue(const char *message) {
    auto inp = enterString(message);
    if (inp.empty()) return false;
    if (inp == "yes" || inp[0] == 'y') return true;
    return false;
}

void showHelp() {
    std::cout
            << "Help: \n\n"
            << "\tedit <student id> <new rate: optional> - to edit student's rate\n"
            << "\tnew <count: optional, default 1> - to add a new student\n"
            << "\tlist - to get list of students\n"
            << "\tid <student id> - to get info about specific student\n"
            << "\texit - to exit the program\n";
}

std::vector<std::string> splitString(std::string source, char sep) {
    std::vector<std::string> ret;
    std::string::size_type pos = 0, prevPos = 0;

    while ((pos = source.find(sep, pos)) != std::string::npos) {
        std::string substring(source.substr(prevPos, pos - prevPos));
        ret.push_back(substring);
        prevPos = ++pos;
    }
    ret.push_back(source.substr(prevPos, pos - prevPos));
    return ret;
}

void editStudent(Student &st, Dean &dean) {
    float newRate = handleInput<float>("Enter new rate: ", 0, 10);
    dean.changeRate(st, newRate);
    std::cout << "Rate changed." << std::endl;
    printStudent(st);
}

int parseInt(std::string str) {
    try {
        return std::stoi(str);
    } catch(std::invalid_argument) {
        return -1;
    }
}

float parseFloat(std::string str) {
    try {
        return std::stof(str);
    } catch(std::invalid_argument) {
        return -1;
    }
}

char handleMenu(std::string str, std::vector<Student> &students, Dean &dean) {
    if (str.size() == 0) return ERR_INPUT_EMPTY;

    auto args = splitString(str, ' ');

    if (args[0] == "?") {
        showHelp();
    } else if (args[0] == "edit") {
        if (args.size() < 2) {
            return ERR_INVALID_ARGS;
        }

        int id = parseInt(args[1]);
        if (id < 0) return ERR_INVALID_ARGS;
        if (id < 0 || id >= Student::GLOBAL_ID) return ERR_INVALID_ID;

        float newRate = -1;
        if (args.size() >= 3) {
            if ((newRate = parseFloat(args[2])) >= 0 && newRate <= 10) {
                dean.changeRate(students[id], newRate);
                std::cout << "Rate changed." << std::endl;
                printStudent(students[id]);
                return ERR_SUCCEED;
            } else {
                std::cout << "[EE] " << args[2] << " is invalid rate.\n";
            }
        }
        editStudent(students[id], dean);
    } else if (args[0] == "list") {
        for (auto st : students) {
            printStudent(st);
        }
    } else if (args[0] == "id") {
        if (args.size() < 2) {
            return ERR_INVALID_ARGS;
        }

        int id = parseInt(args[1]);
        if (id < 0) return ERR_INVALID_ARGS;
        if (id < 0 || id >= Student::GLOBAL_ID) return ERR_INVALID_ID;

        printStudent(students[id]);
    } else if (args[0] == "new") {
        int count = 1;
        if (args.size() > 1) {
            count = parseInt(args[1]);
            if (count < 0 || (count > 10 && !askContinue("Really? That's a lot! Maybe 1? (yes[y] to continue): "))) count = 1;
        }
        for (; count > 0; count--) {
            students.push_back(Student());
        }
    } else if (args[0] == "exit") {
        std::cout << "Goodby!" << std::endl;
        return ERR_EXIT_CODE;
    } else {
        return ERR_NO_SUCH_CMD;
    }

    return ERR_SUCCEED;
}

int main() {
    std::vector<Student> students;
    Dean dean;
    do {
        auto inputString = enterString("(? to get help) > ");
        switch(handleMenu(inputString, students, dean)) {
            case ERR_SUCCEED: { continue; break; }
            case ERR_EXIT_CODE: return 0;
            case ERR_INPUT_EMPTY: {
                // std::cout << "Input is empty. Use ? to get help\n";
                break;
            }
            case ERR_INVALID_ARGS: {
                std::cout << "[EE] Invalid arguments passed. Enter ? to get help with commands\n";
                break;
            }
            case ERR_INVALID_ID: {
                std::cout << "[EE] Student with such ID not found. Minimum ID is 0, maximum - " << Student::GLOBAL_ID - 1 << std::endl;
                break;
            }
            case ERR_NO_SUCH_CMD: {
                std::cout << "[EE] No such command.\n";
                break;
            }
        }
    } while (true);
    return 0;
}