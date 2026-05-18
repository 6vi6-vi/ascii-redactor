#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawLineCommand.h"
#include "command/DrawRectCommand.h"
#include "command/ClearCommand.h"
#include "command/FloodFillCommand.h"

TEST_CASE("Undo/redo scenario: Basic undo after drawing", "[scenario][undo]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Undo line") {
        DrawLineCommand* lineCmd = new DrawLineCommand(&canvas, 0, 0, 10, 10, '#');
        history.executeCommand(lineCmd);

        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '#');
        }

        history.undo();

        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '.');
        }
    }
}

TEST_CASE("Undo/redo scenario: Multiple actions", "[scenario][undo]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Multiple undo") {
        DrawLineCommand* line1 = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
        DrawLineCommand* line2 = new DrawLineCommand(&canvas, 10, 10, 15, 15, '*');

        history.executeCommand(line1);
        history.executeCommand(line2);

        REQUIRE(canvas.getPixel(0, 0) == '#');
        REQUIRE(canvas.getPixel(10, 10) == '*');

        history.undo();
        REQUIRE(canvas.getPixel(10, 10) == '.');
        REQUIRE(canvas.getPixel(0, 0) == '#');

        history.undo();
        REQUIRE(canvas.getPixel(0, 0) == '.');
    }
}

TEST_CASE("Undo/redo scenario: Undo flood fill", "[scenario][undo]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Fill and undo") {
        canvas.setPixel(5, 5, '.');
        FloodFillCommand* fill = new FloodFillCommand(&canvas, 5, 5, '@');
        history.executeCommand(fill);

        REQUIRE(canvas.getPixel(5, 5) == '@');

        history.undo();
        REQUIRE(canvas.getPixel(5, 5) == '.');
    }
}

TEST_CASE("Undo/redo scenario: Undo after clear", "[scenario][undo]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Draw, clear, undo clear") {
        DrawLineCommand* line = new DrawLineCommand(&canvas, 0, 0, 10, 10, '#');
        history.executeCommand(line);

        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '#');
        }

        ClearCommand* clear = new ClearCommand(&canvas);
        history.executeCommand(clear);

        REQUIRE(canvas.getPixel(0, 0) == '.');

        history.undo();

        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '#');
        }
    }
}

TEST_CASE("Undo/redo scenario: Empty history", "[scenario][undo]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Undo on empty history does nothing") {
        history.undo();
        SUCCEED();
    }

    SECTION("Redo on empty history does nothing") {
        history.redo();
        SUCCEED();
    }
}

TEST_CASE("Undo/redo scenario: Rectangle operations", "[scenario][undo]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Draw rectangle and undo") {
        DrawRectCommand* rect = new DrawRectCommand(&canvas, 5, 5, 15, 10, false, '#');
        history.executeCommand(rect);

        for (int x = 5; x <= 15; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '#');
        }

        history.undo();

        for (int x = 5; x <= 15; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '.');
        }
    }
}