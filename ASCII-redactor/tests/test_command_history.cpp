#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawLineCommand.h"
#include "command/DrawRectCommand.h"
#include "command/FloodFillCommand.h"
#include "command/ClearCommand.h"

TEST_CASE("CommandHistory", "[history]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Execute command") {
        DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 10, 10, '#');
        history.executeCommand(cmd);

        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '#');
        }
    }

    SECTION("Undo command") {
        DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 10, 10, '#');
        history.executeCommand(cmd);
        history.undo();

        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '.');
        }
    }

    SECTION("Redo command") {
        DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 10, 10, '#');
        history.executeCommand(cmd);
        history.undo();
        history.redo();

        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '#');
        }
    }
}

TEST_CASE("Cursor and char preservation", "[history]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Undo restores cursor position") {
        canvas.moveCursor(15, 8);
        canvas.setCurrentChar('*');

        DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
        history.executeCommand(cmd);

        history.undo();

        REQUIRE(canvas.getCursorX() == 15);
        REQUIRE(canvas.getCursorY() == 8);
        REQUIRE(canvas.getCurrentChar() == '*');
    }
}