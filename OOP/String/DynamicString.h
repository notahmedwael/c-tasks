#ifndef DYNAMICSTRING_H
#define DYNAMICSTRING_H

#include <iostream>
#include <stdexcept>

class DynamicString {
private:
    char* data;
    size_t length;

    void copyFrom(const char* src, size_t len);   // helper
    int charCompare(char a, char b) const;        // helper

public:
    // Constructors / Destructor
    DynamicString();
    DynamicString(const char* str);
    DynamicString(const DynamicString& other);
    ~DynamicString();

    // Copy assignment
    DynamicString& operator=(const DynamicString& other);

    // Basic operations
    size_t size() const;
    bool empty() const;
    const char* c_str() const;

    void clear();
    void push_back(char c);
    void pop_back();

    // append / insert / erase / reverse / compare
    void append(const char* s);
    void insert(int index, const char* s);
    void erase(int pos, int len);
    void reverse();
    int compare(const char* s) const; // negative / zero / positive

    // Custom string operations
    void toLower();
    void toUpper();
    void capitalize();

    // Concatenation
    DynamicString operator+(const DynamicString& other) const;
    DynamicString& operator+=(const DynamicString& other);

    // Comparison
    bool operator==(const DynamicString& other) const;
    bool operator!=(const DynamicString& other) const;
    bool operator<(const DynamicString& other) const;
    bool operator>(const DynamicString& other) const;

    // Indexing
    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    // Unary operators
    DynamicString operator+() const;      // copy
    DynamicString operator-() const;      // returns lowercase copy

    // Streams
    friend std::ostream& operator<<(std::ostream& out, const DynamicString& s);
    friend std::istream& operator>>(std::istream& in, DynamicString& s);
};

#endif
