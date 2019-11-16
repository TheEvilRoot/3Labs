#pragma once

#include <utility>
#include <vector>
#include <string>

#include <fstream>

class Student {
private:
  size_t id;
  std::string name;
  std::string faculty;
  size_t semester;

public:
  Student(size_t id, const std::string &name, const std::string &faculty, size_t semester) {
      this->id = id;
      this->name = name;
      this->faculty = faculty;
      this->semester = semester;
  }

  size_t binaryLength() {
      return
              sizeof(id) +
              sizeof(std::string::size_type) +
              name.size() +
              sizeof(std::string::size_type) +
              faculty.size() +
              sizeof(semester);
  }

  template<typename T>
  void varToBytes(char *bytes, T *var, size_t &pointer) {
      if (!bytes || !var) return;

      memcpy(bytes + pointer, var, sizeof(*var));
      pointer += sizeof(*var);
  }

  void toBinaryFile(std::ofstream &file) {
      // TODO: Check is binary
      if (!file.is_open()) throw std::logic_error("File not opened");

      size_t length = binaryLength();
      char *bytes = new char[length];

      size_t pointer = 0;
      varToBytes(bytes, &id, pointer);

      size_t nameLength = name.size();
      size_t facLength = faculty.size();

      varToBytes(bytes, &nameLength, pointer);
      const char *nameBytes = name.c_str();
      memcpy(bytes + pointer, nameBytes, nameLength);
      pointer += nameLength;

      varToBytes(bytes, &facLength, pointer);
      const char *facBytes = faculty.c_str();
      memcpy(bytes + pointer, facBytes, facLength);
      pointer += facLength;

      varToBytes(bytes, &semester, pointer);

      file.write(bytes, length);
  }

  static Student fromBinary(std::ifstream &file) {
      // TODO: Check is binary
      if (!file.is_open()) throw std::logic_error("File not opened");

      size_t id = 0;
      file.read((char *) &id, sizeof(id));
      if (file.eof()) throw std::range_error("Unexpected eof while reading student");

      size_t nameLength = 0;
      file.read((char *) &nameLength, sizeof(size_t));
      if (file.eof()) throw std::range_error("Unexpected eof while reading student");

      char *nameBytes = new char[nameLength];
      file.read(nameBytes, nameLength);
      if (file.eof()) throw std::range_error("Unexpected eof while reading student");

      size_t facLength = 0;
      file.read((char *) &facLength, sizeof(size_t));
      if (file.eof()) throw std::range_error("Unexpected eof while reading student");

      char *facBytes = new char[facLength];
      file.read(facBytes, facLength);
      if (file.eof()) throw std::range_error("Unexpected eof while reading student");

      size_t semester = 0;
      file.read((char *) &semester, sizeof(semester));

      return Student(id, std::string(nameBytes) + '\0', std::string(facBytes) + '\0', semester);
  }

  void toTextFile(std::ofstream &file) {
      // TODO: Check is text
      if (!file.is_open()) throw std::logic_error("File not opened");

      file << id;
      file << "\t";

      file << name;
      file << "\t";

      file << faculty;
      file << "\t";

      file << semester;
      file << "\n";
  }

  friend std::ostream& operator<<(std::ostream& out, Student &student) {
      out << "Student [ " << student.id << " " << student.name << " on faculty " << student.faculty << " semester " << student.semester << " ]";
      return out;
  }

  static Student fromText(std::ifstream &file, bool binary = false) {
      // TODO: Check is text
      if (!file.is_open()) throw std::logic_error("File not opened");

      if (binary)
          return fromTextBinary(file);
      return fromTextText(file);
  }

  static Student fromInputHandler(InputHandler &handler) {
      size_t id = handler.handleInput("\tID: ",(size_t) 0,
                                      std::numeric_limits<size_t>::max());
      std::string name = handler.enterString("\tName:");
      std::string faculty = handler.enterString("\tFaculty: ");
      size_t semester = handler.handleInput("\tSemester: ", 1, 8);

      return Student(id, name, faculty, semester);
  }

private:
  static bool isNumber(int c) { return c >= '0' && c <= '9'; }

  template<typename T>
  static T reverseNumber(T number) {
      T reversed = 0;
      while (number > 0) {
          reversed = (reversed * 10) + (number % 10);
          number /= 10;
      }
      return reversed;
  }

  static Student fromTextBinary(std::ifstream &file) {
      /** We should read : **/
      /** [...id...]\t[...name...]\t[...faculty...]\t[...semester...]\n **/

      size_t id = 0;
      int c;
      while ((c = file.get()) != '\t') {
          if (c < 0 || !isNumber(c)) throw std::logic_error("Invalid text file notation while reading id: " + std::to_string((char) c));
          id = (id * 10) + (c - '0'); // Aggressive char->int transformation
      }

      std::string name;
      while ((c = file.get()) != '\t') {
          if (c < 0) throw std::logic_error("Invalid text file notation while reading name");
          name += (char) c;
      }

      std::string fac;
      while ((c = file.get()) != '\t') {
          if (c < 0) throw std::logic_error("Invalid text file notation while reading faculty");
          fac += (char) c;
      }

      size_t semester = 0;
      while ((c = file.get()) != '\n') {
          if (c < 0 || !isNumber(c)) throw std::logic_error("Invalid text file notation while reading semester");
          semester = (semester * 10) + (c - '0');
      }

      return Student(id, name, fac, semester);
  }

  static Student fromTextText(std::ifstream &file) {
      size_t id = 0;
      file >> id;

      file.get();

      char c = 0;
      std::string name;
      while ((c = file.get()) != '\t') {
          if (c < 0) throw std::logic_error("Invalid text file notation while reading name");
          name += (char) c;
      }

      std::string fac;
      while((c = file.get()) != '\t') {
          if (c < 0) throw std::logic_error("Invalid text file notation while reading faculty");
          fac += (char) c;
      }

      size_t semester = 0;
      file >> semester;

      file.get();

      return Student(id, name, fac, semester);
  }
};