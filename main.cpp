#include <iostream>
#include <limits>
#include <vector>
#include "shapes.hpp"

const int ERR_SUCCEED = 0;
const int ERR_INVALID_VALUE = 1;
const int ERR_LOWER_BORDER_EXCEED = 2;
const int ERR_UPPER_BORDER_EXCEED = 3;

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
      std::cout << "[EE] (" << res.first << ") " << localize(res.first) << "\n";
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


int main(int argc, const char * argv[]) {
    std::vector<Polygon *> polygons;

    std::cout << "Enter square position: \n";
    int sx = handleInput<int>("\tx: ", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    int sy = handleInput<int>("\ty: ", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    auto edge = handleInput<float>("Enter square's edge length: ", 0, std::numeric_limits<float>::max());

    std::cout << "Enter circle position: \n";
    int cx = handleInput<int>("\tx: ", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    int cy = handleInput<int>("\ty: ", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    auto radius = handleInput<float>("Enter circle's radius: ", 0, std::numeric_limits<float>::max());

    polygons.push_back(new Square({sx, sy}, edge));
    polygons.push_back(new Circle({cx, cy}, radius));

    for (auto shape : polygons) {
    std::cout << shape->toString() << "\n";
    for (auto other : polygons) {
      if (other != shape) {
        try {
          auto intersects = shape->isIntersects(other);
          std::cout << "... " << (intersects ? "is " : "is not ") << "intercects with " << other->toString() << "\n";
        } catch(std::logic_error &err) { /* No-op */ }
      }
    }
    }
    return 0;
}
