#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include "command/DrawLineCommand.h"
#include "command/DrawRectCommand.h"
#include "command/FloodFillCommand.h"
#include "command/ClearCommand.h"
#include <string>

using namespace std;


TEST_CASE("Command saveBackup tests", "[commands]") {
    Canvas canvas(40, 20);

    SECTION("saveBackup saves current state before changes") {
        canvas.setPixel(5, 5, '#');
        canvas.moveCursor(10, 10);
        canvas.setCurrentChar('*');

        DrawLineCommand cmd(&canvas, 0, 0, 5, 5, '@');

        cmd.saveBackup();

        canvas.setPixel(5, 5, '.');
        canvas.moveCursor(0, 0);
        canvas.setCurrentChar('.');

        cmd.undo();

        REQUIRE(canvas.getPixel(5, 5) == '#');
        REQUIRE(canvas.getCursorX() == 10);
        REQUIRE(canvas.getCursorY() == 10);
        REQUIRE(canvas.getCurrentChar() == '*');
    }

    SECTION("saveBackup called multiple times") {
        canvas.setPixel(5, 5, '#');

        DrawLineCommand cmd(&canvas, 0, 0, 5, 5, '@');

        cmd.saveBackup();
        canvas.setPixel(5, 5, '*');

        cmd.saveBackup();

        cmd.undo();

        REQUIRE(canvas.getPixel(5, 5) == '*');
    }

    SECTION("undo without saveBackup") {
        DrawLineCommand cmd(&canvas, 0, 0, 5, 5, '#');

        cmd.undo();  
        SUCCEED();
    }
}

TEST_CASE("DrawLineCommand", "[commands]") {
    Canvas canvas(40, 20);

    SECTION("Execute") {
        DrawLineCommand cmd(&canvas, 0, 0, 10, 10, '#');
        cmd.execute();

        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '#');
        }
    }

    SECTION("Description") {
        DrawLineCommand cmd(&canvas, 0, 0, 5, 5, '@');
        REQUIRE(cmd.getDescription().find("Draw line") != std::string::npos);
    }

    SECTION("Undo removes line") {
        DrawLineCommand cmd(&canvas, 0, 0, 10, 10, '#');

        cmd.execute();
        REQUIRE(canvas.getPixel(5, 5) == '#');

        cmd.undo();
        REQUIRE(canvas.getPixel(5, 5) == '.');
    }

    SECTION("Undo after multiple lines") {
        DrawLineCommand cmd1(&canvas, 0, 0, 5, 5, '#');
        DrawLineCommand cmd2(&canvas, 10, 10, 15, 15, '*');

        cmd1.execute();
        cmd2.execute();

        REQUIRE(canvas.getPixel(2, 2) == '#');
        REQUIRE(canvas.getPixel(12, 12) == '*');

        cmd2.undo();
        REQUIRE(canvas.getPixel(2, 2) == '#');
        REQUIRE(canvas.getPixel(12, 12) == '.');
    }

    SECTION("Undo restores cursor position and current char") {
        canvas.moveCursor(5, 5);
        canvas.setCurrentChar('*');

        DrawLineCommand cmd(&canvas, 0, 0, 5, 5, '#');

        cmd.execute();

        cmd.undo();

        REQUIRE(canvas.getCursorX() == 5);
        REQUIRE(canvas.getCursorY() == 5);
        REQUIRE(canvas.getCurrentChar() == '*');
    }
}

TEST_CASE("DrawRectCommand", "[commands]") {
    Canvas canvas(40, 20);

    SECTION("Execute") {
        DrawRectCommand cmd(&canvas, 5, 5, 15, 10, false, '#');
        cmd.execute();

        for (int x = 5; x <= 15; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '#');
            REQUIRE(canvas.getPixel(x, 10) == '#');
        }
    }

    SECTION("With swapped coordinates") {
        DrawRectCommand cmd(&canvas, 15, 10, 5, 5, false, '#');
        cmd.execute();

        for (int x = 5; x <= 15; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '#');
        }
    }

    SECTION("Undo removes rectangle") {
        DrawRectCommand cmd(&canvas, 5, 5, 15, 10, false, '#');

        cmd.execute();
        REQUIRE(canvas.getPixel(10, 5) == '#');

        cmd.undo();
        REQUIRE(canvas.getPixel(10, 5) == '.');
    }

    SECTION("Undo for filled rectangle") {
        DrawRectCommand cmd(&canvas, 5, 5, 10, 10, true, '@');

        cmd.execute();
        REQUIRE(canvas.getPixel(7, 7) == '@');

        cmd.undo();
        REQUIRE(canvas.getPixel(7, 7) == '.');
    }
}

TEST_CASE("FloodFillCommand", "[commands]") {
    Canvas canvas(40, 20);

    SECTION("Execute") {
        canvas.setPixel(5, 5, '.');
        FloodFillCommand cmd(&canvas, 5, 5, '@');
        cmd.execute();
        REQUIRE(canvas.getPixel(5, 5) == '@');
    }

    SECTION("Same character") {
        canvas.setPixel(5, 5, '@');
        FloodFillCommand cmd(&canvas, 5, 5, '@');
        cmd.execute();
        REQUIRE(canvas.getPixel(5, 5) == '@');
    }

    SECTION("Undo restores original area") {
        canvas.setPixel(5, 5, '.');
        canvas.setPixel(5, 6, '.');
        canvas.setPixel(6, 5, '.');

        FloodFillCommand cmd(&canvas, 5, 5, '@');
        cmd.execute();

        REQUIRE(canvas.getPixel(5, 5) == '@');
        REQUIRE(canvas.getPixel(5, 6) == '@');
        REQUIRE(canvas.getPixel(6, 5) == '@');

        cmd.undo();

        REQUIRE(canvas.getPixel(5, 5) == '.');
        REQUIRE(canvas.getPixel(5, 6) == '.');
        REQUIRE(canvas.getPixel(6, 5) == '.');
    }

    SECTION("Undo when no changes made") {
        canvas.setPixel(5, 5, '@');
        FloodFillCommand cmd(&canvas, 5, 5, '@');

        cmd.execute();
        cmd.undo();     

        REQUIRE(canvas.getPixel(5, 5) == '@');
    }
}
TEST_CASE("ClearCommand", "[commands]") {
    Canvas canvas(40, 20);
    canvas.setPixel(10, 5, '#');
    canvas.setPixel(20, 10, '*');

    SECTION("Execute") {
        ClearCommand cmd(&canvas);
        cmd.execute();

        REQUIRE(canvas.getPixel(10, 5) == '.');
        REQUIRE(canvas.getPixel(20, 10) == '.');
    }

    SECTION("Description") {
        ClearCommand cmd(&canvas);
        REQUIRE(cmd.getDescription() == "Clear entire canvas");
    }

    SECTION("Undo restores previous state") {
        canvas.drawLineImpl(0, 0, 5, 5, '#');
        REQUIRE(canvas.getPixel(2, 2) == '#');

        ClearCommand cmd(&canvas);
        cmd.execute();
        REQUIRE(canvas.getPixel(2, 2) == '.');

        cmd.undo();
        REQUIRE(canvas.getPixel(2, 2) == '#');
    }

    SECTION("Undo on empty canvas") {
        Canvas emptyCanvas(40, 20);

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                REQUIRE(emptyCanvas.getPixel(x, y) == '.');
            }
        }

        ClearCommand cmd(&emptyCanvas);
        cmd.execute(); 
        cmd.undo();    

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                REQUIRE(emptyCanvas.getPixel(x, y) == '.');
            }
        }
    }
}


