#pragma once
#include <stack>
#include "command/Command.h"

using namespace std;

class CommandHistory {
private:
    stack<Command*> undoStack;
    stack<Command*> redoStack;

public:
    CommandHistory() {}
    ~CommandHistory();

    void executeCommand(Command* cmd);
    void undo();
    void redo();
};