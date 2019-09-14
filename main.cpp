#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

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
      std::cout << "[EE] (" << (int) res.first << ") " << localize(res.first) << "\n";
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

class Student {
private:
  int id;
  std::string name;
  float rate;
public:
  static int GLOBAL_ID;

  Student(std::string name, float rate) {
    this->name = name;
    this->rate = rate;

    this->id = GLOBAL_ID++;
  }

  Student() {
    this->id = GLOBAL_ID++;
    this->name = enterString("Enter student's name: ");
    this->rate = handleInput<float>("Enter student's rate[0-10]: ", std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
  }

  int getID() {
    return id;
  }

  std::string getName() {
    return name;
  }

  friend void printStudent(Student &st);
  friend class Dean;
};

class Dean {
public:
  void  changeRate(Student &st, float newRate) {
    st.rate = newRate;
  }
};

void printStudent(Student &st) {
  std::cout << "Student [" << st.id << "] " << st.name << " has rate " << st.rate << std::endl;
}

int Student::GLOBAL_ID = 0;

bool askContinue() {
  return handleInput("Enter another student? (0 or 1)", 0, 1) == 1;
}

const char ERR_INPUT_EMPTY = 4;
const char ERR_NO_SUCH_CMD = 5;
const char ERR_INVALID_ARGS = 6;

void showHelp() {
    std::cout
            << "Help: \n"
            << "edit <student id> - to edit student's rate\n"
            << "list - to get list of students\n"
            << "id <student id> - to get info about specific student\n"
            << "exit - to exit the program\n";
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

char handleMenuInput(std::string str) {
    if (str.size() == 0) return ERR_INPUT_EMPTY;

    auto args = splitString(str, ' ');

    if (args[0] == "?") {
        showHelp();
    } else if (args[0] == "edit") {
        if (args.size() < 2) {
            return ERR_INVALID_ARGS;
        }
        int id;
        try {
            id = std::stoi(args[1]);
        } catch(std::invalid_argument) {
            return ERR_INVALID_ARGS;
        }

        //editStudent()

    }
}

int main() {
 std::vector<Student> students;

  do {
    students.push_back(Student());
  } while (askContinue());

  std::cout << "Got " << students.size() << " students." << std::endl;

  Dean d;
  do {
      auto inputString = enterString("(? to get help) > ");

      switch(handleMenuInput(inputString)) {

      }
    // std::cout << "Enter ID of student to change rate:" << std::endl;
    // for (auto st : students)  {
    //   std::cout << "[" << st.getID() << "] " << st.getName() << std::endl;
    // }
    // int cId = handleInput<int>("# ", 0, students.size() - 1);
    // printStudent(students[cId]);
    // float newRate = handleInput<float>("Enter new rate: ", 0, 10);
    // d.changeRate(students[cId], newRate);
    // std::cout << "Rate changed." << std::endl;
    // printStudent(students[cId]);
  } while (true);

 return 0;
}


