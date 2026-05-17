#include "command/CommandHistory.h"

CommandHistory::~CommandHistory() {
    while (!undoStack.empty()) {
        delete undoStack.top();
        undoStack.pop();
    }
    while (!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }
}

void CommandHistory::executeCommand(Command* cmd) {
    cmd->execute();
    undoStack.push(cmd);
    while (!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }
}

void CommandHistory::undo() {
    if (undoStack.empty()) return;
    Command* cmd = undoStack.top();
    undoStack.pop();
    cmd->undo();
    redoStack.push(cmd);
}

void CommandHistory::redo() {
    if (redoStack.empty()) return;
    Command* cmd = redoStack.top();
    redoStack.pop();
    cmd->execute();
    undoStack.push(cmd);
}