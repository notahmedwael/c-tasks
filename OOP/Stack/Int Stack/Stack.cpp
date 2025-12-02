#include "Stack.h"

// Constructor
Stack::Stack(int cap) : capacity(cap), topIndex(-1) {
    arr = new int[capacity];  // allocate on heap
}

// Deep Copy Constructor
Stack::Stack(const Stack& other)
    : capacity(other.capacity), topIndex(other.topIndex)
{
    arr = new int[capacity];  // allocate NEW memory
    for (int i = 0; i < capacity; i++) {
        arr[i] = other.arr[i];   // deep copy data
    }
}

// Destructor
Stack::~Stack() {
    delete[] arr;   // free the heap memory
}

// Copy Assignment Operator
Stack& Stack::operator=(const Stack& other) {
    if (this != &other) {  // avoid self-assignment
        delete[] arr;      // free old memory

        capacity = other.capacity;
        topIndex = other.topIndex;

        arr = new int[capacity]; // allocate new memory
        for (int i = 0; i < capacity; i++) {
            arr[i] = other.arr[i];
        }
    }
    return *this;
}

// push
void Stack::push(int value) {
    if (isFull()) throw std::overflow_error("Stack overflow!");
    arr[++topIndex] = value;
}

// pop
int Stack::pop() {
    if (isEmpty()) throw std::underflow_error("Stack underflow!");
    return arr[topIndex--];
}

// top
int Stack::top() const {
    if (isEmpty()) throw std::underflow_error("Stack is empty!");
    return arr[topIndex];
}

bool Stack::isEmpty() const { return topIndex == -1; }
bool Stack::isFull() const { return topIndex == capacity - 1; }

int Stack::size() const { return topIndex + 1; }

void Stack::print() const {
    std::cout << "[ ";
    for (int i = 0; i <= topIndex; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "]\n";
}
