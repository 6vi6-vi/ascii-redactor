#include "command/FloodFillCommand.h"
#include "canvas/Canvas.h"

FloodFillCommand::FloodFillCommand(Canvas* c, int x, int y, char newChar)
    : Command(c), x(x), y(y), newChar(newChar) {}

void FloodFillCommand::execute() {
    saveBackup();
    canvas->floodFillImpl(x, y, newChar);
}

string FloodFillCommand::getDescription() const {
    return "Заливка в точке (" + to_string(x) + "," + to_string(y) + ")";
}