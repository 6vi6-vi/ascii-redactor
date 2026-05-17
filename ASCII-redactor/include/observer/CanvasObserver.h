#pragma once
#include <string>

using namespace std;

class Canvas;

class CanvasObserver {
public:
    virtual ~CanvasObserver() = default;
    virtual void onCanvasChanged(const Canvas& canvas) = 0;
    virtual void onStateChanged(const string& message) = 0;
    virtual void onToolChanged(const string& toolName, const string& statusMsg) = 0;
};