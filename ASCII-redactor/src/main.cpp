#include "platform_compat.h"
#include "ui/EditorContext.h"
#include "ui/InputHandler.h"
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "observer/ConsoleRenderer.h"
#include "state/CursorState.h"
#include <iostream>

using namespace std;

// Функция для чтения клавиш с поддержкой стрелок в Linux
int readKey() {
    int ch = _getch();

    // В Linux стрелки передаются как escape последовательности
    if (ch == 27) {  // ESC
        int next = _getch();
        if (next == 91) {  // '['
            int arrow = _getch();
            switch (arrow) {
            case 65: return 72;   // Стрелка вверх - код 72 (как в Windows)
            case 66: return 80;   // Стрелка вниз - код 80
            case 67: return 77;   // Стрелка вправо - код 77
            case 68: return 75;   // Стрелка влево - код 75
            default: return arrow;
            }
        }
        return ch;
    }

    return ch;
}

int main() {
    int width, height;

    cout << "==========================================================" << endl;
    cout << "|                     ASCII PAINT                        |" << endl;
    cout << "|                 ASCII Graphics Editor                  |" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    cout << "Enter canvas size (40x20 to 200x100)" << endl;
    cout << "Width (40-200): ";
    cin >> width;
    cout << "Height (20-100): ";
    cin >> height;

    Canvas canvas(width, height);
    CommandHistory history;
    EditorContext context(&canvas, &history);

    ConsoleRenderer renderer(width, height);
    canvas.attachObserver(&renderer);

    context.setState(new CursorState());
    InputHandler handler(&context);

    // Начальная отрисовка
    canvas.notifyCanvasChanged();

    while (true) {
        if (!_kbhit()) {
            Sleep(10);
            continue;
        }

        int ch = readKey();
        char key = static_cast<char>(ch);

        // Обработка стрелок (коды 72,80,75,77 как в Windows)
        if (ch == 72) {  // вверх
            handler.handleCursorMove(0, -1);
        }
        else if (ch == 80) {  // вниз
            handler.handleCursorMove(0, 1);
        }
        else if (ch == 75) {  // влево
            handler.handleCursorMove(-1, 0);
        }
        else if (ch == 77) {  // вправо
            handler.handleCursorMove(1, 0);
        }
        else if (key == 26) {  // Ctrl+Z
            context.undo();
            canvas.notifyCanvasChanged();
        }
        else if (key == 25) {  // Ctrl+Y
            context.redo();
            canvas.notifyCanvasChanged();
        }
        else {
            handler.handleKeyPress(key);
        }
    }

    return 0;
}