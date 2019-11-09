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

  virtual std::string toString(bool title = true) {
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

  std::string toString(bool title = true) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if (title) stream << "WebDeveloper ";

      stream << Developer::toString(false);
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

  std::string toString(bool title = true) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if (title) stream << "FrontendDeveloper ";

      stream << WebDeveloper::toString(false);
      stream << "\n\t" << "knowWebAssembly" << isKnowWebAssembly() << " Libraries [ ";

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

  std::string toString(bool title = true) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if(title) stream << "NativeSoftwareDeveloper ";

      stream << Developer::toString(false);
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

class ServerSoftwareDeveloper: virtual public NativeSoftwareDeveloper {
private:
  std::string serverSpeciality;
public:
  ServerSoftwareDeveloper(const std::vector<std::string>& languages = { },
          std::string os = "", std::string spec = ""): NativeSoftwareDeveloper(languages, os) {
      serverSpeciality = spec;
  }
  ~ServerSoftwareDeveloper() = default;

  std::string getServerSpeciality() { return serverSpeciality; }

  std::string toString(bool title = true) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if (title) stream << "NativeSoftwareDeveloper ";

      stream << NativeSoftwareDeveloper::toString(false);
      stream << "\n\t" << "server speciality " << getServerSpeciality();

      return stream.str();
  }

  void input(InputHandler &handler, bool onlyOwn = false) override {
      NativeSoftwareDeveloper::input(handler, onlyOwn);

      std::string inSS = handler.enterString("What type of server software does the developer specialize in? ");
      serverSpeciality = inSS;
      std::cout << "Your answer is " << serverSpeciality << "\n";
  }

};

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

  std::string toString(bool title = true) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if (title) stream << "FullStackDeveloper ";
      stream << ServerSoftwareDeveloper::toString(false) << "\n";

      if (title) stream << "FullStackDeveloper ";
      stream << FrontendDeveloper::toString(false);

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