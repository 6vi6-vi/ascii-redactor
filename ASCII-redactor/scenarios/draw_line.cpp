#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawLineCommand.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== Scenario 1: Drawing a line ===" << endl;

    Canvas canvas(40, 20);
    CommandHistory history;

    DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 10, 10, '#');
    history.executeCommand(cmd);

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            cout << canvas.getPixel(x, y);
        }
        cout << endl;
    }

    cout << "Scenario completed. Line drawn successfully" << endl;

    return 0;
}