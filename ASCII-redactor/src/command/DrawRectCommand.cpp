#include "command/DrawRectCommand.h"
#include "canvas/Canvas.h"

DrawRectCommand::DrawRectCommand(Canvas* c, int x1, int y1, int x2, int y2, bool fill, char ch)
    : Command(c), x1(x1), y1(y1), x2(x2), y2(y2), fill(fill), ch(ch) {}

void DrawRectCommand::execute() {
    saveBackup();
    canvas->drawRectImpl(x1, y1, x2, y2, fill, ch);
    canvas->notifyStateChanged("Прямоугольник нарисован");
}

string DrawRectCommand::getDescription() const {
    return "Рисование прямоугольника";
}