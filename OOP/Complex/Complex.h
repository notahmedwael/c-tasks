#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>

class Complex {
private:
    double real;
    double imag;

public:
    // Constructors
    Complex();
    Complex(double r);
    Complex(double r, double i);

    // Getters
    double getReal() const;
    double getImag() const;

    // Copy Assignment
    Complex& operator=(const Complex& other);

    // Arithmetic Operators
    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const Complex& other) const;

    Complex& operator+=(const Complex& other);
    Complex& operator-=(const Complex& other);
    Complex& operator*=(const Complex& other);
    Complex& operator/=(const Complex& other);

    // Unary Operators
    Complex operator+() const;
    Complex operator-() const;

    // Comparison Operators
    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;

    // Increment / Decrement
    Complex& operator++();       // prefix
    Complex operator++(int);     // postfix
    Complex& operator--();       // prefix
    Complex operator--(int);     // postfix

    // Special Operators
    double& operator[](int index);           // 0 = real, 1 = imag
    double operator()() const;               // magnitude

    // Stream Operators
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    friend std::istream& operator>>(std::istream& is, Complex& c);
};

#endif
