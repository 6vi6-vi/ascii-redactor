#pragma once

class EditorContext;

class InputHandler {
private:
    EditorContext* context;
    void showHelp();

public:
    InputHandler(EditorContext* ctx);
    void handleKeyPress(char key);
    void handleCursorMove(int dx, int dy);
};