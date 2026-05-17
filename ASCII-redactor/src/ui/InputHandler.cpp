#include "windows_compat.h"
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
    system("cls");
    cout << "==================== ПОМОЩЬ ====================" << endl;
    cout << "Управление курсором:" << endl;
    cout << "  Стрелки - перемещение курсора" << endl;
    cout << endl;
    cout << "Инструменты:" << endl;
    cout << "  L - режим рисования линии" << endl;
    cout << "  R - режим рисования прямоугольника" << endl;
    cout << "  F - заливка области" << endl;
    cout << "  C - очистить весь холст" << endl;
    cout << "  U - отменить последнее действие" << endl;
    cout << "  Ctrl+Z - отменить (альтернатива)" << endl;
    cout << "  Ctrl+Y - повторить" << endl;
    cout << endl;
    cout << "Работа с файлами:" << endl;
    cout << "  S - сохранить в файл (.ascii или .txt)" << endl;
    cout << "  O - загрузить из файла (.ascii или .txt)" << endl;
    cout << endl;
    cout << "Символы:" << endl;
    cout << "  Любой печатный символ - установить текущий символ" << endl;
    cout << "  1-9 - быстрый выбор: @ # % * + - = | /" << endl;
    cout << endl;
    cout << "Прочее:" << endl;
    cout << "  H - показать эту справку" << endl;
    cout << "  Q - выход" << endl;
    cout << endl;
    cout << "================================================" << endl;
    cout << "Нажмите любую клавишу для продолжения...";
    _getch();
    context->getCanvas()->notifyCanvasChanged();
}

InputHandler::InputHandler(EditorContext* ctx) : context(ctx) {}

void InputHandler::handleKeyPress(char key) {
    switch (key) {
    case '1': context->getCanvas()->setCurrentChar('@'); return;
    case '2': context->getCanvas()->setCurrentChar('#'); return;
    case '3': context->getCanvas()->setCurrentChar('%'); return;
    case '4': context->getCanvas()->setCurrentChar('*'); return;
    case '5': context->getCanvas()->setCurrentChar('+'); return;
    case '6': context->getCanvas()->setCurrentChar('-'); return;
    case '7': context->getCanvas()->setCurrentChar('='); return;
    case '8': context->getCanvas()->setCurrentChar('|'); return;
    case '9': context->getCanvas()->setCurrentChar('/'); return;
    }

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
            cout << "Имя файла для сохранения (.ascii): ";
            cin >> filename;
            if (filename.find('.') == string::npos) filename += ".ascii";
            context->getCanvas()->saveToFile(filename);
            _getch();
            context->getCanvas()->notifyCanvasChanged();
            return;
        }
        case 'O': case 'o': {
            string filename;
            cout << "Имя файла для загрузки (.ascii или .txt): ";
            cin >> filename;
            context->getCanvas()->loadFromFile(filename);
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