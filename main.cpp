#include <iostream>
#include <limits>
#include <cmath>
#include <ctime>
#include <vector>
#include <map>

const int ERR_SUCCEED = 1;
const int ERR_INVALID_VALUE = 2;
const int ERR_LOWER_BORDER_EXCEED = 3;
const int ERR_UPPER_BORDER_EXCEED = 4;

const int ERR_INVALID_ARGUMENTS = 5;
const int ERR_NO_SUCH_CMD = 6;
const int ERR_NO_VECTORS = 7;

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
T handleInput(const char *message, T lower, T upper, std::map<const int, std::string> localizations) {
  std::pair<char, T> res;

  while (true) {
    res = enter(message, lower, upper);
    if (res.first != ERR_SUCCEED) {
      std::cout << "[EE] (" << res.first << ") " << localizations[res.first] << "\n";
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

template<typename T>
void sort(T *array, size_t size, int (*comparator)(T*,T*)) {
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size - 1; j++) {
      if (comparator(&array[j], &array[j + 1]) > 0) {
        std::swap(array[j], array[j + 1]);
      }
    }
  }
}

struct Vector {
  int x;
  int y;

  float mag() {
    return sqrt(x * x + y * y);
  }
};

int vectorCompare(Vector *a, Vector *b) {
  if (!a && !b) return 0;
  if (!a) return -1;
  if (!b) return 1;
  return a->mag() - b->mag();
}

std::vector<std::string> splitString(std::string s, std::string delimiter = " ") {
  size_t pos = 0;
  std::string token;
  std::vector<std::string> result;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    result.push_back(token);
    s.erase(0, pos + delimiter.length());
  }
  result.push_back(s);
  return result;
}

std::string trim(const std::string& str)
{
  size_t first = str.find_first_not_of(' ');
  if (std::string::npos == first) return str;

  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

int parseInt(std::string& str, bool isSigned = true) {
  int res = 0;
  bool sign = 0;
  if (str.length() == 0) return res;
  int i = 0;
  if (str[i] == '-' || str[i] == '+') sign = str[i++] == '-';
  for (size_t j = i; j < str.length(); j++) {
    char c = str[j];
    if (std::isdigit(c)) res = (res * 10) + c - '0';
    else if (c == '\0') break;
    else throw j;
  }
  return res * (sign ? -1 : 1);
}

void setupI18n(std::map<const int, std::string>& i18n) {
  i18n.insert(std::pair<const int, std::string>(ERR_SUCCEED, "tesh: Succeed"));
  i18n.insert(std::pair<const int, std::string>(ERR_INVALID_VALUE, "tesh: Value you entered is invalid, sorry."));
  i18n.insert(std::pair<const int, std::string>(ERR_LOWER_BORDER_EXCEED, "tesh: Value should be higher"));
  i18n.insert(std::pair<const int, std::string>(ERR_UPPER_BORDER_EXCEED, "tesh: Value should be lower"));
  i18n.insert(std::pair<const int, std::string>(ERR_NO_SUCH_CMD, "tesh: Command not found"));
  i18n.insert(std::pair<const int, std::string>(ERR_INVALID_ARGUMENTS, "tesh: Command arguments is invalid. Enter help to get manual"));
  i18n.insert(std::pair<const int, std::string>(ERR_NO_VECTORS, "tesh: Vector list is empty"));
}

int helpCmd(std::vector<std::string> &args, std::vector<Vector>& vectors) {
  std::cout << "Available commands for theevilshell\n";
  std::cout << "\thelp - get this message\n";
  std::cout << "\tadd <x> <y> - add new vector to the list. Accepts two parameters: x(number), y(number)\n";
  std::cout << "\tprint (index) - print vectors array. If index is passed, prints only selected vector\n";
  std::cout << "\texit - exit session\n";

  return 0;
}

int addCmd(std::vector<std::string> &args, std::vector<Vector>& vectors) {
  if (args.size() < 3) {
    return ERR_INVALID_ARGUMENTS;
  }
  int x = 0;
  int y = 0;
  try {
    x = parseInt(args[1]);
    y = parseInt(args[2]);
  } catch(size_t idx) {
    return ERR_INVALID_VALUE;
  }

  vectors.push_back({x, y});
  return 0;
}

int printCmd(std::vector<std::string> &args, std::vector<Vector>& vectors) {
  if (vectors.size() == 0) {
    return ERR_NO_VECTORS;
  }
  int index = 0;
  try {
    if (args.size() < 2) throw 1;
    index = parseInt(args[1]);
    if (index < 0) index = vectors.size() + index;
    if (index >= vectors.size()) throw 2;

    auto vec = vectors[index];
    std::cout << "(" << vec.x << ", " << vec.y << ") = " << vec.mag() << "\n";
  } catch(...) {
    for (auto vec : vectors) {
      std::cout << "(" << vec.x << ", " << vec.y << ") = " << vec.mag() << "\n";
    }
  }
  return 0;
}

int sortCmd(std::vector<std::string> &args, std::vector<Vector>& vectors) {
  if (vectors.empty()) {
    return ERR_NO_VECTORS;
  }
  sort(vectors.data(), vectors.size(), &vectorCompare);
  return 0;
}

int exitCmd(std::vector<std::string> &args, std::vector<Vector>& vectors) {
  return 255;
}

typedef int(*Callback)(std::vector<std::string>&, std::vector<Vector>&);

int main() {
  std::map<const int, std::string> i18n;
  setupI18n(i18n);

  std::map<std::string, Callback> commands;
  commands.insert(std::pair<std::string, Callback>("help", &helpCmd));
  commands.insert(std::pair<std::string, Callback>("add", &addCmd));
  commands.insert(std::pair<std::string, Callback>("print", &printCmd));
  commands.insert(std::pair<std::string, Callback>("sort", &sortCmd));
  commands.insert(std::pair<std::string, Callback>("exit", &exitCmd));

  std::vector<Vector> vectors;
  std::cout << "Welcome to theevilshell\n";
  while (1) {
    auto inputString = enterString("tesh > ");
    auto trimmed = trim(inputString);
    if (trimmed.length() == 0) continue;

    auto args = splitString(trimmed);
    if (args.empty()) continue;

    if (commands.find(args[0]) == commands.end()) {
      auto localizedMessage = i18n[ERR_NO_SUCH_CMD];
      std::cout << localizedMessage << ": " << args[0] << "\n";
      continue;
    }

    auto command = commands[args[0]];
    auto exitCode = command(args, vectors);

    if (exitCode == 255) break;

    if (i18n.find(exitCode) != i18n.end()) {
      std::cout << i18n[exitCode] << "\n";
    } else if (exitCode != 0) {
      std::cout << "Command exit with exit code " << exitCode << "\n";
    }
  }
  return 0;
}