#include "platform_compat.h"
#include "ui/EditorContext.h"
#include "ui/InputHandler.h"
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "observer/ConsoleRenderer.h"
#include "state/CursorState.h"
#include <iostream>
#include <csignal>

using namespace std;

// Функция для восстановления терминала при выходе
void cleanup(int signum) {
#ifdef __linux__
    restore_terminal();
#endif
    exit(signum);
}

int readKey() {
    int ch = _getch();

#ifdef __linux__
    // Нормализуем Enter
    ch = normalize_key(ch);

    // Обработка стрелок в Linux
    if (ch == 27) {  // ESC
        int next = _getch();
        if (next == 91) {  // '['
            int arrow = _getch();
            switch (arrow) {
            case 65: return 72;   // вверх
            case 66: return 80;   // вниз
            case 67: return 77;   // вправо
            case 68: return 75;   // влево
            default: return arrow;
            }
        }
        return ch;
    }
#endif

    return ch;
}

int main() {
    // Устанавливаем обработчик для восстановления терминала
    signal(SIGINT, cleanup);

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

    canvas.notifyCanvasChanged();

    while (true) {
        if (!_kbhit()) {
            Sleep(10);
            continue;
        }

        int ch = readKey();
        char key = static_cast<char>(ch);

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
        }
        else if (key == 25) {  // Ctrl+Y
            context.redo();
        }
        else {
            handler.handleKeyPress(key);
        }
    }

    return 0;
}