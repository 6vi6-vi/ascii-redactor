#include "windows_compat.h"
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
            canvas->notifyStateChanged("Первая точка выбрана. Переместите курсор ко второй точке и нажмите Enter");
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
            canvas->notifyStateChanged("Линия нарисована. Возврат в режим курсора");
        }
    }
    else if (key == 27) {
        waitingForSecondPoint = false;
        context->setState(new CursorState());
        canvas->notifyStateChanged("Режим рисования линии отменен");
    }
}

void LineToolState::onCursorMove(int dx, int dy) {
    context->getCanvas()->moveCursor(dx, dy);
}

string LineToolState::getName() const {
    return "Линия";
}

string LineToolState::getStatusMessage() const {
    return waitingForSecondPoint ? "Выберите вторую точку линии (Enter)" : "Выберите первую точку линии (Enter)";
}

bool LineToolState::isDrawingMode() const {
    return true;
}