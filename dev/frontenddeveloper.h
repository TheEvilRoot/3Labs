#pragma once

#include "webdeveloper.h"
#include "api.h"

class FrontendDeveloper: virtual public WebDeveloper {
private:
  std::set<std::string> jsLibraries;
  bool knowWebAssembly;
public:
  explicit FrontendDeveloper(bool sass = false,
                             bool bootstrap = false,
                             const std::vector<std::string>& libs = { },
                             bool webAsm = false): WebDeveloper(sass, bootstrap) {
      for (const auto &l : libs) { addJsLibrary(l); }
      knowWebAssembly = webAsm;
      if (knowWebAssembly) addTechnique("web assembly");
  }

  ~FrontendDeveloper() = default;

  void addJsLibrary(const std::string& lib) { jsLibraries.insert(lib); }
  std::set<std::string> getLibraries() { return jsLibraries; }

  bool isKnowWebAssembly() { return knowWebAssembly; }

  std::string toString(bool title = true, bool onlyOwn = false) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if (title) stream << "FrontendDeveloper ";

      stream << WebDeveloper::toString(false, onlyOwn);
      stream << "\n\t" << "knowWebAssembly " << isKnowWebAssembly() << " Libraries [ ";

      for (const auto &l : getLibraries()) {
          stream << l << " ";
      }
      stream << "]";

      return stream.str();
  }

  void input(InputHandler& handler, bool onlyOwn = false) override {
      WebDeveloper::input(handler, onlyOwn);

      std::string inWA = handler.enterString("Do developer know Web Assembly? (yes, no): ");
      knowWebAssembly = inWA == "yes" || inWA == "true";
      std::cout << "Your answer is " << knowWebAssembly << "\n";

      std::string inLibs = handler.enterString("Enter JS libraries developer knows separated with comma (,): \n");
      std::vector<std::string> argsLibs = handler.splitString(inLibs, ',');

      for (const auto& l : argsLibs) {
          addJsLibrary(l);
          std::cout << "Added JS library: " << l << "\n";
      }
  }

};
