#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawRectCommand.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== Scenario 2: Drawing a rectangle ===" << endl;

    Canvas canvas(40, 20);
    CommandHistory history;

    DrawRectCommand* cmd = new DrawRectCommand(&canvas, 5, 5, 15, 10, false, '#');
    history.executeCommand(cmd);


    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            cout << canvas.getPixel(x, y);
        }
        cout << endl;
    }

    cout << "Scenario completed. Rectangle drawn successfully" << endl;

    return 0;
}