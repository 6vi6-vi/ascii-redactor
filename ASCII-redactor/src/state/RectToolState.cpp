#include "state/RectToolState.h"
#include "state/CursorState.h"
#include "ui/EditorContext.h"
#include "canvas/Canvas.h"
#include "command/DrawRectCommand.h"

using namespace std;

void RectToolState::onKeyPress(char key) {
    Canvas* canvas = context->getCanvas();
    if (key == 13) {
        if (!waitingForSecondPoint) {
            startX = canvas->getCursorX();
            startY = canvas->getCursorY();
            waitingForSecondPoint = true;
            canvas->notifyStateChanged("First corner selected. Move cursor to second corner and press Enter");
            canvas->notifyToolChanged(context->getCurrentToolName(), getStatusMessage());
        }
        else {
            Command* cmd = new DrawRectCommand(
                canvas, startX, startY,
                canvas->getCursorX(), canvas->getCursorY(),
                false, canvas->getCurrentChar()
            );
            context->executeCommand(cmd);
            context->setState(new CursorState());
            canvas->notifyStateChanged("Rectangle drawn. Returned to cursor mode");
        }
    }
    else if (key == 27) {
        waitingForSecondPoint = false;
        context->setState(new CursorState());
        canvas->notifyStateChanged("Rectangle drawing mode cancelled");
    }
}

void RectToolState::onCursorMove(int dx, int dy) {
    context->getCanvas()->moveCursor(dx, dy);
}

string RectToolState::getName() const {
    return "Rectangle";
}

string RectToolState::getStatusMessage() const {
    return waitingForSecondPoint ? "Select second corner (Enter)" : "Select first corner (Enter)";
}

bool RectToolState::isDrawingMode() const {
    return true;
}