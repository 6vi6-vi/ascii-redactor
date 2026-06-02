#include "platform_compat.h"
#include "ui/InputHandler.h"
#include "ui/EditorContext.h"
#include "state/LineToolState.h"
#include "state/RectToolState.h"
#include "command/FloodFillCommand.h"
#include "command/ClearCommand.h"
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include <iostream>

using namespace std;

void InputHandler::showHelp() {
    platform_clear_screen();
    cout << "==================== HELP ====================" << endl;
    cout << "Cursor control:" << endl;
    cout << "  Arrows - move cursor" << endl;
    cout << endl;
    cout << "Tools:" << endl;
    cout << "  L - line drawing mode" << endl;
    cout << "  R - rectangle drawing mode" << endl;
    cout << "  F - flood fill" << endl;
    cout << "  C - clear canvas" << endl;
    cout << "  U - undo" << endl;
    cout << "  Ctrl+Z - undo" << endl;
    cout << "  Ctrl+Y - redo" << endl;
    cout << endl;
    cout << "File operations:" << endl;
    cout << "  S - save to file (.ascii or .txt)" << endl;
    cout << "  O - load from file (.ascii or .txt)" << endl;
    cout << endl;
    cout << "Characters:" << endl;
    cout << "  Any printable character - set current character" << endl;
    cout << endl;
    cout << "Other:" << endl;
    cout << "  H - show this help" << endl;
    cout << "  Q - exit" << endl;
    cout << endl;
    cout << "================================================" << endl;
    cout << "Press any key to continue...";
    _getch();
    context->getCanvas()->notifyCanvasChanged();
}

InputHandler::InputHandler(EditorContext* ctx) : context(ctx) {}

void InputHandler::handleKeyPress(char key) {

    if (key == 'L' || key == 'l') {
        if (!context->isDrawingMode()) {
            context->setState(new LineToolState());
            context->getCanvas()->notifyCanvasChanged();
        }
        return;
    }

    if (key == 'R' || key == 'r') {
        if (!context->isDrawingMode()) {
            context->setState(new RectToolState());
            context->getCanvas()->notifyCanvasChanged();
        }
        return;
    }

    if (!context->isDrawingMode()) {
        switch (key) {
        case 'F': case 'f': {
            Command* cmd = new FloodFillCommand(
                context->getCanvas(),
                context->getCanvas()->getCursorX(),
                context->getCanvas()->getCursorY(),
                context->getCanvas()->getCurrentChar()
            );
            context->executeCommand(cmd);
            context->getCanvas()->notifyCanvasChanged();
            return;
        }
        case 'C': case 'c': {
            Command* cmd = new ClearCommand(context->getCanvas());
            context->executeCommand(cmd);
            context->getCanvas()->notifyCanvasChanged();
            return;
        }
        case 'U': case 'u':
            context->undo();
            context->getCanvas()->notifyCanvasChanged();
            return;
        case 'S': case 's': {
            string filename;
            cout << "Save file name (.ascii): ";
            cin >> filename;
            if (filename.find('.') == string::npos) filename += ".ascii";

            // Сохраняем в папку /app/saves (которая смонтирована на хост)
            string fullPath = "/app/saves/" + filename;

            if (context->getCanvas()->saveToFile(fullPath)) {
                cout << "Saved to: " << fullPath << endl;
                cout << "File is available on your host in the 'saves' folder" << endl;
            }
            else {
                cout << "Save failed!" << endl;
            }
            _getch();
            context->getCanvas()->notifyCanvasChanged();
            return;
        }
        case 'O': case 'o': {
            string filename;
            cout << "Load file name (.ascii): ";
            cin >> filename;

            // Ищем в папке /app/saves
            string fullPath = "/app/saves/" + filename;

            if (context->getCanvas()->loadFromFile(fullPath)) {
                cout << "Loaded from: " << fullPath << endl;
            }
            else {
                // Если не нашли, пробуем как есть (для обратной совместимости)
                cout << "File not found in saves folder, trying current directory..." << endl;
                if (context->getCanvas()->loadFromFile(filename)) {
                    cout << "Loaded from current directory: " << filename << endl;
                }
                else {
                    cout << "Load failed!" << endl;
                }
            }
            _getch();
            context->getCanvas()->notifyCanvasChanged();
            return;
        }
        case 'H': case 'h':
            showHelp();
            return;
        case 'Q': case 'q':
            exit(0);
            return;
        default:
            if (key >= 32 && key <= 126) {
                context->getCanvas()->setCurrentChar(key);
            }
            return;
        }
    }

    context->handleKeyPress(key);
    context->getCanvas()->notifyCanvasChanged();
}

void InputHandler::handleCursorMove(int dx, int dy) {
    context->handleCursorMove(dx, dy);
}