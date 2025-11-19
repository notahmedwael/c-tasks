#ifndef APP_H
#define APP_H

#include <string>
#include <vector>

enum ScreenState {
    MAIN_SCREEN,
    NEW_SCREEN,
    DISPLAY_SCREEN
};

class App {
public:
    App();
    ~App();

    void run();

private:
    ScreenState state;
    int selected;

    void drawMainScreen(int rows, int cols);
    void drawNewScreen(int rows, int cols);
    void drawDisplayScreen(int rows, int cols);

    void handleMainInput(int ch, int rows);
    void handleSubScreenInput(int ch);
};

#endif
