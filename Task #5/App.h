#ifndef APP_H
#define APP_H

#include <string>
#include <vector>

enum ScreenState {
    MAIN_SCREEN,
    NEW_SCREEN,
    DISPLAY_SCREEN
};

struct Employee {
    std::string firstName;
    std::string lastName;
    char gender;
    int age;
    double salary;
};

class App {
public:
    App();
    ~App();

    void run();

private:
    ScreenState state;
    int selected;

    std::vector<Employee> employees;

    void drawMainScreen(int rows, int cols);
    void drawNewScreen(int rows, int cols);
    void drawDisplayScreen(int rows, int cols);

    void handleMainInput(int ch, int rows);
    void handleSubScreenInput(int ch);

    std::string inputString(const std::string &prompt, int rows, int cols);
    bool isInteger(const std::string &s);
    bool isNumber(const std::string &s);
};

#endif
