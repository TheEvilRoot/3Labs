#pragma once

#include "developer.h"

class WebDeveloper: virtual public Developer {
private:
  bool knowSass;
  bool knowBootstrap;
public:
  explicit WebDeveloper(bool sass = false, bool bootstrap = false): Developer({ "html", "css" }) {
      knowSass = sass;
      if (knowSass) addTechnique("sass");
      knowBootstrap = bootstrap;
      if (knowBootstrap) addTechnique("bootstrap");
  }
  ~WebDeveloper() = default;

  bool isKnowSass() { return knowSass; }
  bool isKnowBootstrap() { return knowBootstrap; }

  std::string toString(bool title = true, bool onlyOwn = false) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if (title) stream << "WebDeveloper ";
      if (!onlyOwn) stream << Developer::toString(false);
      stream << "\n\t" << "knowSass " << isKnowSass() << " knowBootstrap " << isKnowBootstrap();

      return stream.str();
  }

  void input(InputHandler &handler, bool onlyOwn = false) override {
      if (!onlyOwn) Developer::input(handler);
      std::string inSass = handler.enterString("Do developer know SASS? (yes, no): ");
      knowSass = inSass == "yes" || inSass == "true";
      std::cout << "Your answer is " << knowSass << "\n";

      std::string inBS = handler.enterString("Do developer know Bootstrap? (yes, no): ");
      knowBootstrap = inBS == "yes" || inBS == "true";
      std::cout << "Your answer is " << knowBootstrap << "\n";
  }
};