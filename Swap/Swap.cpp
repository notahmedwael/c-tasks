#include "Swap.h"

void swapDefault(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void swapXOR(int &a, int &b) {
    if (&a != &b) { // Check for self-swap, XORing a number with itself results in 0
        a = a ^ b;
        b = a ^ b;
        a = a ^ b;
    }
}

void swapAddSub(int &a, int &b) {
    a = a + b;
    b = a - b;
    a = a - b;
}