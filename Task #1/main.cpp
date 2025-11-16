#include <iostream>
#include <thread>
#include <chrono>
#include "terminal_utils.h"

using namespace std;

int main() {
    // Print colored text in a grid-like pattern
    printAt(10, 5,  "Red text", "red");
    printAt(10, 7,  "Blue text", "blue");
    printAt(10, 9,  "Green text", "green");
    printAt(10, 11, "Bright Yellow", "bright_yellow");

    // Wait 2 seconds
    this_thread::sleep_for(chrono::seconds(2));

    // Clear screen
    clearScreen();

    // Print something else after clearing
    printAt(5, 5, "Hello after clearing!", "magenta");

    printAt(0, 20, "", "white"); // move cursor down
    cout << endl;
}