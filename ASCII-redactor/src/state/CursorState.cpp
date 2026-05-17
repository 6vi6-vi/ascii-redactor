#include "state/CursorState.h"
#include "ui/EditorContext.h"
#include "canvas/Canvas.h"

void CursorState::onCursorMove(int dx, int dy) {
    context->getCanvas()->moveCursor(dx, dy);
}

string CursorState::getName() const {
    return "Курсор";
}

string CursorState::getStatusMessage() const {
    return "Режим выбора";
}