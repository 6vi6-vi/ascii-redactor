#pragma once
#include "state/ToolState.h"

class RectToolState : public ToolState {
private:
    bool waitingForSecondPoint = false;
    int startX = 0, startY = 0;
public:
    void onKeyPress(char key) override;
    void onCursorMove(int dx, int dy) override;
    string getName() const override;
    string getStatusMessage() const override;
    bool isDrawingMode() const override;
};