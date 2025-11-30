#include "App.h"
#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <dirent.h>
#include <algorithm>

namespace fs = std::filesystem;

App::App() : state(MAIN_SCREEN), selected(0) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_RED, COLOR_BLACK);
    }

    createFilesDir();
    buffer = {""};
}

App::~App() {
    endwin();
}

void App::createFilesDir() {
    if (!fs::exists("Files")) {
        fs::create_directory("Files");
    }
}

void App::loadFilesFromDir() {
    file_list.clear();
    for (const auto& entry : fs::directory_iterator("Files")) {
        if (entry.is_regular_file()) {
            file_list.push_back(entry.path());
        }
    }
    std::sort(file_list.begin(), file_list.end());
}

void App::run() {
    while (true) {
        erase();

        switch (state) {
            case MAIN_SCREEN:           drawMainScreen(); break;
            case NEW_CHOICE_SCREEN:     drawNewChoiceScreen(); break;
            case FILE_LIST_SCREEN:      drawFileListScreen(); break;
            case FILE_MODE_SCREEN:      drawFileModeScreen(); break;
            case EDITOR_SCREEN:         drawEditorScreen(); break;
            case DISPLAY_CHOICE_SCREEN: drawDisplayChoiceScreen(); break;
            case DISPLAY_BUFFER_SCREEN: drawDisplayBufferScreen(); break;
            case DISPLAY_FILE_SCREEN:   drawDisplayFileScreen(getFileContent(current_file)); break;
        }

        refresh();
        int ch = getch();

        switch (state) {
            case MAIN_SCREEN:           handleMainInput(ch); break;
            case NEW_CHOICE_SCREEN:     handleNewChoiceInput(ch); break;
            case FILE_LIST_SCREEN:      handleFileListInput(ch); break;
            case FILE_MODE_SCREEN:      handleFileModeInput(ch); break;
            case EDITOR_SCREEN:         handleEditorInput(ch); break;
            case DISPLAY_CHOICE_SCREEN: handleDisplayChoiceInput(ch); break;
            default: break;
        }
    }
}

// ============= DRAW FUNCTIONS =============

void App::drawMainScreen() {
    std::vector<std::string> items = {"New", "Display", "Exit"};
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    for (int i = 0; i < (int)items.size(); ++i) {
        int y = rows / 2 - 2 + i;
        if (i == selected) attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(y, (cols - (int)items[i].size()) / 2, "%s", items[i].c_str());
        if (i == selected) attroff(COLOR_PAIR(1) | A_BOLD);
    }
    mvprintw(rows - 3, 2, "Use ↑↓ to navigate, Enter to select");
}

void App::drawNewChoiceScreen() {
    std::vector<std::string> items = {"Write in Terminal (buffer)", "Write to File"};
    int rows, cols; getmaxyx(stdscr, rows, cols);
    mvprintw(5, (cols - 30)/2, "Where do you want to write?");

    for (int i = 0; i < (int)items.size(); ++i) {
        int y = 8 + i;
        if (i == selected) attron(COLOR_PAIR(1));
        mvprintw(y, (cols - (int)items[i].size())/2, "%s", items[i].c_str());
        if (i == selected) attroff(COLOR_PAIR(1));
    }
}

void App::drawFileListScreen() {
    loadFilesFromDir();
    int rows, cols; getmaxyx(stdscr, rows, cols);
    mvprintw(3, (cols - 30)/2, "Select file (or type new name):");

    if (file_list.empty()) {
        mvprintw(6, (cols - 20)/2, "(No files yet)");
    } else {
        for (int i = 0; i < (int)file_list.size(); ++i) {
            std::string name = file_list[i].filename().string();
            if (i + offset_y == selected) attron(A_REVERSE);
            mvprintw(6 + i, 4, "  %s", name.c_str());
            if (i + offset_y == selected) attroff(A_REVERSE);
        }
    }
    mvprintw(rows - 3, 2, "↑↓ Navigate | Enter: select | N: new file");
}

void App::drawFileModeScreen() {
    int rows, cols; getmaxyx(stdscr, rows, cols);
    mvprintw(6, (cols - 40)/2, "File: %s", current_file.c_str());
    mvprintw(8, (cols - 40)/2, "Mode:");
    std::vector<std::string> opts = {"Append", "Overwrite"};
    for (int i = 0; i < 2; ++i) {
        if (i == (append_mode ? 0 : 1)) attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(10 + i, (cols - 10)/2, "%s", opts[i].c_str());
        if (i == (append_mode ? 0 : 1)) attroff(COLOR_PAIR(1) | A_BOLD);
    }
    mvprintw(rows - 3, 2, "↑↓ Toggle | Enter to continue");
}

void App::drawEditorScreen() {
    int rows, cols; getmaxyx(stdscr, rows, cols);
    mvprintw(1, 2, "Editing: %s | Max chars: %d | ESC to save & exit",
             current_file.empty() ? "Buffer" : current_file.c_str(), max_chars);

    for (int i = 0; i < rows - 4 && i + offset_y < (int)buffer.size(); ++i) {
        mvprintw(3 + i, 2, "%s", buffer[i + offset_y].c_str());
    }

    // Cursor
    int y = 3 + cursor_y - offset_y;
    int x = 2 + cursor_x;
    if (y >= 3 && y < rows - 1) {
        move(y, x < 2 ? 2 : x);
    }
}

void App::drawDisplayChoiceScreen() {
    std::vector<std::string> items = {"View Buffer", "View File from Disk"};
    int rows, cols; getmaxyx(stdscr, rows, cols);
    for (int i = 0; i < (int)items.size(); ++i) {
        if (i == selected) attron(COLOR_PAIR(1));
        mvprintw(8 + i, (cols - (int)items[i].size())/2, "%s", items[i].c_str());
        if (i == selected) attroff(COLOR_PAIR(1));
    }
}

void App::drawDisplayBufferScreen() {
    int rows, cols; getmaxyx(stdscr, rows, cols);
    mvprintw(1, 2, "Buffer Content (Read-only) - ESC to exit");
    for (int i = 0; i < rows - 4 && i + offset_y < (int)buffer.size(); ++i) {
        mvprintw(3 + i, 2, "%s", buffer[i + offset_y].c_str());
    }
}

void App::drawDisplayFileScreen(const std::string& content) {
    int rows, cols; getmaxyx(stdscr, rows, cols);
    mvprintw(1, 2, "File: %s (Read-only) - ESC to exit", current_file.c_str());

    std::vector<std::string> lines;
    std::string line;
    for (char c : content) {
        if (c == '\n') { lines.push_back(line); line.clear(); }
        else line += c;
    }
    if (!line.empty()) lines.push_back(line);

    for (int i = 0; i < rows - 4 && i + offset_y < (int)lines.size(); ++i) {
        mvprintw(3 + i, 2, "%s", lines[i + offset_y].c_str());
    }
}

// ============= INPUT HANDLERS =============

void App::handleMainInput(int ch) {
    if (ch == KEY_UP) selected = (selected - 1 + 3) % 3;
    else if (ch == KEY_DOWN) selected = (selected + 1) % 3;
    else if (ch == 10 || ch == 13) {
        if (selected == 0) { state = NEW_CHOICE_SCREEN; selected = 0; }
        else if (selected == 1) { state = DISPLAY_CHOICE_SCREEN; selected = 0; }
        else { showMessage("Thanks For Using The Program"); endwin(); exit(0); }
    }
}

void App::handleNewChoiceInput(int ch) {
    if (ch == KEY_UP || ch == KEY_DOWN) selected = 1 - selected;
    else if (ch == 10 || ch == 13) {
        buffer = {""};
        cursor_x = cursor_y = offset_y = 0;
        if (selected == 0) {
            // Terminal buffer
            current_file = "";
            char input[256];
            echo();
            mvprintw(LINES - 3, 2, "Max characters: ");
            getnstr(input, 10);
            noecho();
            max_chars = atoi(input);
            if (max_chars < 10) max_chars = 1000;
            state = EDITOR_SCREEN;
        } else {
            state = FILE_LIST_SCREEN;
            selected = 0;
            offset_y = 0;
        }
    }
}

void App::handleFileListInput(int ch) {
    if (ch == KEY_UP) selected = std::max(0, selected - 1);
    if (ch == KEY_DOWN) selected = std::min((int)file_list.size(), selected + 1);

    if (ch == 'n' || ch == 'N') {
        echo();
        char name[256] = {0};
        mvprintw(LINES - 3, 2, "New filename: Files/                         ");
        refresh();
        getnstr(name, 255);
        noecho();

        if (strlen(name) == 0) {
            showMessage("Filename cannot be empty!");
            return;
        }

        current_file = "Files/" + std::string(name);
        // C++17 compatible way to check/add .txt
        if (current_file.size() < 4 || current_file.substr(current_file.size() - 4) != ".txt") {
            current_file += ".txt";
        }
        append_mode = false;  // new file → overwrite mode
        state = FILE_MODE_SCREEN;
    }

    if (ch == 10 || ch == 13) {  // Enter
        if (!file_list.empty() && selected < (int)file_list.size()) {
            current_file = file_list[selected].string();
            state = FILE_MODE_SCREEN;
        } else if (file_list.empty() && selected == 0) {
            // Allow pressing Enter when no files (same as 'n')
            echo();
            char name[256] = {0};
            mvprintw(LINES - 3, 2, "New filename: Files/                         ");
            refresh();
            getnstr(name, 255);
            noecho();
            if (strlen(name) > 0) {
                current_file = "Files/" + std::string(name);
                if (current_file.size() < 4 || current_file.substr(current_file.size() - 4) != ".txt") {
                    current_file += ".txt";
                }
                append_mode = false;
                state = FILE_MODE_SCREEN;
            }
        }
    }

    if (ch == 27) {  // ESC
        state = NEW_CHOICE_SCREEN;
        selected = 0;
    }
}

void App::handleFileModeInput(int ch) {
    if (ch == KEY_UP || ch == KEY_DOWN) append_mode = !append_mode;
    if (ch == 10 || ch == 13) {
        char input[256];
        echo();
        mvprintw(LINES - 3, 2, "Max characters: ");
        getnstr(input, 10);
        noecho();
        max_chars = atoi(input);
        if (max_chars < 10) max_chars = 1000;

        if (!append_mode && fs::exists(current_file)) {
            buffer = {""};
        } else if (append_mode && fs::exists(current_file)) {
            std::ifstream f(current_file);
            buffer.clear();
            std::string line;
            while (std::getline(f, line)) buffer.push_back(line);
            if (buffer.empty()) buffer.push_back("");
            cursor_y = buffer.size() - 1;
            cursor_x = buffer.back().size();
        } else {
            buffer = {""};
        }
        state = EDITOR_SCREEN;
    }
}

void App::handleEditorInput(int ch) {
    std::string& line = buffer[cursor_y];

    if (ch == 27) { // ESC
        saveBufferToFile();
        showMessage("Saved successfully!");
        state = MAIN_SCREEN;
        selected = 0;
        return;
    }

    if (ch == KEY_LEFT && cursor_x > 0) cursor_x--;
    else if (ch == KEY_RIGHT && cursor_x < (int)line.size()) cursor_x++;
    else if (ch == KEY_UP && cursor_y > 0) { cursor_y--; cursor_x = std::min(cursor_x, (int)buffer[cursor_y].size()); }
    else if (ch == KEY_DOWN && cursor_y < (int)buffer.size() - 1) { cursor_y++; cursor_x = std::min(cursor_x, (int)buffer[cursor_y].size()); }
    else if (ch == KEY_BACKSPACE || ch == 127) {
        if (cursor_x > 0) {
            line.erase(cursor_x - 1, 1);
            cursor_x--;
        } else if (cursor_y > 0) {
            cursor_x = buffer[cursor_y - 1].size();
            buffer[cursor_y - 1] += line;
            buffer.erase(buffer.begin() + cursor_y);
            cursor_y--;
        }
    }
    else if (ch == KEY_DC && cursor_x < (int)line.size()) {
        line.erase(cursor_x, 1);
    }
    else if (ch == 10 || ch == 13) {
        std::string rest = line.substr(cursor_x);
        line.erase(cursor_x);
        buffer.insert(buffer.begin() + cursor_y + 1, rest);
        cursor_y++;
        cursor_x = 0;
    }
    else if (ch >= 32 && ch <= 126 && (int)(line.size() + buffer.size()) < max_chars) {
        line.insert(cursor_x, 1, (char)ch);
        cursor_x++;
    }

    // Scrolling
    int rows, cols; getmaxyx(stdscr, rows, cols);
    if (cursor_y - offset_y >= rows - 4) offset_y = cursor_y - (rows - 5);
    if (cursor_y - offset_y < 0) offset_y = cursor_y;
}

void App::handleDisplayChoiceInput(int ch) {
    if (ch == KEY_UP || ch == KEY_DOWN) selected = 1 - selected;
    if (ch == 10 || ch == 13) {
        offset_y = 0;
        if (selected == 0) state = DISPLAY_BUFFER_SCREEN;
        else { loadFilesFromDir(); state = FILE_LIST_SCREEN; }
    }
    if (ch == 27) state = MAIN_SCREEN;
}

// ============= UTILITIES =============

void App::saveBufferToFile() {
    std::ofstream f(current_file.empty() ? "/dev/null" : current_file, append_mode ? std::ios::app : std::ios::trunc);
    for (const auto& line : buffer) {
        f << line << '\n';
    }
}

std::string App::getFileContent(const std::string& path) {
    std::ifstream f(path);
    std::string content, line;
    while (std::getline(f, line)) content += line + "\n";
    return content;
}

void App::showMessage(const std::string& msg) {
    int rows, cols; getmaxyx(stdscr, rows, cols);
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(rows / 2, (cols - (int)msg.size()) / 2, "%s", msg.c_str());
    attroff(COLOR_PAIR(3) | A_BOLD);
    refresh();
    napms(1200);
}