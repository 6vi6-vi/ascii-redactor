#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawLineCommand.h"
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