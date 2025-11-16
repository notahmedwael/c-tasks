#include <ncurses.h>
#include <string>
#include <vector>
#include <unistd.h> // for usleep

using namespace std;

enum ScreenState { MAIN, NEW_SCREEN, OLD_SCREEN };

void drawCentered(int y, const string& text, int color_pair = 0) {
    int cols = COLS;
    int x = (cols - text.length()) / 2;
    if (x < 0) x = 0;
    
    if (color_pair > 0) attron(COLOR_PAIR(color_pair));
    mvprintw(y, x, "%s", text.c_str());
    if (color_pair > 0) attroff(COLOR_PAIR(color_pair));
}

int main() {
    // Initialize ncurses
    initscr();              // Start ncurses mode
    cbreak();               // Disable line buffering
    noecho();               // Don't echo input
    keypad(stdscr, TRUE);   // Enable special keys (arrows, etc.)
    curs_set(0);            // Hide cursor
    
    // Initialize colors if terminal supports it
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);    // Cyan for selected
        init_pair(2, COLOR_WHITE, COLOR_BLACK);   // White for normal
        init_pair(3, COLOR_GREEN, COLOR_BLACK);   // Green for new screen
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);  // Yellow for old screen
        init_pair(5, COLOR_RED, COLOR_BLACK);     // Red for exit
    }
    
    vector<string> options = {"New", "Display", "Exit"};
    int selected = 0;
    ScreenState state = MAIN;
    string oldScreen = "Old Screen Content";
    
    while (true) {
        clear();  // Clear screen
        
        int rows = LINES;
        int cols = COLS;
        
        if (state == MAIN) {
            // Calculate starting position for centered menu
            int menu_height = options.size();
            int startY = (rows - menu_height) / 2;
            
            // Find widest option for centering
            int menu_width = 0;
            for (const auto& opt : options) {
                if ((int)opt.length() > menu_width) {
                    menu_width = opt.length();
                }
            }
            menu_width += 2; // for "> " prefix
            
            int startX = (cols - menu_width) / 2;
            if (startX < 0) startX = 0;
            
            // Draw menu options
            for (int i = 0; i < (int)options.size(); ++i) {
                int y = startY + i;
                
                if (i == selected) {
                    attron(COLOR_PAIR(1));
                    mvprintw(y, startX, "> %s", options[i].c_str());
                    attroff(COLOR_PAIR(1));
                } else {
                    attron(COLOR_PAIR(2));
                    mvprintw(y, startX, "  %s", options[i].c_str());
                    attroff(COLOR_PAIR(2));
                }
            }
            
            // Instructions at bottom
            attron(COLOR_PAIR(2));
            mvprintw(rows - 2, 2, "Use Arrow Keys to navigate, Enter to select");
            attroff(COLOR_PAIR(2));
        }
        else if (state == NEW_SCREEN) {
            drawCentered(rows / 2, "Welcome to New Line", 3);
            attron(COLOR_PAIR(2));
            mvprintw(rows - 2, 2, "Press Backspace, Home, or Enter to return");
            attroff(COLOR_PAIR(2));
        }
        else if (state == OLD_SCREEN) {
            drawCentered(rows / 2, oldScreen, 4);
            attron(COLOR_PAIR(2));
            mvprintw(rows - 2, 2, "Press Backspace, Home, or Enter to return");
            attroff(COLOR_PAIR(2));
        }
        
        refresh();  // Update screen
        
        // Get user input
        int ch = getch();
        
        // Handle input based on state
        if (state == MAIN) {
            switch (ch) {
                case KEY_UP:
                case KEY_LEFT:
                    selected--;
                    if (selected < 0) selected = options.size() - 1;
                    break;
                    
                case KEY_DOWN:
                case KEY_RIGHT:
                    selected++;
                    if (selected >= (int)options.size()) selected = 0;
                    break;
                    
                case KEY_HOME:
                    selected = 0;
                    break;
                    
                case KEY_END:
                    selected = options.size() - 1;
                    break;
                    
                case 10:  // Enter key
                case 13:  // Enter key (alternative)
                    if (selected == 0) {
                        state = NEW_SCREEN;
                    } else if (selected == 1) {
                        state = OLD_SCREEN;
                    } else if (selected == 2) {
                        // Exit
                        clear();
                        drawCentered(rows / 2, "Thanks For Using The Program", 5);
                        refresh();
                        usleep(800000);  // 800ms delay
                        endwin();
                        return 0;
                    }
                    break;
            }
        } else {
            // On NEW_SCREEN or OLD_SCREEN
            switch (ch) {
                case KEY_BACKSPACE:
                case 127:  // Backspace
                case 8:    // Backspace (alternative)
                case KEY_HOME:
                case 10:   // Enter
                case 13:   // Enter (alternative)
                    state = MAIN;
                    break;
            }
        }
    }
    
    endwin();  // End ncurses mode
    return 0;
}