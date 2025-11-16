#include <iostream>
#include <thread>
#include <chrono>
#include "terminal_utils.h"

using namespace std;

int main() {

    clearScreen();

    setColor("blue");
    gotoXY(10, 5);
    display("Hello from the grid!");

    this_thread::sleep_for(chrono::seconds(2));

    clearScreen();

    setColor("red");
    gotoXY(20, 10);
    display("Screen cleared, new message here!");

    this_thread::sleep_for(chrono::seconds(2));

    clearScreen();
    setColor("green");
    gotoXY(5, 3);
    display("Program end.");

    return 0;
}
