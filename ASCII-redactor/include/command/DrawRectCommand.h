#pragma once
#include "command/Command.h"

class DrawRectCommand : public Command {
private:
    int x1, y1, x2, y2;
    bool fill;
    char ch;

public:
    DrawRectCommand(Canvas* c, int x1, int y1, int x2, int y2, bool fill, char ch);
    void execute() override;
    string getDescription() const override;
};