#include "windows_compat.h"
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
            canvas->notifyStateChanged("Первый угол выбран. Переместите курсор ко второму углу и нажмите Enter");
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
            canvas->notifyStateChanged("Прямоугольник нарисован. Возврат в режим курсора");
        }
    }
    else if (key == 27) {
        waitingForSecondPoint = false;
        context->setState(new CursorState());
        canvas->notifyStateChanged("Режим рисования прямоугольника отменен");
    }
}

void RectToolState::onCursorMove(int dx, int dy) {
    context->getCanvas()->moveCursor(dx, dy);
}

string RectToolState::getName() const {
    return "Прямоугольник";
}

string RectToolState::getStatusMessage() const {
    return waitingForSecondPoint ? "Выберите второй угол (Enter)" : "Выберите первый угол (Enter)";
}

bool RectToolState::isDrawingMode() const {
    return true;
}