#include "state/LineToolState.h"
#include "state/CursorState.h"
#include "ui/EditorContext.h"
#include "canvas/Canvas.h"
#include "command/DrawLineCommand.h"

using namespace std;

void LineToolState::onKeyPress(char key) {
    Canvas* canvas = context->getCanvas();
    if (key == 13) {
        if (!waitingForSecondPoint) {
            startX = canvas->getCursorX();
            startY = canvas->getCursorY();
            waitingForSecondPoint = true;
            canvas->notifyStateChanged("First point selected. Move cursor to second point and press Enter");
            canvas->notifyToolChanged(context->getCurrentToolName(), getStatusMessage());
        }
        else {
            Command* cmd = new DrawLineCommand(
                canvas, startX, startY,
                canvas->getCursorX(), canvas->getCursorY(),
                canvas->getCurrentChar()
            );
            context->executeCommand(cmd);
            context->setState(new CursorState());
            canvas->notifyStateChanged("Line drawn. Returned to cursor mode");
        }
    }
    else if (key == 27) {
        waitingForSecondPoint = false;
        context->setState(new CursorState());
        canvas->notifyStateChanged("Line drawing mode cancelled");
    }
}

void LineToolState::onCursorMove(int dx, int dy) {
    context->getCanvas()->moveCursor(dx, dy);
}

string LineToolState::getName() const {
    return "Line";
}

string LineToolState::getStatusMessage() const {
    return waitingForSecondPoint ? "Select second point (Enter)" : "Select first point (Enter)";
}

bool LineToolState::isDrawingMode() const {
    return true;
}