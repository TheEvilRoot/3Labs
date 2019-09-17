#include "input.hpp"
#include <limits>

const char * localizeError(char error) {
  switch (error) {
    case ERR_SUCCEED:
      return "Успешно";
    case ERR_INVALID_VALUE:
      return "Неверное значение";
    case ERR_UPPER_BORDER_EXCEED:
      return "Число слишком большое";
    case ERR_LOWER_BORDER_EXCEED:
      return "Число слишком маленькое";
    default:
      return "Непредвиденная ошибка";
  }
}
template <class T>
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
        std::cout << "value " << value << "lower " << lower << "\n";
        error = ERR_LOWER_BORDER_EXCEED;
    }
    if (value > upper) {
      error = ERR_UPPER_BORDER_EXCEED;
    }
  }
  std::cin.ignore(std::numeric_limits<int>::max(), '\n');

  return std::pair<char, T>(error, value);
}

template <class T>
T handleInput(const char *message, T lower, T upper, const char * (*localize)(char)) {
  std::pair<char, T> res;

  while (true) {
    res = enter(message, lower, upper);
    if (res.first != ERR_SUCCEED) {
      std::cout << "[EE] (" << res.first << ") " << localize(res.first) << "\n";
    } else break;
  }
  return res.second;
}

std::string enterString(const char *message, bool includeWhitespaces) {
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

template int handleInput<int>(const char *message, int lower, int upper, const char * (*localize)(char));
template float handleInput<float>(const char *message, float lower, float upper, const char * (*localize)(char));