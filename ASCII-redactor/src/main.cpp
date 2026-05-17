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
    int width, height;

    cout << "==========================================================" << endl;
    cout << "|                   ASCII PAINT                          |" << endl;
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

        char key = _getch();

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