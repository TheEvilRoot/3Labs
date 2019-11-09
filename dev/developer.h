#pragma once

#include <vector>
#include <string>
#include <set>
#include <sstream>

#include "api.h"

class Developer {
private:
  std::set<std::string> techniques;
public:
  explicit Developer(const std::vector<std::string>& tech = { }) {
      for (const auto &t : tech) { addTechnique(t); }
  }
  ~Developer() = default;

  void addTechnique(const std::string& t) { techniques.insert(t); }
  std::set<std::string> getTechniques() { return techniques; }

  virtual std::string toString(bool title = true, bool onlyOwn = false) {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if (title) stream << "Developer";

      stream << "[ ";
      for (const auto &tech : techniques) {
          stream << tech << " ";
      }
      stream << "]";
      return stream.str();
  }

  virtual void input(InputHandler& handler, bool onlyOwn = false) {
      std::cout << "Enter developer data:\n";
      std::string inTechniques = handler.enterString("Enter developer's techniques separated with comma (,): \n");
      std::vector<std::string> argsTechniques = handler.splitString(inTechniques, ',');

      for (const auto& t : argsTechniques) {
          addTechnique(t);
          std::cout << "Added technique: " << t << "\n";
      }
  }
};

