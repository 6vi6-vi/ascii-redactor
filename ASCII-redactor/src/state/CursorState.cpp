#include "state/CursorState.h"
#include "ui/EditorContext.h"
#include "canvas/Canvas.h"

void CursorState::onCursorMove(int dx, int dy) {
    context->getCanvas()->moveCursor(dx, dy);
}

string CursorState::getName() const {
    return "Cursor";
}

string CursorState::getStatusMessage() const {
    return "Selection mode";
}