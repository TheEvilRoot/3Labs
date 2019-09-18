#include <iostream>
#include <limits>
#include <ncurses>

#include "complex_number.hpp"
#include "input.hpp"

ComplexNumber enterComplexNumber() {
    float r = handleInput("Enter real: ", -std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max());
    float i = handleInput("Enter imaginary: ", -std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max());

    ComplexNumber ret(r, i);
    return ret;
}

int main() {

    getmaxyx(stdscr, 100, 100);

    ComplexNumber num(enterComplexNumber());
    ComplexNumber abc(enterComplexNumber());

    std::cout << "z = " << num << "\n";
    std::cout << "Z = " << abc << "\n";

    std::cout << "num == abc => " << (num == abc) << "\n";
    std::cout << "num != abc => " << (num != abc) << "\n";
    return 0;
}