#pragma once
#include "state/ToolState.h"

class CursorState : public ToolState {
public:
    void onCursorMove(int dx, int dy) override;
    string getName() const override;
    string getStatusMessage() const override;
};