#pragma once

#include <string>
#include <functional>
#include <thread>
#include <chrono>
#include <utility>
#include <random>
#include <iostream>

class Employee {
protected:
  std::string name;
public:
  explicit Employee(std::string  name): name(std::move(name)) {}
  explicit Employee(InputHandler& handler) {
      this->name = handler.enterString("Enter employee's name: ");
  }
  virtual ~Employee() = default;

  virtual void doJob() = 0;
  virtual std::string toString() = 0;
};

class Programmer: public Employee {
private:
  std::string level;
  static std::vector<std::string> stuffToDo;
public:
  explicit Programmer(const std::string& name, std::string  level): Employee(name), level(std::move(level)) { }
  explicit Programmer(InputHandler& handler): Employee(handler) {
      this->level = handler.enterString("Enter programmer's level: ");
  }
  ~Programmer() override = default;

  void doJob() override {
      std::cout << "Programmer gets to work\n";
      for (int i = 0; i < 4; i++) {
          std::cout << stuffToDo[random() % stuffToDo.size()] << "...\n";
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
      std::cout << "Done!\n";
  }

  std::string toString() override {
      return "Programmer (" + level + ") " + name;
  }

  void refactorCode() {
      std::cout << "\tRefactoring... Didn't work...\n";
      std::cout << "\tgit reset --hard\n";
  }
};

class Designer: public Employee {
private:
  static std::string icon;
public:
  explicit Designer(std::string name): Employee(std::move(name)) {}
  explicit Designer(InputHandler& handler): Employee(handler) {}
  ~Designer() override = default;

  void doJob() override {
      std::cout << "Designer gets to work\n";
      std::cout << "Designer has no mood for getting work...\n";
  }

  std::string toString() override {
      return "Designer " + name;
  }

  void createApplicationIcon() {
      std::cout << icon << "\n";
  }
};

class Tester: public Employee {
public:
  explicit Tester(std::string name): Employee(std::move(name)) {}
  explicit Tester(InputHandler& handler): Employee(handler) {}
  ~Tester() override = default;

  void doJob() override {
      std::cout << "I clicked something and everything was gone\n";
  }

  std::string toString() override {
      return "Tester " + name;
  }

  void testProduct() {
      std::cout << "Not working!!!\n";
  }

};

std::vector<std::string> Programmer::stuffToDo = {
        "Refactoring legacy code",
        "Reinstalling windows",
        "Finding semicolon",
        "Fixing bugs",
        "Committing to github",
        "Waiting for make install",
        "Reading StackOverFlow",
        "Googling",
        "Arguing with testing department",
        "Transforming bug into feature"
};

std::string Designer::icon = " @@@@@@@@@@@@@@@@@@&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      \n"
                             " @@@@@@@@@@@@&&&%#####&%###%&@@@@@@@@@@@@@@@@@@@@@@@@@@                      \n"
                             " @@@@@@@@@########%#######%##&@@@@@@@@@@@@@@@@@@@@@@@@@                      \n"
                             " @@@@&%%%####%#########%########%&@@@@@@@@@@@@@@@@@@@@@                      \n"
                             " @@@&%#%#######################%%##%&@@@@@@@@@@@@@@@@@@@@                      \n"
                             " @@&&%#%%###%#########%%%############&@@@@@@@@@@@@@@@@@@@                      \n"
                             " &%%%#########%#%#########%##########&@@@@@@@@@@@@@@@@@@@                      \n"
                             " %%#####%%#############%#%#####%%##%@@@@@@@@@@@@@&%@@@@@@                      \n"
                             " &%%###%#########%%######((((((##%&&@@@@@@@@@@@@%/%@@@@@@                      \n"
                             " &%%###############((((##(,....***#@@@@@@@@@@#/%%@@@@@@                      \n"
                             " &%%#########%#((((#*.........../@@@@&@@@@@@@%///%(%@@@@@                      \n"
                             " @&%#%###%###((#*.....,...(@@@@,,/&/(*%@@@@@%////(%@@@@@@                      \n"
                             " @@@&&&%#####/*....,%@@,..@@&*.*#((&@%*/&@@%////(%@@@@@@@                      \n"
                             " @@@@@&%%%&%**#@@///(@&/..,%(//(*(@@@..**%(////(%@@@@@@@@                      \n"
                             " @@@@@@@@@&(@(#@#.*(%((@..../@@@(.......**#//((&@@@@@@@@@                      \n"
                             " @@@@@@@@@%/*@#(,//((%@(..,%%............*/#(%@@@@@@@@@@@@@@@@@@@@@@@&@@@@@@@\n"
                             " @@@@@@@@@%/**&@&@@(.....*...............,*(%@@@@@@@@@@@@&,@@&@@@&&@@&&@@&&@@@@\n"
                             " @@@@@@@@@%/**............................**%@&&&&&&&@&&@&&@@&&@@&&&@&&@@&&@@@@\n"
                             " @@@@@@@@@#/**......#(%....................*/&@@@&&@@@&&@@&&@&&&@@&&@@&@@&&&@@@\n"
                             " @@@@@@@@@((**.....%(%.....................,*%@@@&&@@@@&&&&&@@&&@@@&@@&&&&&&&@@\n"
                             " @@@@@@@@%/%**...*%//(*((((**/(,......,.....*/@@@@&@@@@@&&&&@@&&@@@@@@@@@@@@@@@\n"
                             " @@@@@@@%/#%*,..%////#*(/(*/(* .((,.........**%@@@&&@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"
                             " @@@@@&(/////../////#(*(,  /.,  ,,**,...,...**%@@@@@@@@@@@@@@@@@@@@@@@@@@@@/@@@\n"
                             " @@@@%((//////////(%,(*..  (*/,(,.***.......**%@@@@@@@@@@@@@&@@&@@@&&@@&&@@&&@@\n"
                             " @@@%(//%*%((///(%*.,,(,(**(*(  */*(*/......**%@@&&&&@&&@@&@@&&@@@&/@&&@@&&@@\n"
                             " @@%(//%**...*(,(((/(///(//(/(*/((((((......**%@@@@&&@@@&&@@&&@&&&&@&&@@&@@&&@@\n"
                             " @%(//%**.......*/**(**.///(./**,**(*/.....,*/@@@@@&&@@@@&&&&&@@&&@@@&@@&&&&&&&\n"
                             " @%(///*......,./,*,,,/**../,(*/*/,* /.....**%@@@@@@&@@@@@@&&&@@&&@@@@@@@@@@@@@\n"
                             "  #(/(**........./**,/*..../ ****,(,(.....**%@@@@@@@&&                         \n"
                             "  %(//#*,.........,***(.,/.//*/,***,....,**%@@@@@@@@@@                         \n"
                             "  @%(/(**,...........((((((((((((......**%@@@@@@@@@@@@                         \n"
                             "  @@%(/((**.........................,**%@@@@@@@@@@@@@@                         \n"
                             "  @@@@%(/%***,...................,***%@@@@@@@@@@@@@@@@                         \n"
                             "  @@@@@@@%#%(***,..,..........****%&@@@@@@@@@@@@@@@@@@                         \n"
                             "  @@@@@@@@@@@&%%/***********/%%#((%@@@@@@@@@@@@@@@@@@@                         \n"
                             "  @@@@@@@@@@@@@%////(%@@@@@%////(%@@@@@@@@@@@@@@@@@@@@                         \n"
                             "  @@@@@@@@@@@@@@%////#@@@@@////%%@@@@@@@@@@@@@@@@@@@@@                         \n"
                             "  @@@@@@@@@@@@@&////(%@@@@@%////(%@@@@@@@@@@@@@@@@@@@@                         \n"
                             "  @@@@@@@@@@@@@%//(%@@@@@@@@@%%/(%@@@@@@@@@@@@@@@@@@@@                         ";