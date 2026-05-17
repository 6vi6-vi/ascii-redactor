#pragma once
#include "command/Command.h"

class FloodFillCommand : public Command {
private:
    int x, y;
    char newChar;

public:
    FloodFillCommand(Canvas* c, int x, int y, char newChar);
    void execute() override;
    string getDescription() const override;
};