#include "Swap.h"

void swapDefault(int &a, int &b) {
    int temp = a; // Temp to hold variable
    a = b; // b becomes original a
    b = temp; // a becomes original b
}

void swapXOR(int &a, int &b) {
    if (&a != &b) { // Check for self-swap, XORing a number with itself results in 0
        a = a ^ b; // a now holds (a XOR b)
        b = a ^ b; // b becomes original a: (a XOR b) XOR b = a
        a = a ^ b; // a becomes original b: (a XOR b) XOR a = b
    }
}

void swapAddSub(int &a, int &b) {
    a = a + b; // a now holds sum of both
    b = a - b; // b becomes original a: (a+b) - b = a
    a = a - b; // a becomes original b: (a+b) - a = b
}