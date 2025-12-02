#ifndef STACK_CPP
#define STACK_CPP

#include "Stack.h"

// Constructor
template <typename T>
Stack<T>::Stack(int cap) : capacity(cap), topIndex(-1) {
    arr = new T[capacity];  // allocate on heap
}

// Deep Copy Constructor
template <typename T>
Stack<T>::Stack(const Stack& other)
    : capacity(other.capacity), topIndex(other.topIndex)
{
    arr = new T[capacity];  // allocate NEW memory
    for (int i = 0; i < capacity; i++) {
        arr[i] = other.arr[i];   // deep copy data
    }
}

// Destructor
template <typename T>
Stack<T>::~Stack() {
    delete[] arr;   // free the heap memory
}

// Copy Assignment Operator
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
    if (this != &other) {  // avoid self-assignment
        delete[] arr;      // free old memory

        capacity = other.capacity;
        topIndex = other.topIndex;

        arr = new T[capacity]; // allocate new memory
        for (int i = 0; i < capacity; i++) {
            arr[i] = other.arr[i];
        }
    }
    return *this;
}

// push
template <typename T>
void Stack<T>::push(const T& value) {
    if (isFull()) throw std::overflow_error("Stack overflow!");
    arr[++topIndex] = value;
}

// pop
template <typename T>
T Stack<T>::pop() {
    if (isEmpty()) throw std::underflow_error("Stack underflow!");
    return arr[topIndex--];
}

// top
template <typename T>
T Stack<T>::top() const {
    if (isEmpty()) throw std::underflow_error("Stack is empty!");
    return arr[topIndex];
}

template <typename T>
bool Stack<T>::isEmpty() const { return topIndex == -1; }

template <typename T>
bool Stack<T>::isFull() const { return topIndex == capacity - 1; }

template <typename T>
int Stack<T>::size() const { return topIndex + 1; }

template <typename T>
void Stack<T>::print() const {
    std::cout << "[ ";
    for (int i = 0; i <= topIndex; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "]\n";
}

#endif