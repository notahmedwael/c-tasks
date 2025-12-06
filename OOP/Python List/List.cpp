#include "List.h"
#include <sstream>
#include <cctype>

List::List() : capacity(10), size(0) {
    elements = new ListElement[capacity];
}

List::~List() {
    delete[] elements;
}

void List::resize() {
    capacity *= 2;
    ListElement* newElements = new ListElement[capacity];
    for (int i = 0; i < size; i++) {
        newElements[i] = elements[i];
    }
    delete[] elements;
    elements = newElements;
}

bool List::isInteger(const std::string& str) {
    if (str.empty()) return false;
    int start = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    if (start == 1 && str.length() == 1) return false;
    
    for (size_t i = start; i < str.length(); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool List::isDouble(const std::string& str) {
    if (str.empty()) return false;
    bool hasDecimal = false;
    int start = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    if (start == 1 && str.length() == 1) return false;
    
    for (size_t i = start; i < str.length(); i++) {
        if (str[i] == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        } else if (!isdigit(str[i])) {
            return false;
        }
    }
    return hasDecimal;
}

void List::append(int value) {
    if (size >= capacity) resize();
    elements[size].type = ValueType::INT;
    elements[size].intVal = value;
    size++;
}

void List::append(double value) {
    if (size >= capacity) resize();
    elements[size].type = ValueType::DOUBLE;
    elements[size].doubleVal = value;
    size++;
}

void List::append(const std::string& value) {
    if (size >= capacity) resize();
    elements[size].type = ValueType::STRING;
    elements[size].stringVal = value;
    size++;
}

void List::insert(int index, int value) {
    if (index < 0 || index > size) return;
    if (size >= capacity) resize();
    
    for (int i = size; i > index; i--) {
        elements[i] = elements[i - 1];
    }
    elements[index].type = ValueType::INT;
    elements[index].intVal = value;
    size++;
}

void List::insert(int index, double value) {
    if (index < 0 || index > size) return;
    if (size >= capacity) resize();
    
    for (int i = size; i > index; i--) {
        elements[i] = elements[i - 1];
    }
    elements[index].type = ValueType::DOUBLE;
    elements[index].doubleVal = value;
    size++;
}

void List::insert(int index, const std::string& value) {
    if (index < 0 || index > size) return;
    if (size >= capacity) resize();
    
    for (int i = size; i > index; i--) {
        elements[i] = elements[i - 1];
    }
    elements[index].type = ValueType::STRING;
    elements[index].stringVal = value;
    size++;
}

void List::remove(int index) {
    if (index < 0 || index >= size) return;
    
    for (int i = index; i < size - 1; i++) {
        elements[i] = elements[i + 1];
    }
    size--;
}

void List::clear() {
    size = 0;
}

int List::getSize() const {
    return size;
}

ListElement List::get(int index) const {
    if (index >= 0 && index < size) {
        return elements[index];
    }
    return ListElement();
}

void List::parseFromString(const std::string& listStr) {
    clear();
    
    std::string str = listStr;
    size_t start = str.find('[');
    size_t end = str.rfind(']');
    
    if (start == std::string::npos || end == std::string::npos) {
        std::cout << "Invalid list format. Use: [item1, item2, ...]\n";
        return;
    }
    
    std::string content = str.substr(start + 1, end - start - 1);
    std::stringstream ss(content);
    std::string item;
    
    while (std::getline(ss, item, ',')) {
        while (!item.empty() && isspace(item[0])) item.erase(0, 1);
        while (!item.empty() && isspace(item.back())) item.pop_back();
        
        if (item.empty()) continue;
        
        if ((item.front() == '"' && item.back() == '"') || 
            (item.front() == '\'' && item.back() == '\'')) {
            append(item.substr(1, item.length() - 2));
        } else if (isInteger(item)) {
            append(std::stoi(item));
        } else if (isDouble(item)) {
            append(std::stod(item));
        } else {
            append(item);
        }
    }
}

void List::display() const {
    std::cout << "[";
    for (int i = 0; i < size; i++) {
        if (elements[i].type == ValueType::INT) {
            std::cout << elements[i].intVal;
        } else if (elements[i].type == ValueType::DOUBLE) {
            std::cout << elements[i].doubleVal;
        } else {
            std::cout << "'" << elements[i].stringVal << "'";
        }
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

std::string List::toString() const {
    std::stringstream ss;
    ss << "[";
    for (int i = 0; i < size; i++) {
        if (elements[i].type == ValueType::INT) {
            ss << elements[i].intVal;
        } else if (elements[i].type == ValueType::DOUBLE) {
            ss << elements[i].doubleVal;
        } else {
            ss << "'" << elements[i].stringVal << "'";
        }
        if (i < size - 1) ss << ", ";
    }
    ss << "]";
    return ss.str();
}