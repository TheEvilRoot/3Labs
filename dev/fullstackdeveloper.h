#pragma once

#include "serverdeveloper.h"
#include "frontenddeveloper.h"
#include "api.h"

class FullStackDeveloper: public ServerSoftwareDeveloper, public FrontendDeveloper {
private:
  bool hasATeam;
public:
  FullStackDeveloper(
          bool team = false,
          const std::vector<std::string>& languages = { },
          std::string os = "",
          std::string spec = "",
          bool bootstrap = false,
          bool sass = false,
          bool webAsm = false,
          const std::vector<std::string>& libs = { }
  ): ServerSoftwareDeveloper(
          languages,
          std::move(os),
          spec), FrontendDeveloper(
          sass,
          bootstrap,
          libs,
          webAsm) {
      hasATeam = team;
  }

  bool hasTeam() { return hasATeam; }

  ~FullStackDeveloper() = default;

  std::string toString(bool title = true, bool onlyOwn = false) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if (title) stream << "FullStackDeveloper ";
      stream << ServerSoftwareDeveloper::toString(false);
      stream << FrontendDeveloper::toString(false, true);

      stream << "\n\thas team " << hasTeam();

      return stream.str();
  }

  void input(InputHandler &handler, bool onlyOwn = false) override {
      FrontendDeveloper::input(handler);
      ServerSoftwareDeveloper::input(handler, true);

      std::string inTeam = handler.enterString("Does the developer have his own team? (yes, no): ");
      hasATeam = inTeam == "yes" || inTeam == "true";
      std::cout << "Your answer is " << hasATeam << "\n";
  }
};