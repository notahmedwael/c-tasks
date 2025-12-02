#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>

class Stack {
private:
    int* arr;       // dynamic array allocated on heap
    int capacity;   // total size
    int topIndex;   // index of the top element

public:
    // Constructors
    Stack(int cap = 10);          // default + parameterized
    Stack(const Stack& other);    // deep copy constructor

    // Destructor
    ~Stack();

    // Copy assignment operator (deep copy)
    Stack& operator=(const Stack& other);

    // Basic Operations
    void push(int value);
    int pop();
    int top() const;
    bool isEmpty() const;
    bool isFull() const;

    // Utility
    int size() const;
    void print() const;
};

#endif
