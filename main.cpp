#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

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

class MyArray {
public:
    static int GLOBAL_ID;
    int id;
    char *pointer;
    int length;

    MyArray() {
        this->id = GLOBAL_ID++;
        std::cout << "[DB] MyArray " << id << " initialization\n";
        this->pointer = NULL;
        this->length = 0;
    }

    ~MyArray() {
        std::cout << "[DB] MyArray " << id << " deinitialization\n";
        delete[] this->pointer;
    }

    void push(char data) {
        std::cout << "[DB] MyArray " << id << " pushing " << data << "\n";
        this->pointer = (char*) realloc(this->pointer, ++(this->length));
        this->pointer[length - 1] = data;
    }

    char & operator[](int index) {
        std::cout << "[DB] MyArray " << id << " getting " << index << "\n";
         if (index < 0 || index >= length) {
             throw std::out_of_range("");
         }
         return pointer[index];
      }

    MyArray intersections(MyArray &with) {
        MyArray newArr;
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < with.length; j++) {
                if (pointer[i] == with[j]) {
                    newArr.push(pointer[i]);
                }
            }
        }
        return newArr;
    }

    friend std::ostream & operator << (std::ostream &out, const MyArray &arr);

};

int MyArray::GLOBAL_ID = 0;

std::ostream & operator << (std::ostream &out, const MyArray &arr)
{
    for (int i = 0; i < arr.length; i++) {
        std::cout << arr.pointer[i] << " ";
    }
    return out;
}


int main() {
    MyArray arr;
    arr.push('a');
    arr.push('b');

    std::cout << "ARR: " << arr << "\n";

    MyArray brr;
    brr.push('b');
    brr.push('c');

    std::cout << "BRR: " << brr << "\n";

    MyArray crr = arr.intersections(brr);

    std::cout << "CRR: " << crr << "\n";

    return 0;
}

