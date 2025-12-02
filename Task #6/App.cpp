#include "App.h"
#include <ncurses.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// ------------------------ C-string helper utilities ------------------------

char* App::allocLineFromStdString(const std::string& s) {
    size_t len = s.size();
    char* p = new char[len + 1];
    memcpy(p, s.c_str(), len);
    p[len] = '\0';
    return p;
}

void App::replaceLine(int idx, const std::string& s) {
    if (idx < 0 || idx >= (int)buffer.size()) return;
    delete[] buffer[idx];
    buffer[idx] = allocLineFromStdString(s);
}

void App::insertLine(int idx, const std::string& s) {
    char* p = allocLineFromStdString(s);
    if (idx < 0 || idx > (int)buffer.size()) buffer.push_back(p);
    else buffer.insert(buffer.begin() + idx, p);
}

void App::deleteLine(int idx) {
    if (idx < 0 || idx >= (int)buffer.size()) return;
    delete[] buffer[idx];
    buffer.erase(buffer.begin() + idx);
}

void App::insertCharInLine(int y, int x, char c) {
    if (y < 0) return;
    if (y >= (int)buffer.size()) {
        // create lines up to y
        while (y >= (int)buffer.size()) buffer.push_back(allocLineFromStdString(""));
    }
    std::string s(buffer[y]);
    if (x < 0) x = 0;
    if (x > (int)s.size()) x = (int)s.size();
    s.insert((size_t)x, 1, c);
    replaceLine(y, s);
}

void App::eraseCharInLine(int y, int x) {
    if (y < 0 || y >= (int)buffer.size()) return;
    std::string s(buffer[y]);
    if (x < 0 || x >= (int)s.size()) return;
    s.erase((size_t)x, 1);
    replaceLine(y, s);
}

// simple wrap: split line at wrap_col, move suffix to new line below
void App::wrapLineIfNeeded(int y, int /*cursorColBasedOnDisplay*/) {
    int rows, cols; getmaxyx(stdscr, rows, cols);
    int wrap_col = cols - 4; // keep margin, since we print at column 2
    if (wrap_col < 10) wrap_col = 10; // sanity

    if (y < 0 || y >= (int)buffer.size()) return;
    int len = (int)strlen(buffer[y]);
    if (len <= wrap_col) return;

    std::string s(buffer[y]);
    std::string suffix = s.substr(wrap_col);
    s.erase(wrap_col);

    replaceLine(y, s);
    insertLine(y + 1, suffix);

    // if cursor was beyond wrap_col, move it to beginning of new line
    if (cursor_y == y && cursor_x > wrap_col) {
        cursor_y = y + 1;
        cursor_x = cursor_x - wrap_col;
    }
}

// ------------------------ main class implementation ------------------------

App::App() : state(MAIN_SCREEN), selected(0) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

#ifdef NCURSES_VERSION
    set_escdelay(0);
#endif
    notimeout(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_RED, COLOR_BLACK);
    }

    createFilesDir();
    // initial buffer: single empty line
    buffer.clear();
    buffer.push_back(allocLineFromStdString(""));
    ensureBufferInvariant();
}

App::~App() {
    // free lines
    for (auto p : buffer) delete[] p;
    endwin();
}

void App::createFilesDir() {
    if (!fs::exists("Files")) {
        fs::create_directory("Files");
    }
}

void App::loadFilesFromDir() {
    file_list.clear();
    if (!fs::exists("Files")) return;
    for (const auto& entry : fs::directory_iterator("Files")) {
        if (entry.is_regular_file()) file_list.push_back(entry.path());
    }
    std::sort(file_list.begin(), file_list.end());
}

int App::totalCharsInBuffer() const {
    int tot = 0;
    for (const auto& s : buffer) tot += (int)strlen(s);
    return tot;
}

void App::ensureBufferInvariant() {
    if (buffer.empty()) buffer.push_back(allocLineFromStdString(""));
    if (cursor_y < 0) cursor_y = 0;
    if (cursor_y >= (int)buffer.size()) cursor_y = (int)buffer.size() - 1;
    if (cursor_x < 0) cursor_x = 0;
    int curLineLen = (buffer[cursor_y]) ? (int)strlen(buffer[cursor_y]) : 0;
    if (cursor_x > curLineLen) cursor_x = curLineLen;
    if (offset_y < 0) offset_y = 0;
    // do not touch append_lock_* here
}

// ------------------------ run loop & draw ------------------------

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
            case DISPLAY_FILE_SCREEN:   drawDisplayFileScreen(current_file.empty() ? std::string() : getFileContent(current_file)); break;
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
            case DISPLAY_BUFFER_SCREEN: handleDisplayBufferInput(ch); break;
            case DISPLAY_FILE_SCREEN:   handleDisplayFileInput(ch); break;
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
    mvprintw(rows - 3, 2, "Use UP/DOWN KEYS to navigate, Enter to select");
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
    mvprintw(LINES - 3, 2, "Use UP/DOWN KEYS to navigate, Enter confirm, ESC back");
}

void App::drawFileListScreen() {
    loadFilesFromDir();
    int rows, cols; getmaxyx(stdscr, rows, cols);
    mvprintw(3, (cols - 30)/2, "Select file (or press N to create new):");

    if (file_list.empty()) {
        mvprintw(6, (cols - 20)/2, "(No files yet)");
    } else {
        int max_display = rows - 10;
        int start = 0;
        if (selected < start) start = selected;
        if (selected >= start + max_display) start = selected - max_display + 1;

        for (int i = start; i < (int)file_list.size() && i < start + max_display; ++i) {
            std::string name = file_list[i].filename().string();
            if (i == selected) attron(A_REVERSE);
            mvprintw(6 + (i - start), 4, "  %s", name.c_str());
            if (i == selected) attroff(A_REVERSE);
        }
    }
    mvprintw(rows - 3, 2, "Use UP/DOWN KEYS to Navigate | Enter: select | N: new | ESC: back");
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
    mvprintw(rows - 3, 2, "Use UP/DOWN KEYS to Toggle | Enter to continue | ESC: cancel");
}

void App::drawEditorScreen() {
    ensureBufferInvariant();
    int rows, cols; getmaxyx(stdscr, rows, cols);
    mvprintw(1, 2, "Editing: %s | Total chars: %d/%d | ESC to save & exit",
             current_file.empty() ? "Buffer" : current_file.c_str(), totalCharsInBuffer(), max_chars);

    int display_lines = rows - 5;
    if (cursor_y < offset_y) offset_y = cursor_y;
    if (cursor_y >= offset_y + display_lines) offset_y = cursor_y - display_lines + 1;

    for (int i = 0; i < display_lines && i + offset_y < (int)buffer.size(); ++i) {
        mvprintw(3 + i, 2, "%s", buffer[i + offset_y]);
        clrtoeol();
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
    mvprintw(LINES - 3, 2, "Use UP/DOWN KEYS to  navigate, Enter select, ESC back");
}

void App::drawDisplayBufferScreen() {
    int rows, cols; getmaxyx(stdscr, rows, cols);
    mvprintw(1, 2, "Buffer Content (Read-only) - ESC to exit");
    int display_lines = rows - 5;
    if (offset_y < 0) offset_y = 0;
    for (int i = 0; i < display_lines && i + offset_y < (int)buffer.size(); ++i) {
        mvprintw(3 + i, 2, "%s", buffer[i + offset_y]);
    }
    mvprintw(rows - 3, 2, "Up/Down to scroll, ESC to return");
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
    // include last (even if empty)
    lines.push_back(line);

    int display_lines = rows - 5;
    if (offset_y < 0) offset_y = 0;
    for (int i = 0; i < display_lines && i + offset_y < (int)lines.size(); ++i) {
        mvprintw(3 + i, 2, "%s", lines[i + offset_y].c_str());
    }
    mvprintw(rows - 3, 2, "Up/Down to scroll, ESC to return");
}

// ============= INPUT HANDLERS =============

void App::handleMainInput(int ch) {
    if (ch == KEY_UP) selected = (selected - 1 + 3) % 3;
    else if (ch == KEY_DOWN) selected = (selected + 1) % 3;
    else if (ch == 10 || ch == 13) {
        if (selected == 0) { state = NEW_CHOICE_SCREEN; selected = 0; }
        else if (selected == 1) { state = DISPLAY_CHOICE_SCREEN; selected = 0; }
        else { clear(); showMessage("Thanks For Using The Program"); endwin(); exit(0); }
    }
    else if (ch == 27) {
        // ESC on main does nothing
    }
}

void App::handleNewChoiceInput(int ch) {
    if (ch == KEY_UP || ch == KEY_DOWN) selected = 1 - selected;
    else if (ch == 10 || ch == 13) {
        // reset buffer to single empty line
        for (auto p : buffer) delete[] p;
        buffer.clear();
        buffer.push_back(allocLineFromStdString(""));
        cursor_x = cursor_y = offset_y = 0;
        ensureBufferInvariant();
        if (selected == 0) {
            // Terminal buffer
            current_file = "";
            char input[32] = {0};
            echo();
            mvprintw(LINES - 3, 2, "Max characters (enter number, default 1000): ");
            clrtoeol();
            refresh();
            getnstr(input, 10);
            noecho();
            int tmp = atoi(input);
            max_chars = (tmp >= 10) ? tmp : 1000;
            selecting_for_display = false;
            state = EDITOR_SCREEN;
        } else {
            // Write to file -> show file list for selection/creation
            selecting_for_display = false;
            state = FILE_LIST_SCREEN;
            selected = 0;
            offset_y = 0;
        }
    } else if (ch == 27) {
        state = MAIN_SCREEN;
        selected = 0;
    }
}

void App::handleFileListInput(int ch) {
    loadFilesFromDir();
    if (file_list.empty()) {
        // allow only new file creation or ESC
        if (ch == 'n' || ch == 'N') {
            echo();
            char name[256] = {0};
            mvprintw(LINES - 3, 2, "New filename (without path): ");
            clrtoeol();
            refresh();
            getnstr(name, 255);
            noecho();
            if (strlen(name) == 0) {
                showMessage("Filename cannot be empty!");
                return;
            }
            current_file = "Files/" + std::string(name);
            if (current_file.size() < 4 || current_file.substr(current_file.size() - 4) != ".txt") current_file += ".txt";
            append_mode = false;
            append_lock_x = append_lock_y = -1;
            state = FILE_MODE_SCREEN;
            return;
        }
        if (ch == 27) { state = NEW_CHOICE_SCREEN; selected = 0; return; }
        return;
    }

    if (ch == KEY_UP) selected = std::max(0, selected - 1);
    else if (ch == KEY_DOWN) selected = std::min((int)file_list.size() - 1, selected + 1);
    else if (ch == 'n' || ch == 'N') {
        echo();
        char name[256] = {0};
        mvprintw(LINES - 3, 2, "New filename (without path): ");
        clrtoeol();
        refresh();
        getnstr(name, 255);
        noecho();

        if (strlen(name) == 0) {
            showMessage("Filename cannot be empty!");
            return;
        }

        current_file = "Files/" + std::string(name);
        if (current_file.size() < 4 || current_file.substr(current_file.size() - 4) != ".txt") {
            current_file += ".txt";
        }
        append_mode = false;  // new file → overwrite mode
        append_lock_x = append_lock_y = -1;
        state = FILE_MODE_SCREEN;
    }
    else if (ch == 10 || ch == 13) {  // Enter
        if (!file_list.empty() && selected >= 0 && selected < (int)file_list.size()) {
            current_file = file_list[selected].string();
            if (selecting_for_display) {
                state = DISPLAY_FILE_SCREEN;
                offset_y = 0;
            } else {
                state = FILE_MODE_SCREEN;
            }
        }
    }
    else if (ch == 27) {  // ESC
        // return to whichever previous screen appropriate
        if (selecting_for_display) {
            state = DISPLAY_CHOICE_SCREEN;
            selected = 1; // highlight "View File from Disk"
        } else {
            state = NEW_CHOICE_SCREEN;
            selected = 1; // default to "Write to File"
        }
    }
}

void App::handleFileModeInput(int ch) {
    if (ch == KEY_UP || ch == KEY_DOWN) append_mode = !append_mode;
    if (ch == 10 || ch == 13) {
        char input[32] = {0};
        echo();
        mvprintw(LINES - 3, 2, "Max characters (enter number, default 1000): ");
        clrtoeol();
        refresh();
        getnstr(input, 10);
        noecho();
        int tmp = atoi(input);
        max_chars = (tmp >= 10) ? tmp : 1000;

        for (auto p : buffer) delete[] p;
        buffer.clear();
        original_file_lines.clear();

        if (append_mode && fs::exists(current_file)) {
            // Load file into buffer and original_file_lines
            std::ifstream f(current_file);
            std::string line;
            while (std::getline(f, line)) {
                buffer.push_back(allocLineFromStdString(line));
                original_file_lines.push_back(line);
            }
            if (buffer.empty()) buffer.push_back(allocLineFromStdString(""));
            
            // Lock cursor at end
            append_lock_y = (int)buffer.size() - 1;
            append_lock_x = (int)strlen(buffer.back());
            cursor_y = append_lock_y;
            cursor_x = append_lock_x;
        } else {
            // Overwrite or new file
            buffer.push_back(allocLineFromStdString(""));
            append_lock_x = append_lock_y = -1;
        }

        ensureBufferInvariant();
        state = EDITOR_SCREEN;
    }
    if (ch == 27) { state = FILE_LIST_SCREEN; }
}


void App::handleEditorInput(int ch) {
    ensureBufferInvariant();

    // ESC -> save (if file) and exit to main
    if (ch == 27) {
        if (!current_file.empty()) {
            saveBufferToFile();
            showMessage("Saved successfully!");
        } else {
            showMessage("Buffer kept in memory (not saved to file).");
        }
        state = MAIN_SCREEN;
        selected = 0;
        return;
    }

    int rows, cols; getmaxyx(stdscr, rows, cols);
    int display_lines = rows - 5;
    int wrap_col = cols - 4;
    if (wrap_col < 10) wrap_col = 10;

    // Movement keys
    if (ch == KEY_LEFT) {
        if (cursor_x > 0) cursor_x--;
        else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = (int)strlen(buffer[cursor_y]);
        }
    }
    else if (ch == KEY_RIGHT) {
        int linelen = (int)strlen(buffer[cursor_y]);
        if (cursor_x < linelen) cursor_x++;
        else if (cursor_y < (int)buffer.size() - 1) {
            cursor_y++;
            cursor_x = 0;
        }
    }
    else if (ch == KEY_UP) {
        if (cursor_y > 0) {
            cursor_y--;
            cursor_x = std::min(cursor_x, (int)strlen(buffer[cursor_y]));
        }
    }
    else if (ch == KEY_DOWN) {
        if (cursor_y < (int)buffer.size() - 1) {
            cursor_y++;
            cursor_x = std::min(cursor_x, (int)strlen(buffer[cursor_y]));
        }
    }
    // BACKSPACE
    else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
        if (cursor_x > 0) {
            eraseCharInLine(cursor_y, cursor_x - 1);
            cursor_x--;
        } else if (cursor_y > 0) {
            int prev_len = (int)strlen(buffer[cursor_y - 1]);
            // append current line to previous
            std::string merged = std::string(buffer[cursor_y - 1]) + std::string(buffer[cursor_y]);
            replaceLine(cursor_y - 1, merged);
            deleteLine(cursor_y);
            cursor_y--;
            cursor_x = prev_len;
        }
    }
    // DELETE
    else if (ch == KEY_DC) {
        int linelen = (int)strlen(buffer[cursor_y]);
        if (cursor_x < linelen) {
            eraseCharInLine(cursor_y, cursor_x);
        } else if (cursor_y < (int)buffer.size() - 1) {
            std::string merged = std::string(buffer[cursor_y]) + std::string(buffer[cursor_y + 1]);
            replaceLine(cursor_y, merged);
            deleteLine(cursor_y + 1);
        }
    }
    // ENTER / newline
    else if (ch == 10 || ch == 13) {
        std::string cur(buffer[cursor_y]);
        std::string rest = (cursor_x < (int)cur.size()) ? cur.substr(cursor_x) : std::string();
        if (cursor_x < (int)cur.size()) cur.erase(cursor_x);
        replaceLine(cursor_y, cur);
        insertLine(cursor_y + 1, rest);
        cursor_y++;
        cursor_x = 0;
    }
    // printable char insertion
    else if (ch >= 32 && ch <= 126) {
        if (totalCharsInBuffer() < max_chars) {
            insertCharInLine(cursor_y, cursor_x, (char)ch);
            cursor_x++;
            // wrap if needed
            if ((int)strlen(buffer[cursor_y]) > wrap_col) {
                // split at wrap_col
                std::string cur(buffer[cursor_y]);
                std::string suffix = cur.substr(wrap_col);
                cur.erase(wrap_col);
                replaceLine(cursor_y, cur);
                insertLine(cursor_y + 1, suffix);
                if (cursor_x > wrap_col) {
                    cursor_y = cursor_y + 1;
                    cursor_x = cursor_x - wrap_col;
                }
            }
        } else beep();
    }

    // Scrolling logic
    if (cursor_y < offset_y) offset_y = cursor_y;
    if (cursor_y >= offset_y + display_lines) offset_y = cursor_y - display_lines + 1;

    ensureBufferInvariant();
}

// Display handlers so ESC works and Up/Down scrolls
void App::handleDisplayBufferInput(int ch) {
    int rows, cols; getmaxyx(stdscr, rows, cols);
    int display_lines = rows - 5;
    if (ch == KEY_UP) offset_y = std::max(0, offset_y - 1);
    else if (ch == KEY_DOWN) offset_y = offset_y + 1;
    else if (ch == 27) {
        state = DISPLAY_CHOICE_SCREEN;
        selected = 0;
        offset_y = 0;
    }
    if (offset_y > (int)buffer.size()) offset_y = (int)buffer.size();
}

void App::handleDisplayFileInput(int ch) {
    if (ch == KEY_UP) offset_y = std::max(0, offset_y - 1);
    else if (ch == KEY_DOWN) offset_y = offset_y + 1;
    else if (ch == 27) {
        state = DISPLAY_CHOICE_SCREEN;
        selected = 1;
        offset_y = 0;
    }
    if (offset_y < 0) offset_y = 0;
}

void App::handleDisplayChoiceInput(int ch) {
    if (ch == KEY_UP || ch == KEY_DOWN) selected = 1 - selected;
    if (ch == 10 || ch == 13) {
        offset_y = 0;
        if (selected == 0) {
            state = DISPLAY_BUFFER_SCREEN;
        } else {
            loadFilesFromDir();
            selecting_for_display = true;
            selected = 0;
            state = FILE_LIST_SCREEN;
        }
    }
    if (ch == 27) state = MAIN_SCREEN;
}

// ============= UTILITIES =============

void App::saveBufferToFile() {
    if (current_file.empty()) return;

    if (append_mode) {
        std::ofstream f(current_file, std::ios::app);
        if (!f.is_open()) {
            showMessage("Failed to open file for writing!");
            return;
        }

        int original_size = (int)original_file_lines.size();

        for (int i = append_lock_y; i < (int)buffer.size(); ++i) {
            std::string line(buffer[i]);
            if (i < original_size) {
                // Only append new chars after original content
                int start_pos = (i == append_lock_y) ? append_lock_x : 0;
                if (start_pos < (int)line.size()) {
                    f << line.substr(start_pos) << '\n';
                }
            } else {
                // Entire new line
                f << line << '\n';
            }
        }

        // Update original_file_lines for next append session
        original_file_lines.clear();
        std::ifstream fin(current_file);
        std::string l;
        while (std::getline(fin, l)) original_file_lines.push_back(l);

        // Move append lock to new end
        append_lock_y = (int)buffer.size() - 1;
        append_lock_x = (int)strlen(buffer.back());

        return;
    }

    // Overwrite mode
    std::ofstream f(current_file, std::ios::trunc);
    if (!f.is_open()) {
        showMessage("Failed to open file for writing!");
        return;
    }
    for (const auto& line : buffer) f << line << '\n';
}



std::string App::getFileContent(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return std::string();
    std::string content, line;
    while (std::getline(f, line)) content += line + "\n";
    return content;
}

void App::showMessage(const std::string& msg) {
    int rows, cols; getmaxyx(stdscr, rows, cols);
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(rows / 2, std::max(0, (cols - (int)msg.size()) / 2), "%s", msg.c_str());
    attroff(COLOR_PAIR(3) | A_BOLD);
    refresh();
    napms(1000);
}
