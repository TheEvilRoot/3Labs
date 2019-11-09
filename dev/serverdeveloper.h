#pragma once

#include "nativedeveloper.h"
#include "api.h"

class ServerSoftwareDeveloper: virtual public NativeSoftwareDeveloper {
private:
  std::string serverSpeciality;
public:
  explicit ServerSoftwareDeveloper(const std::vector<std::string>& languages = { },
                                   std::string os = "", std::string spec = ""): NativeSoftwareDeveloper(languages, os) {
      serverSpeciality = std::move(spec);
  }
  ~ServerSoftwareDeveloper() = default;

  std::string getServerSpeciality() { return serverSpeciality; }

  std::string toString(bool title = true, bool onlyOwn = false) override {
      std::ostringstream stream;
      stream.setf(std::ios::boolalpha);

      if (title) stream << "ServerSoftwareDeveloper ";
      stream << NativeSoftwareDeveloper::toString(false, onlyOwn);
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