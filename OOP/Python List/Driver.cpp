#include "Driver.h"
#include <iostream>
#include <string>
#include <limits>

void displayMenu() {
    std::cout << "\n=== Python-like List Menu ===\n";
    std::cout << "1. Append element\n";
    std::cout << "2. Insert element at index\n";
    std::cout << "3. Remove element at index\n";
    std::cout << "4. Parse list from string\n";
    std::cout << "5. Get element at index\n";
    std::cout << "6. Display list\n";
    std::cout << "7. Clear list\n";
    std::cout << "8. Exit\n";
    std::cout << "Choice: ";
}

void handleAppend(List& list) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string input;
    std::cout << "Enter value: ";
    std::getline(std::cin, input);
    
    // Trim whitespace
    while (!input.empty() && isspace(input[0])) input.erase(0, 1);
    while (!input.empty() && isspace(input.back())) input.pop_back();
    
    if (input.empty()) {
        std::cout << "Error: Cannot append empty value!\n";
        return;
    }
    
    // Check if it's a number (int or double)
    bool isNumber = true;
    bool hasDecimal = false;
    int start = (input[0] == '-' || input[0] == '+') ? 1 : 0;
    
    if (input.empty() || (start == 1 && input.length() == 1)) {
        isNumber = false;
    } else {
        for (size_t i = start; i < input.length(); i++) {
            if (input[i] == '.') {
                if (hasDecimal) {
                    isNumber = false;
                    break;
                }
                hasDecimal = true;
            } else if (!isdigit(input[i])) {
                isNumber = false;
                break;
            }
        }
    }
    
    if (isNumber && hasDecimal) {
        list.append(std::stod(input));
        std::cout << "Appended as double\n";
    } else if (isNumber && !hasDecimal) {
        list.append(std::stoi(input));
        std::cout << "Appended as integer\n";
    } else {
        list.append(input);
        std::cout << "Appended as string\n";
    }
}

void handleInsert(List& list) {
    int index;
    std::cout << "Enter index (0 to " << list.getSize() << "): ";
    
    if (!(std::cin >> index)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input! Index must be an integer.\n";
        return;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (index < 0) {
        std::cout << "Error: Index cannot be negative!\n";
        return;
    }
    
    if (index > list.getSize()) {
        std::cout << "Error: Index " << index << " is too large! You can insert at positions 0 to " << list.getSize() << ".\n";
        return;
    }
    
    std::string input;
    std::cout << "Enter value: ";
    std::getline(std::cin, input);
    
    // Trim whitespace
    while (!input.empty() && isspace(input[0])) input.erase(0, 1);
    while (!input.empty() && isspace(input.back())) input.pop_back();
    
    if (input.empty()) {
        std::cout << "Error: Cannot insert empty value!\n";
        return;
    }
    
    // Check if it's a number (int or double)
    bool isNumber = true;
    bool hasDecimal = false;
    int start = (input[0] == '-' || input[0] == '+') ? 1 : 0;
    
    if (input.empty() || (start == 1 && input.length() == 1)) {
        isNumber = false;
    } else {
        for (size_t i = start; i < input.length(); i++) {
            if (input[i] == '.') {
                if (hasDecimal) {
                    isNumber = false;
                    break;
                }
                hasDecimal = true;
            } else if (!isdigit(input[i])) {
                isNumber = false;
                break;
            }
        }
    }
    
    if (isNumber && hasDecimal) {
        list.insert(index, std::stod(input));
        std::cout << "Inserted as double at index " << index << "\n";
    } else if (isNumber && !hasDecimal) {
        list.insert(index, std::stoi(input));
        std::cout << "Inserted as integer at index " << index << "\n";
    } else {
        list.insert(index, input);
        std::cout << "Inserted as string at index " << index << "\n";
    }
}

void handleRemove(List& list) {
    if (list.getSize() == 0) {
        std::cout << "Error: List is empty! Cannot remove elements.\n";
        return;
    }
    
    int index;
    std::cout << "Enter index to remove (0 to " << list.getSize() - 1 << "): ";
    
    if (!(std::cin >> index)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input! Index must be an integer.\n";
        return;
    }
    
    if (index < 0) {
        std::cout << "Error: Index cannot be negative!\n";
        return;
    }
    
    if (index >= list.getSize()) {
        std::cout << "Error: Index " << index << " is out of bounds! List size is " << list.getSize() << ".\n";
        return;
    }
    
    list.remove(index);
    std::cout << "Element at index " << index << " removed successfully!\n";
}

void handleParse(List& list) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string input;
    std::cout << "Enter list in Python format [e.g., [1, 2.5, 'hello', \"world\"]]: ";
    std::getline(std::cin, input);
    
    // Trim whitespace
    while (!input.empty() && isspace(input[0])) input.erase(0, 1);
    while (!input.empty() && isspace(input.back())) input.pop_back();
    
    if (input.empty()) {
        std::cout << "Error: Cannot parse empty input!\n";
        return;
    }
    
    // Check for valid list format
    if (input.front() != '[' || input.back() != ']') {
        std::cout << "Error: Invalid list format! Must start with '[' and end with ']'.\n";
        return;
    }
    
    list.parseFromString(input);
    std::cout << "List parsed successfully! (" << list.getSize() << " elements)\n";
}

void handleGet(List& list) {
    if (list.getSize() == 0) {
        std::cout << "Error: List is empty! No elements to retrieve.\n";
        return;
    }
    
    int index;
    std::cout << "Enter index (0 to " << list.getSize() - 1 << "): ";
    
    if (!(std::cin >> index)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: Invalid input! Index must be an integer.\n";
        return;
    }
    
    if (index < 0) {
        std::cout << "Error: Index cannot be negative!\n";
        return;
    }
    
    if (index >= list.getSize()) {
        std::cout << "Error: Index " << index << " is out of bounds! List size is " << list.getSize() << ".\n";
        return;
    }
    
    ListElement elem = list.get(index);
    std::cout << "Element at index " << index << ": ";
    if (elem.type == ValueType::INT) {
        std::cout << elem.intVal << " (int)\n";
    } else if (elem.type == ValueType::DOUBLE) {
        std::cout << elem.doubleVal << " (double)\n";
    } else {
        std::cout << "'" << elem.stringVal << "' (string)\n";
    }
}

void run() {
    List list;
    int choice;
    
    std::cout << "Python-like List in C++\n";
    std::cout << "=======================\n";
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                handleAppend(list);
                break;
            case 2:
                handleInsert(list);
                break;
            case 3:
                handleRemove(list);
                break;
            case 4:
                handleParse(list);
                break;
            case 5:
                handleGet(list);
                break;
            case 6:
                std::cout << "List: ";
                list.display();
                std::cout << "Size: " << list.getSize() << "\n";
                break;
            case 7:
                list.clear();
                std::cout << "List cleared!\n";
                break;
            case 8:
                std::cout << "Goodbye!\n";
                return;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}