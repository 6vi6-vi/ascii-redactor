#include "ui/EditorContext.h"
#include "state/ToolState.h"
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"

EditorContext::EditorContext(Canvas* c, CommandHistory* h)
    : currentState(nullptr), canvas(c), history(h) {}

EditorContext::~EditorContext() {
    delete currentState;
}

void EditorContext::setState(ToolState* newState) {
    delete currentState;
    currentState = newState;
    if (currentState) {
        currentState->setContext(this);
    }
    canvas->notifyToolChanged(getCurrentToolName(), getStatusMessage());
}

ToolState* EditorContext::getState() const {
    return currentState;
}

Canvas* EditorContext::getCanvas() const {
    return canvas;
}

CommandHistory* EditorContext::getHistory() const {
    return history;
}

void EditorContext::executeCommand(Command* cmd) {
    history->executeCommand(cmd);
}

void EditorContext::undo() {
    history->undo();
}

void EditorContext::redo() {
    history->redo();
}

void EditorContext::moveCursor(int dx, int dy) {
    canvas->moveCursor(dx, dy);
}

void EditorContext::handleKeyPress(char key) {
    if (currentState) {
        currentState->onKeyPress(key);
    }
    canvas->notifyToolChanged(getCurrentToolName(), getStatusMessage());
}

void EditorContext::handleCursorMove(int dx, int dy) {
    if (currentState) {
        currentState->onCursorMove(dx, dy);
    }
    else {
        canvas->moveCursor(dx, dy);
    }
}

string EditorContext::getCurrentToolName() const {
    return currentState ? currentState->getName() : "Cursor";
}

string EditorContext::getStatusMessage() const {
    return currentState ? currentState->getStatusMessage() : "";
}

bool EditorContext::isDrawingMode() const {
    return currentState ? currentState->isDrawingMode() : false;
}