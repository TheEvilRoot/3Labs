#pragma once

#include "developer.h"
#include "api.h"

class NativeSoftwareDeveloper: virtual public Developer {
  std::string targetOS;
public:
  NativeSoftwareDeveloper(
          const std::vector<std::string>& languages = { },
          std::string os = ""): Developer(languages) {
      targetOS = os;
  }
  ~NativeSoftwareDeveloper() = default;

  std::string getTargetOS() { return targetOS; }

  std::string toString(bool title = true, bool onlyOwn = false) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if(title) stream << "NativeSoftwareDeveloper ";
      if (!onlyOwn) stream << Developer::toString(false);
      stream << "\n\t" << "target OS " << getTargetOS();

      return stream.str();
  }

  void input(InputHandler &handler, bool onlyOwn = false) {
      if (!onlyOwn) Developer::input(handler, onlyOwn);

      std::string inOS = handler.enterString("What operating system does the developer specialize in? ");
      targetOS = inOS;
      std::cout << "Your answer is " << targetOS << "\n";
  }
};