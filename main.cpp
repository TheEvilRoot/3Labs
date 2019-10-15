#include <iostream>
#include <limits>
#include <cmath>
#include <ctime>

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

template<typename T>
void sort(T *array, size_t size, int (*comparator)(T*,T*)) {
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size - 1 - i; j++) {
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

int main() {
  Vector *vectors = new Vector[10];
  srand(static_cast<unsigned int>(time(nullptr)));
  for (int i = 0; i < 10; i++) {
    vectors[i] = {rand() % 100, rand() % 20};
  }
  
  for (int i = 0; i < 10; i++) {
    std::cout << vectors[i].mag() << "\n";
  }
  
  std::cout << "Sorting...\n";
  
  sort(vectors, 10,(int(*)(Vector*, Vector*))([](Vector* t, Vector* v) -> int {
    if (t == nullptr && v == nullptr) return 0;
    if (t == nullptr) return -1;
    if (v == nullptr) return 1;
    return t->mag() - v->mag();
  }));
  
  for (int i = 0; i < 10; i++) {
    std::cout << vectors[i].mag() << "\n";
  }
  return 0;
}

