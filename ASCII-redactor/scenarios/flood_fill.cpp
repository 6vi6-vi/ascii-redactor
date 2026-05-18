#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawRectCommand.h"
#include "command/FloodFillCommand.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== Scenario 3: Flood fill ===" << endl;

    Canvas canvas(40, 20);
    CommandHistory history;

    DrawRectCommand* frame = new DrawRectCommand(&canvas, 5, 5, 15, 10, false, '#');
    history.executeCommand(frame);

    FloodFillCommand* fill = new FloodFillCommand(&canvas, 10, 7, '@');
    history.executeCommand(fill);


    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            cout << canvas.getPixel(x, y);
        }
        cout << endl;
    }

    cout << "Scenario completed. Flood fill executed successfully" << endl;

    return 0;
}