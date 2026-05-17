#pragma once
#include "command/Command.h"

class DrawLineCommand : public Command {
private:
    int x1, y1, x2, y2;
    char ch;

public:
    DrawLineCommand(Canvas* c, int x1, int y1, int x2, int y2, char ch);
    void execute() override;
    string getDescription() const override;
};