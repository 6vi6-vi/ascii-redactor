#include "command/ClearCommand.h"
#include "canvas/Canvas.h"

ClearCommand::ClearCommand(Canvas* c) : Command(c) {}

void ClearCommand::execute() {
    saveBackup();
    canvas->clearImpl();
}

string ClearCommand::getDescription() const {
    return "Очистка всего холста";
}