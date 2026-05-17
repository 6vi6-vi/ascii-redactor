#pragma once
#include <string>

using namespace std;

class Canvas;
class Memento;

class Command {
protected:
    Canvas* canvas;
    Memento* backup;

public:
    Command(Canvas* c);
    virtual ~Command();

    void saveBackup();
    void undo();

    virtual void execute() = 0;
    virtual string getDescription() const = 0;
};