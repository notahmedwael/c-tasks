#ifndef LIST_H
#define LIST_H

#include <string>
#include <iostream>

enum class ValueType { INT, DOUBLE, STRING };

struct ListElement {
    ValueType type;
    int intVal;
    double doubleVal;
    std::string stringVal;
    
    ListElement() : type(ValueType::INT), intVal(0), doubleVal(0.0) {}
};

class List {
private:
    ListElement* elements;
    int capacity;
    int size;
    
    void resize();
    bool isInteger(const std::string& str);
    bool isDouble(const std::string& str);
    
public:
    List();
    ~List();
    
    void append(int value);
    void append(double value);
    void append(const std::string& value);
    
    void insert(int index, int value);
    void insert(int index, double value);
    void insert(int index, const std::string& value);
    
    void remove(int index);
    void clear();
    
    int getSize() const;
    ListElement get(int index) const;
    
    void parseFromString(const std::string& listStr);
    void display() const;
    std::string toString() const;
};

#endif