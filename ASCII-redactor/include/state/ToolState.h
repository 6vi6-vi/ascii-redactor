#pragma once
#include <string>

using namespace std;

class EditorContext;

class ToolState {
protected:
    EditorContext* context;
public:
    virtual ~ToolState() = default;
    void setContext(EditorContext* ctx) { context = ctx; }

    virtual void onKeyPress(char key) {}
    virtual void onCursorMove(int dx, int dy) {}
    virtual string getName() const { return "Инструмент"; }
    virtual string getStatusMessage() const { return ""; }
    virtual bool isDrawingMode() const { return false; }
};