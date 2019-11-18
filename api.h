#pragma once

#include <vector>
#include <map>
#include <string>

#include <iostream>

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


typedef std::map<const char, std::string> I18N;

class InputHandler {
private:
  I18N& i18n;
public:
  explicit InputHandler(I18N& i18n): i18n(i18n) {}

  template<typename T>
  std::pair<char, T> enter(const std::string& message, T lower, T upper) {
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

  template<typename T>
  T handleInput(const std::string& message, T lower, T upper) {
      std::pair<char, T> res;

      while (true) {
          res = enter(message, lower, upper);
          if (res.first != ERR_SUCCEED) {
              std::cout << "[EE] (" << (int) res.first << ") " << i18n[res.first] << "\n";
          } else break;
      }
      return res.second;
  }

  std::string enterString(const std::string &message, bool includeWhitespaces = true) {
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

  template<typename T>
  size_t enterIndexOf(const std::string& message, std::vector<T>& from) {
      return handleInput(message, (size_t) 0, from.size() - 1);
  }

};