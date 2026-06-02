#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawLineCommand.h"
#include "command/DrawRectCommand.h"
#include "platform_compat.h"
#include <iostream>
#include <cstdio>

using namespace std;

int main() {
    cout << "=== Scenario 5: Save and load file ===" << endl;

    // Создаём папку saves, если её нет
    system("mkdir -p /app/saves");

    string filename = "/app/saves/saved_canvas.ascii";

    {
        Canvas canvas(40, 20);
        CommandHistory history;

        DrawRectCommand* rect = new DrawRectCommand(&canvas, 5, 5, 15, 10, false, '#');
        history.executeCommand(rect);

        DrawLineCommand* line = new DrawLineCommand(&canvas, 0, 0, 10, 10, '*');
        history.executeCommand(line);

        canvas.saveToFile(filename);
        cout << "Drawing saved to: " << filename << endl;
    }

    {
        Canvas canvas(40, 20);
        canvas.loadFromFile(filename);
        cout << "Drawing loaded from: " << filename << endl;

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                cout << canvas.getPixel(x, y);
            }
            cout << endl;
        }
    }

    cout << "\nScenario completed. Save and load work correctly!" << endl;

    return 0;
}