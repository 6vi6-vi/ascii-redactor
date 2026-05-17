#include "windows_compat.h"
#include "observer/ConsoleRenderer.h"
#include "canvas/Canvas.h"
#include <iostream>

using namespace std;

ConsoleRenderer::ConsoleRenderer(int w, int h) : width(w), height(h), isDrawingMode(false) {}

void ConsoleRenderer::onCanvasChanged(const Canvas& canvas) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    cout << "=";
    for (int x = 0; x < width + 2; x++) cout << "=";
    cout << "=" << endl;

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

    cout << "=";
    for (int x = 0; x < width + 2; x++) cout << "=";
    cout << "=" << endl;

    cout << " ===============================================================================" << endl;
    cout << "| Текущий символ: [" << canvas.getCurrentChar() << "]     Активный инструмент: " << currentToolName;
    for (int i = 0; i < 35 - (int)currentToolName.length(); i++) cout << " ";
    cout << "|" << endl;

    if (!currentStatusMsg.empty()) {
        SetConsoleTextAttribute(hConsole, 14);
        cout << "| [СТАТУС] " << currentStatusMsg;
        for (int i = 0; i < 58 - (int)currentStatusMsg.length(); i++) cout << " ";
        cout << "|" << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else {
        cout << "|                                                                               |" << endl;
    }

    cout << "| Управление:                                                                   |" << endl;
    cout << "|   [Стрелки] - движение курсора   [L] - линия        [R] - прямоугольник          |" << endl;
    cout << "|   [F] - заливка               [C] - очистить     [U] - отмена (Undo)          |" << endl;
    cout << "|   [S] - сохранить             [O] - загрузить    [H] - помощь                 |" << endl;
    cout << "|   [1-9, символы] - выбрать символ                                             |" << endl;
    cout << "|   [Q] - выход                                                                 |" << endl;
    cout << " ===============================================================================" << endl;
}

void ConsoleRenderer::onStateChanged(const string& message) {
    currentStatusMsg = message;
}

void ConsoleRenderer::onToolChanged(const string& toolName, const string& statusMsg) {
    currentToolName = toolName;
    currentStatusMsg = statusMsg;
}