#include "command/Command.h"
#include "canvas/Canvas.h"
#include "canvas/Memento.h"

Command::Command(Canvas* c) : canvas(c), backup(nullptr) {}
Command::~Command() { delete backup; }

void Command::saveBackup() {
    delete backup;
    backup = new Memento(
        canvas->getGridSnapshot(),
        canvas->getCursorX(),
        canvas->getCursorY(),
        canvas->getCurrentChar()
    );
}

void Command::undo() {
    if (backup) {
        canvas->restoreFromMemento(*backup);
        canvas->notifyStateChanged("Action undone");
    }
}