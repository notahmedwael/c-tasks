#include <iostream>
#include <vector>
#include "terminal_utils.h"

using namespace std;

enum ScreenState { MAIN, NEW_SCREEN, OLD_SCREEN };

int main() {
    enableRawMode();
    atexit(disableRawMode);

    vector<string> options = {"New", "Display", "Exit"};

    int selected = 0;
    ScreenState state = MAIN;

    string oldScreen = "Old Screen Content";

    while (true) {
        clearScreen();

        if (state == MAIN) {
            int midY = 10;
            int midX = 30;

            for (int i = 0; i < (int)options.size(); i++) {
                if (i == selected) {
                    setColor("cyan");
                    gotoXY(midX - 2, midY + i);
                    display("> " + options[i]);
                } else {
                    setColor("white");
                    gotoXY(midX, midY + i);
                    display(options[i]);
                }
            }
        }

        if (state == NEW_SCREEN) {
            setColor("green");
            gotoXY(5, 5);
            display("Welcome to New Line");
        }

        if (state == OLD_SCREEN) {
            setColor("yellow");
            gotoXY(5, 5);
            display(oldScreen);
        }

        int key = readKey();

        if (state == MAIN) {
            if (key == 1001 || key == 1003) {             // UP or LEFT
                selected--;
                if (selected < 0) selected = options.size() - 1;
            }
            else if (key == 1002 || key == 1004) {        // DOWN or RIGHT
                selected++;
                if (selected >= (int)options.size()) selected = 0;
            }
            else if (key == 1005) selected = 0;           // HOME
            else if (key == 1006) selected = options.size() - 1; // END

            else if (key == 13) { // ENTER
                if (selected == 0) state = NEW_SCREEN;
                else if (selected == 1) state = OLD_SCREEN;
                else if (selected == 2) {
                    clearScreen();
                    gotoXY(5, 5);
                    setColor("red");
                    display("Thanks For Using The Program");
                    return 0;
                }
            }
        }

        else { // NEW or OLD screen
            if (key == 8) {         // BACKSPACE
                state = MAIN;
            }
            if (key == 1005) {      // HOME
                state = MAIN;
            }
        }
    }

    return 0;
}
