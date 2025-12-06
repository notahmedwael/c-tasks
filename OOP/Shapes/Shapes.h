#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdexcept>

class Shape
{
public:
    virtual ~Shape() = default; // virtual destructor as it's going to be inherited
    virtual double area() const = 0; // pure virtual function
    virtual std::string typeOfShape() const = 0;
    virtual void display() const = 0; // display dimensions and area
};

class Triangle : public Shape {
private:
    double base, height;

public:
    Triangle(double b, double h) : base(b), height(h) {
        if (base <= 0 || height <= 0) {
            throw std::invalid_argument("Base and height can only be positive values");
        }
    }

    double area() const override {
        return 0.5 * base * height;
    }

    std::string typeOfShape() const override { return "Triangle"; }

    void display() const override {
        std::cout << "Triangle: base = " << base << " ,height = " << height
                  << " ,area = " << area() << "\n";
    }
};

class Rectangle : public Shape {
protected:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("Width and height can only be positive values");
        }
    }

    double area() const override {
        return width * height;
    }

    std::string typeOfShape() const override { return "Rectangle"; }

    void display() const override {
        std::cout << "Rectangle: width = " << width << " ,height = " << height
                  << " ,area = " << area() << "\n";
    }
};

class Square : public Rectangle {
public:
    Square(double s) : Rectangle(s, s) {
        if (s <= 0) {
            throw std::invalid_argument("Square side can only be a positive value");
        }
    }

    double area() const override {
        return width * width; // width == height
    }

    std::string typeOfShape() const override { return "Square"; }

    void display() const override {
        std::cout << "Square: side = " << width << " ,area = " << area() << "\n";
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {
        if (r <= 0) {
            throw std::invalid_argument("Circle radius can only be a positive value");
        }
    }

    double area() const override {
        return M_PI * radius * radius;
    }

    std::string typeOfShape() const override { return "Circle"; }

    void display() const override {
        std::cout << "Circle: radius = " << radius << " ,area = " << area() << "\n";
    }
};
