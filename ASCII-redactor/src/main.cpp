#include "windows_compat.h"
#include "ui/EditorContext.h"
#include "ui/InputHandler.h"
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "observer/ConsoleRenderer.h"
#include "state/CursorState.h"
#include <iostream>

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");

    int width, height;

    cout << "==========================================================" << endl;
    cout << "|                   ASCII DRAW STUDIO                    |" << endl;
    cout << "|                 Редактор псевдографики                 |" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    cout << "Введите размеры холста (от 40x20 до 200x100)" << endl;
    cout << "Ширина (40-200): ";
    cin >> width;
    cout << "Высота (20-100): ";
    cin >> height;

    Canvas canvas(width, height);
    CommandHistory history;
    EditorContext context(&canvas, &history);

    ConsoleRenderer renderer(width, height);
    canvas.attachObserver(&renderer);

    context.setState(new CursorState());
    InputHandler handler(&context);

    canvas.notifyCanvasChanged();

    while (true) {
        if (!_kbhit()) {
            Sleep(10);
            continue;
        }

        int ch = _getch();
        char key = static_cast<char>(ch);

        if (key == -32 || key == 224) {
            key = _getch();
            switch (key) {
            case 72: handler.handleCursorMove(0, -1); break;
            case 80: handler.handleCursorMove(0, 1); break;
            case 75: handler.handleCursorMove(-1, 0); break;
            case 77: handler.handleCursorMove(1, 0); break;
            }
        }
        else {
            if (key == 26) {
                context.undo();
                canvas.notifyCanvasChanged();
            }
            else if (key == 25) {
                context.redo();
                canvas.notifyCanvasChanged();
            }
            else {
                handler.handleKeyPress(key);
            }
        }
    }

    return 0;
}