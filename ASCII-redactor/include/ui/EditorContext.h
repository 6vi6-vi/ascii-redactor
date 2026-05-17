#pragma once
#include <string>

using namespace std;

class ToolState;
class Canvas;
class CommandHistory;
class Command;

class EditorContext {
private:
    ToolState* currentState;
    Canvas* canvas;
    CommandHistory* history;

public:
    EditorContext(Canvas* c, CommandHistory* h);
    ~EditorContext();

    void setState(ToolState* newState);
    ToolState* getState() const;

    Canvas* getCanvas() const;
    CommandHistory* getHistory() const;

    void executeCommand(Command* cmd);
    void undo();
    void redo();

    void moveCursor(int dx, int dy);
    void handleKeyPress(char key);
    void handleCursorMove(int dx, int dy);

    string getCurrentToolName() const;
    string getStatusMessage() const;
    bool isDrawingMode() const;
};