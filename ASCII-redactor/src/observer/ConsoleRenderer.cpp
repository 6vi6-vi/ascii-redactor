#include "windows_compat.h"
#include "observer/ConsoleRenderer.h"
#include "canvas/Canvas.h"
#include <iostream>
#include <string>

using namespace std;

ConsoleRenderer::ConsoleRenderer(int w, int h) : width(w), height(h), isDrawingMode(false) {}

void ConsoleRenderer::onCanvasChanged(const Canvas& canvas) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    // Top border
    cout << "=";
    for (int x = 0; x < width + 2; x++) cout << "=";
    cout << "=" << endl;

    // Canvas
    for (int y = 0; y < height; y++) {
        cout << "| ";
        for (int x = 0; x < width; x++) {
            if (x == canvas.getCursorX() && y == canvas.getCursorY()) {
                SetConsoleTextAttribute(hConsole, 112);
                cout << canvas.getPixel(x, y);
                SetConsoleTextAttribute(hConsole, 7);
            }
            else {
                cout << canvas.getPixel(x, y);
            }
        }
        cout << " |" << endl;
    }

    // Bottom border
    cout << "=";
    for (int x = 0; x < width + 2; x++) cout << "=";
    cout << "=" << endl;

    // Control panel
    cout << " ===============================================================================" << endl;
    cout << "| Current char: [" << canvas.getCurrentChar() << "]     Active tool: " << currentToolName;
    for (int i = 0; i < 43 - (int)currentToolName.length(); i++) cout << " ";
    cout << "|" << endl;

    if (!currentStatusMsg.empty()) {
        SetConsoleTextAttribute(hConsole, 14);
        cout << "| [STATUS] " << currentStatusMsg;
        for (int i = 0; i < 69 - (int)currentStatusMsg.length(); i++) cout << " ";
        cout << "|" << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else {
        cout << "|                                                                               |" << endl;
    }

    cout << "| Controls:                                                                     |" << endl;
    cout << "|   [Arrows] - move cursor   [L] - line        [R] - rectangle                  |" << endl;
    cout << "|   [F] - flood fill       [C] - clear        [U] - undo                        |" << endl;
    cout << "|   [S] - save             [O] - load         [H] - help                        |" << endl;
    cout << "|   [1-9, chars] - select character                                             |" << endl;
    cout << "|   [Q] - exit                                                                  |" << endl;
    cout << " ===============================================================================" << endl;
}

void ConsoleRenderer::onStateChanged(const string& message) {
    currentStatusMsg = message;
}

void ConsoleRenderer::onToolChanged(const string& toolName, const string& statusMsg) {
    currentToolName = toolName;
    currentStatusMsg = statusMsg;
}