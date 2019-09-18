#include "complex_number.hpp"

#include <iostream>

ComplexNumber::ComplexNumber(float real, float im) {
    this->real = real;
    this->im = im;
    std::cout << "[D] Complex Number default constructor has called " << real << " " << im << "\n";
}

ComplexNumber::ComplexNumber(const ComplexNumber &obj) : real(obj.real), im(obj.im) {}

ComplexNumber::~ComplexNumber() {
    std::cout << "[D] Complex Number destructor has called " << real << " " << im << "\n";
}

ComplexNumber& ComplexNumber::operator=(const ComplexNumber& that) noexcept {
    this->im = that.im;
    this->real = that.real;
    return *this;
}

float ComplexNumber::getReal() {
    return real;
}

float ComplexNumber::getImaginary() {
    return im;
}

ComplexNumber ComplexNumber::operator-() {
    ComplexNumber n(-this->real, -this->im);
    return n;
}

ComplexNumber ComplexNumber::operator+(ComplexNumber& other) {
    ComplexNumber n(this->real + other.real, this->im + other.getImaginary());
    return n;
}

ComplexNumber ComplexNumber::operator-(ComplexNumber& other) {
    ComplexNumber n(this->real - other.real, this->im - other.getImaginary());
    return n;
}

ComplexNumber ComplexNumber::operator*(ComplexNumber& other) {
    ComplexNumber n((this->real * other.real) - (this->im * other.im), (this->real * other.im) + (other.real * this->im));
    return n;
}

ComplexNumber& ComplexNumber::operator++()
{
    this->real++;
    return *this;
}

ComplexNumber ComplexNumber::operator++(int) {
    ComplexNumber temp = *this;
    ++*this;
    return temp;
}

ComplexNumber& ComplexNumber::operator--() {
    this->real--;
    return *this;
}

ComplexNumber ComplexNumber::operator--(int) {
    ComplexNumber temp = *this;
    --*this;
    return temp;
}

bool operator== (const ComplexNumber &c1, const ComplexNumber &c2) {
    return c1.real == c2.real && c1.im == c2.im;
}

bool operator!= (const ComplexNumber &c1, const ComplexNumber &c2) {
    return !(c1 == c2);
}

std::ostream& operator<<(std::ostream &out, ComplexNumber &num) {
    out << num.getReal() << (num.getImaginary() < 0 ? " - " : " + ") << (num.getImaginary() < 0 ? -num.getImaginary() : num.getImaginary()) << "i";
    return out;
}
