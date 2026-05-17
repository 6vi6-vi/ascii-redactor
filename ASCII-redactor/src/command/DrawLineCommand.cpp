#include "command/DrawLineCommand.h"
#include "canvas/Canvas.h"

DrawLineCommand::DrawLineCommand(Canvas* c, int x1, int y1, int x2, int y2, char ch)
    : Command(c), x1(x1), y1(y1), x2(x2), y2(y2), ch(ch) {}

void DrawLineCommand::execute() {
    saveBackup();
    canvas->drawLineImpl(x1, y1, x2, y2, ch);
    canvas->notifyStateChanged("Линия нарисована");
}

string DrawLineCommand::getDescription() const {
    return "Рисование линии от (" + to_string(x1) + "," + to_string(y1) +
        ") до (" + to_string(x2) + "," + to_string(y2) + ")";
}