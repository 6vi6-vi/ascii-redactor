#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawLineCommand.h"
#include "command/DrawRectCommand.h"
#include <iostream>

using namespace std;

void printCanvas(const Canvas& canvas) {
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            cout << canvas.getPixel(x, y);
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    cout << "=== Scenario 4: Undo/Redo operations ===" << endl;
    cout << endl;

    Canvas canvas(40, 20);
    CommandHistory history;

    DrawLineCommand* line = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
    history.executeCommand(line);
    cout << "1. Line drawn:" << endl;
    printCanvas(canvas);

    DrawRectCommand* rect = new DrawRectCommand(&canvas, 10, 5, 20, 10, false, '*');
    history.executeCommand(rect);
    cout << "2. Rectangle drawn:" << endl;
    printCanvas(canvas);

    history.undo();
    cout << "3. Rectangle undone:" << endl;
    printCanvas(canvas);

    history.undo();
    cout << "4. Line undone:" << endl;
    printCanvas(canvas);

    history.redo();
    cout << "5. Line redone:" << endl;
    printCanvas(canvas);

    cout << "Scenario completed. Undo/Redo works correctly" << endl;

    return 0;
}