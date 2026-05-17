#pragma once
#include "command/Command.h"

class ClearCommand : public Command {
public:
    ClearCommand(Canvas* c);
    void execute() override;
    string getDescription() const override;
};