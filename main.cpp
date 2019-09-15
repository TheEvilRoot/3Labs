#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

class MyArray {
public:
    static int GLOBAL_ID;
    int id;
    char *pointer;
    int length;

    MyArray() {
        this->id = GLOBAL_ID++;
        std::cout << "[DB] MyArray " << id << " initialization\n";
        this->pointer = NULL;
        this->length = 0;
    }

    ~MyArray() {
        std::cout << "[DB] MyArray " << id << " deinitialization\n";
        delete[] this->pointer;
    }

    void push(char data) {
        std::cout << "[DB] MyArray " << id << " pushing " << data << "\n";
        this->pointer = (char*) realloc(this->pointer, ++(this->length));
        this->pointer[length - 1] = data;
    }

    char & operator[](int index) {
        std::cout << "[DB] MyArray " << id << " getting " << index << "\n";
         if (index < 0 || index >= length) {
             throw std::out_of_range("");
         }
         return pointer[index];
      }

    MyArray intersections(MyArray &with) {
        MyArray newArr;
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < with.length; j++) {
                if (pointer[i] == with[j]) {
                    newArr.push(pointer[i]);
                }
            }
        }
        return newArr;
    }

    friend std::ostream & operator << (std::ostream &out, const MyArray &arr);

};

int MyArray::GLOBAL_ID = 0;

std::ostream & operator << (std::ostream &out, const MyArray &arr)
{
    for (int i = 0; i < arr.length; i++) {
        std::cout << arr.pointer[i] << " ";
    }
    return out;
}


int main() {
    MyArray arr;
    arr.push('a');
    arr.push('b');

    std::cout << "ARR: " << arr << "\n";

    MyArray brr;
    brr.push('b');
    brr.push('c');

    std::cout << "BRR: " << brr << "\n";

    MyArray crr = arr.intersections(brr);

    std::cout << "CRR: " << crr << "\n";

    return 0;
}

