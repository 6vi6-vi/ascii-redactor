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

    SECTION("Multiple undo") {
        DrawLineCommand* cmd1 = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
        DrawLineCommand* cmd2 = new DrawLineCommand(&canvas, 10, 10, 15, 15, '*');

        history.executeCommand(cmd1);
        history.executeCommand(cmd2);

        history.undo();
        REQUIRE(canvas.getPixel(10, 10) == '.');
        REQUIRE(canvas.getPixel(0, 0) == '#');

        history.undo();
        REQUIRE(canvas.getPixel(0, 0) == '.');
    }
}


TEST_CASE("Undo/redo limits", "[history]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Undo on empty history") {
        history.undo();
        SUCCEED();
    }

    SECTION("Redo on empty history") {
        history.redo();
        SUCCEED();
    }

    SECTION("Redo after undo") {
        DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 10, 10, '#');
        history.executeCommand(cmd);

        history.undo();
        REQUIRE(canvas.getPixel(5, 5) == '.');

        history.redo();
        REQUIRE(canvas.getPixel(5, 5) == '#');
    }

    SECTION("Multiple redo after multiple undo") {
        DrawLineCommand* cmd1 = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
        DrawLineCommand* cmd2 = new DrawLineCommand(&canvas, 10, 10, 15, 15, '*');

        history.executeCommand(cmd1);
        history.executeCommand(cmd2);

        history.undo();
        history.undo();

        REQUIRE(canvas.getPixel(0, 0) == '.');
        REQUIRE(canvas.getPixel(10, 10) == '.');

        history.redo();
        REQUIRE(canvas.getPixel(0, 0) == '#');
        REQUIRE(canvas.getPixel(10, 10) == '.');

        history.redo();
        REQUIRE(canvas.getPixel(0, 0) == '#');
        REQUIRE(canvas.getPixel(10, 10) == '*');
    }
}

TEST_CASE("Different command types", "[history]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Mix DrawLine and DrawRect") {
        DrawLineCommand* line = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
        DrawRectCommand* rect = new DrawRectCommand(&canvas, 20, 10, 30, 15, false, '*');

        history.executeCommand(line);
        history.executeCommand(rect);

        REQUIRE(canvas.getPixel(2, 2) == '#');
        REQUIRE(canvas.getPixel(25, 10) == '*');

        history.undo();
        REQUIRE(canvas.getPixel(25, 10) == '.');
        REQUIRE(canvas.getPixel(2, 2) == '#'); 

        history.undo();
        REQUIRE(canvas.getPixel(2, 2) == '.');
    }

    SECTION("Mix FloodFill and Clear") {
        canvas.setPixel(5, 5, '.');
        canvas.setPixel(6, 6, '.');

        FloodFillCommand* fill = new FloodFillCommand(&canvas, 5, 5, '@');
        ClearCommand* clear = new ClearCommand(&canvas);

        history.executeCommand(fill);
        REQUIRE(canvas.getPixel(5, 5) == '@');

        history.executeCommand(clear);
        REQUIRE(canvas.getPixel(5, 5) == '.');

        history.undo();
        REQUIRE(canvas.getPixel(5, 5) == '@');

        history.undo();
        REQUIRE(canvas.getPixel(5, 5) == '.');
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

TEST_CASE("Many operations", "[history]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Undo 10 sequential commands ") {
        for (int i = 0; i < 10; i++) {
            DrawLineCommand* cmd = new DrawLineCommand(&canvas, i, i, i + 2, i + 2, '#');
            history.executeCommand(cmd);
        }

        REQUIRE(canvas.getPixel(9, 9) == '#');

        for (int i = 0; i < 10; i++) {
            history.undo();
        }

        for (int i = 0; i <= 11; i++) {
            REQUIRE(canvas.getPixel(i, i) == '.');
        }
    }

    SECTION("Undo and redo 10 commands ") {
        for (int i = 0; i < 5; i++) {
            DrawLineCommand* cmd = new DrawLineCommand(&canvas, i, i, i + 2, i + 2, '#');
            history.executeCommand(cmd);
        }

        for (int i = 0; i < 5; i++) {
            history.undo();
        }

        for (int i = 0; i < 5; i++) {
            history.redo();
        }

        REQUIRE(canvas.getPixel(4, 4) == '#');
    }
}

TEST_CASE("Memory management", "[history]") {
    Canvas canvas(40, 20);

    SECTION("Commands are properly deleted") {
        CommandHistory* history = new CommandHistory();

        DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
        history->executeCommand(cmd);

        delete history;
        SUCCEED();
    }
}