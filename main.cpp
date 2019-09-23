#include <iostream>
#include <limits>

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
    std::cout << "Enter first: \n";
    ComplexNumber num(enterComplexNumber());
    std::cout << "Enter second: \n";
    ComplexNumber abc(enterComplexNumber());

    std::cout << "first == second => " << ((num == abc) ? "yes" : "no") << "\n";

    ComplexNumber delta(num - abc);
    std::cout << "first - second = " << (delta) << "\n";

    ComplexNumber sum(num + abc);
    std::cout << "first + second = " << (sum) << "\n";

    ComplexNumber mul(num * abc);
    std::cout << "first * second = " << (mul) << "\n";

    return 0;
}