#include "Complex.h"
#include <cmath>

// Constructors
Complex::Complex() : real(0), imag(0) {}
Complex::Complex(double r) : real(r), imag(0) {}
Complex::Complex(double r, double i) : real(r), imag(i) {}

// Getters
double Complex::getReal() const { return real; }
double Complex::getImag() const { return imag; }

// Copy Assignment
Complex& Complex::operator=(const Complex& other) {
    if (this != &other) {
        real = other.real;
        imag = other.imag;
    }
    return *this;
}

// Arithmetic Operators

// + operator
Complex Complex::operator+(const Complex& other) const { return Complex(real + other.real, imag + other.imag); }

// - operator
Complex Complex::operator-(const Complex& other) const { return Complex(real - other.real, imag - other.imag); }

// * operator
Complex Complex::operator*(const Complex& other) const {
    return Complex(real * other.real - imag * other.imag,
                   real * other.imag + imag * other.real);
}

// / operator
Complex Complex::operator/(const Complex& other) const {
    double denom = other.real * other.real + other.imag * other.imag;
    return Complex((real * other.real + imag * other.imag) / denom,
                   (imag * other.real - real * other.imag) / denom);
}

// += operator
Complex& Complex::operator+=(const Complex& other) { real += other.real; imag += other.imag; return *this; }

// -= operator
Complex& Complex::operator-=(const Complex& other) { real -= other.real; imag -= other.imag; return *this; }

// *= operator
Complex& Complex::operator*=(const Complex& other) {
    double r = real * other.real - imag * other.imag;
    double i = real * other.imag + imag * other.real;
    real = r; imag = i; return *this;
}

// /= operator
Complex& Complex::operator/=(const Complex& other) {
    double denom = other.real * other.real + other.imag * other.imag;
    double r = (real * other.real + imag * other.imag) / denom;
    double i = (imag * other.real - real * other.imag) / denom;
    real = r; imag = i;
    return *this;
}

// Unary Operators

// + operator just returns same thing
Complex Complex::operator+() const { return *this; }

// - operator returns the same thing but negates both parts
Complex Complex::operator-() const { return Complex(-real, -imag); }

// Comparison Operators

// Equality Operator
bool Complex::operator==(const Complex& other) const { return real == other.real && imag == other.imag; }

// Non equality operator
bool Complex::operator!=(const Complex& other) const { return !(*this == other); }

// Increment / Decrement
Complex& Complex::operator++() { ++real; ++imag; return *this; }
Complex Complex::operator++(int) { Complex temp = *this; ++(*this); return temp; }
Complex& Complex::operator--() { --real; --imag; return *this; }
Complex Complex::operator--(int) { Complex temp = *this; --(*this); return temp; }

// Special Operators

// Returns real part or imaginary part only, a[0] -> real, a[1] -> imag
double& Complex::operator[](int index) {
    if (index == 0) return real;
    else if (index == 1) return imag;
    else throw std::out_of_range("Index must be 0 (real) or 1 (imag)");
}

// Returns the magnitude of the complex number
double Complex::operator()() const {
    return std::sqrt(real*real + imag*imag);
}

// Stream Operators

// Output stream
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.real << " + " << c.imag << "j";
    return os;
}

// Input stream
std::istream& operator>>(std::istream& is, Complex& c) {
    is >> c.real >> c.imag;
    return is;
}
