#pragma once

class ComplexNumber {
private:
    float real;
    float im;
public:
    ComplexNumber(float real = 0, float im = 1);

    ComplexNumber(ComplexNumber &obj);

    ~ComplexNumber();

    float getReal();

    float getImaginary();

    ComplexNumber operator-();

    ComplexNumber operator+(ComplexNumber& other);

    ComplexNumber operator-(ComplexNumber& other);

    ComplexNumber operator*(ComplexNumber& other);

    ComplexNumber& operator++();

    ComplexNumber operator++(int);

    ComplexNumber& operator--();

    ComplexNumber operator--(int);

    friend bool operator== (const ComplexNumber &c1, const ComplexNumber &c2);
    friend bool operator!= (const ComplexNumber &c1, const ComplexNumber &c2);
};