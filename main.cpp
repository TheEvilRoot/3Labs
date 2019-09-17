#include <iostream>
#include <limits>

#include "complex_number.hpp"
#include "input.hpp"

int main() {
    ComplexNumber num(
        handleInput("Enter real: ", -std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
        handleInput("Enter imaginary: ", -std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
    ComplexNumber abc(
        handleInput("Enter real: ", -std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
        handleInput("Enter imaginary: ", -std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
    std::cout << num.getReal() << " " << num.getImaginary() << "\n";
    std::cout << abc.getReal() << " " << abc.getImaginary() << "\n";

    std::cout << "num == abc => " << (num == abc) << "\n";
    std::cout << "num != abc => " << (num != abc) << "\n";

    return 0;
}