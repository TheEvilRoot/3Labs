#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include "array.hpp"


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

bool help(Array &arr) {
    std::cout << ":: Help message:\n";
    std::cout << "\thelp - print this message\n";
    std::cout << "\tadd - add new item to array\n";
    std::cout << "\tprint - print array\n";
    std::cout << "\tsize - print size of the array\n";
    std::cout << "\talloc_size - print allocated size of the array\n";
    std::cout << "\tsort - sort the array\n";
    std::cout << "\tfunc - find product of Xi where Xi > Xmin and Xi < Xmax or Xi > Xmax and Xi < Xmin for every i in array\n";
    std::cout << "\texit - exit the program\n";
    return false;
}

bool add(Array &arr) {
    int newNum = handleInput<int>("Enter a new number: ", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    arr.push(newNum);
    std::cout << "::Number " << newNum << " added to the array\n";
    return false;
}

bool print(Array &arr) {
    if (arr.lastIndex == 0) {
        std::cout << "::Your array is empty\n";
    } else {
        for (int i = 0; i < arr.lastIndex; i++) {
            std::cout << arr[i];
            if (i < arr.lastIndex - 1) std::cout << ", ";
        }
        std::cout << '\n';
    }
    return false;
}

bool size(Array &arr) {
    std::cout << "::Size of your array is " << arr.lastIndex << "\n";
    return false;
}

bool allocSize(Array &arr) {
    std::cout << "::Your array is allocated to " << arr.lastIndex << " elements.\n";
    return false;
}

bool sort(Array &arr) {
    if(arr.lastIndex == 0) {
        std::cout << "::Your array is empty. There's nothing to sort\n";
    } else {
        arr.sort();
        std::cout << "::Sorted!\n";
    }
    return false;
}

bool func(Array &arr) {
    if(arr.lastIndex == 0) {
        std::cout << "::Your array is empty.\n";
        return false;
    } else if (arr.lastIndex == 1) {
        std::cout << "::You have only one element. That does not make any sense\n";
        return false;
    }
    size_t iMin = 0;
    size_t iMax = 0;
    for (int i = 0; i < arr.lastIndex; i++) {
        if (arr[i] < arr[iMin]) iMin = i;
        if (arr[i] > arr[iMax]) iMax = i;
    }

    if (iMin == iMax) {
        std::cout << "::You have max and min elements on the same position. It can means that all emenents of array is equals!\n";
        return false;
    }
    int delta = iMin - iMax;
    if (delta == 1 || delta == -1) {
        std::cout << "::Maximum and minimum values located nearby\n";
        return 0;
    }

    std::cout << "::Minimum index is " << iMin << ". Maximim index is " << iMax << "\n";

    if (iMax < iMin) {
        std::swap(iMax, iMin);
    }

    int product = arr[iMin + 1];
    for (int i = iMin + 2; i < iMax; i++) {
        product *= arr[i];
    }

    std::cout << "::Product equals " << product << "\n";
    return false;

}

bool exit(Array &arr) {
    std::cout << "::Goodby!\n";
    return true;
}

int main() {
    Array arr;
    std::map<std::string, bool(*)(Array&)> menuFunctions;
    menuFunctions.insert(std::pair<std::string, bool(*)(Array&)>("help", &help));
    menuFunctions.insert(std::pair<std::string, bool(*)(Array&)>("add", &add));
    menuFunctions.insert(std::pair<std::string, bool(*)(Array&)>("print", &print));
    menuFunctions.insert(std::pair<std::string, bool(*)(Array&)>("size", &size));
    menuFunctions.insert(std::pair<std::string, bool(*)(Array&)>("alloc_size", &allocSize));
    menuFunctions.insert(std::pair<std::string, bool(*)(Array&)>("sort", &sort));
    menuFunctions.insert(std::pair<std::string, bool(*)(Array&)>("func", &func));
    menuFunctions.insert(std::pair<std::string, bool(*)(Array&)>("exit", &exit));
    while (true) {
        std::string inp = enterString("> ", true);
        if (menuFunctions.find(inp) == menuFunctions.end()) {
            std::cout << ":: Error. No such command: " << inp << "\n";
            continue;
        }
        if (menuFunctions[inp](arr)) break;
    }
    return 0;
}