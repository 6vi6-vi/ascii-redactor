#pragma once
#include "observer/CanvasObserver.h"
#include <string>

using namespace std;

class ConsoleRenderer : public CanvasObserver {
private:
    int width;
    int height;
    string currentToolName;
    string currentStatusMsg;
    bool isDrawingMode;

public:
    ConsoleRenderer(int w, int h);
    void onCanvasChanged(const Canvas& canvas) override;
    void onStateChanged(const string& message) override;
    void onToolChanged(const string& toolName, const string& statusMsg) override;
};