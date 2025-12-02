#include <iostream>
#include <string>
#include "Stack.h"
#include "Driver.h"

void showMenu() {
    std::cout << "\n===== Stack Menu =====\n";
    std::cout << "1. Push\n";
    std::cout << "2. Pop\n";
    std::cout << "3. Top\n";
    std::cout << "4. Check if empty\n";
    std::cout << "5. Check if full\n";
    std::cout << "6. Size\n";
    std::cout << "7. Print stack\n";
    std::cout << "8. Deep copy using copy constructor\n";
    std::cout << "9. Deep copy using copy assignment\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter choice: ";
}

void runStackMenu() {
    std::cout << "Choose data type:\n";
    std::cout << "1. Integer\n";
    std::cout << "2. Double\n";
    std::cout << "3. String\n";
    std::cout << "4. Character\n";
    std::cout << "Enter choice: ";
    
    int typeChoice;
    std::cin >> typeChoice;
    
    switch (typeChoice) {
        case 1:
            runStackMenuForType<int>();
            break;
        case 2:
            runStackMenuForType<double>();
            break;
        case 3:
            runStackMenuForType<std::string>();
            break;
        case 4:
            runStackMenuForType<char>();
            break;
        default:
            std::cout << "Invalid choice! Defaulting to int.\n";
            runStackMenuForType<int>();
    }
}

template <typename T>
void runStackMenuForType() {
    int capacity;
    std::cout << "Enter stack capacity: ";
    std::cin >> capacity;

    Stack<T> s(capacity);
    Stack<T> copy;   // empty stack for copying
    int choice;

    while (true) {
        showMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: { // Push
                T value;
                std::cout << "Enter value: ";
                std::cin >> value;
                try {
                    s.push(value);
                } catch (std::exception& e) {
                    std::cout << e.what() << "\n";
                }
                break;
            }

            case 2: { // Pop
                try {
                    std::cout << "Popped: " << s.pop() << "\n";
                } catch (std::exception& e) {
                    std::cout << e.what() << "\n";
                }
                break;
            }

            case 3: { // Top
                try {
                    std::cout << "Top: " << s.top() << "\n";
                } catch (std::exception& e) {
                    std::cout << e.what() << "\n";
                }
                break;
            }

            case 4:
                std::cout << (s.isEmpty() ? "Stack is empty\n" : "Stack is NOT empty\n");
                break;

            case 5:
                std::cout << (s.isFull() ? "Stack is full\n" : "Stack is NOT full\n");
                break;

            case 6:
                std::cout << "Size: " << s.size() << "\n";
                break;

            case 7:
                s.print();
                break;

            case 8: { // Deep copy via copy constructor
                Stack<T> temp = s;  // invokes copy constructor
                std::cout << "Deep copy (copy constructor) created!\n";
                std::cout << "Copy stack contents: ";
                temp.print();
                break;
            }

            case 9: { // Deep copy via assignment
                copy = s;  // invokes operator=
                std::cout << "Deep copy (assignment operator) done!\n";
                std::cout << "Copy stack contents: ";
                copy.print();
                break;
            }

            case 0:
                std::cout << "Exiting...\n";
                return;

            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

// Explicit instantiations for common types
template void runStackMenuForType<int>();
template void runStackMenuForType<double>();
template void runStackMenuForType<std::string>();
template void runStackMenuForType<char>();