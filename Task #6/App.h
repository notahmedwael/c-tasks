#ifndef APP_H
#define APP_H

#include <string>
#include <vector>
#include <filesystem>

enum ScreenState {
    MAIN_SCREEN,
    NEW_CHOICE_SCREEN,
    FILE_LIST_SCREEN,
    FILE_MODE_SCREEN,
    EDITOR_SCREEN,
    DISPLAY_CHOICE_SCREEN,
    DISPLAY_BUFFER_SCREEN,
    DISPLAY_FILE_SCREEN
};

class App {
public:
    App();
    ~App();
    void run();

private:
    ScreenState state;
    int selected;

    // Editor data
    std::vector<std::string> buffer;
    int cursor_x = 0, cursor_y = 0;
    int offset_y = 0; // for editor scrolling
    int max_chars = 1000;

    // File handling
    std::string current_file;
    bool append_mode = true;
    std::vector<std::filesystem::path> file_list;

    // Mode whether file list is for opening (edit/write) or for viewing
    bool selecting_for_display = false;

    // Append-lock (prevents editing original content in append mode)
    int append_lock_x = 0;
    int append_lock_y = 0;

    // Screens
    void drawMainScreen();
    void drawNewChoiceScreen();
    void drawFileListScreen();
    void drawFileModeScreen();
    void drawEditorScreen();
    void drawDisplayChoiceScreen();
    void drawDisplayBufferScreen();
    void drawDisplayFileScreen(const std::string& content);

    // Helpers / input handlers
    void handleMainInput(int ch);
    void handleNewChoiceInput(int ch);
    void handleFileListInput(int ch);
    void handleFileModeInput(int ch);
    void handleEditorInput(int ch);
    void handleDisplayChoiceInput(int ch);

    // Display input handlers (needed so ESC works)
    void handleDisplayBufferInput(int ch);
    void handleDisplayFileInput(int ch);

    void saveBufferToFile();
    void loadFilesFromDir();
    void createFilesDir();
    std::string getFileContent(const std::string& path);
    void showMessage(const std::string& msg);

    int totalCharsInBuffer() const;
    void ensureBufferInvariant();
};

#endif
