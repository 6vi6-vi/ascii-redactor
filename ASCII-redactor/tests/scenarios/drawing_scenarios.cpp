#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawLineCommand.h"
#include "command/DrawRectCommand.h"
#include "command/FloodFillCommand.h"

TEST_CASE("Drawing scenario: Simple shape", "[scenario]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Draw square") {
        DrawRectCommand* rectCmd = new DrawRectCommand(&canvas, 10, 5, 20, 10, false, '#');
        history.executeCommand(rectCmd);

        for (int x = 10; x <= 20; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '#');
            REQUIRE(canvas.getPixel(x, 10) == '#');
        }
        for (int y = 5; y <= 10; y++) {
            REQUIRE(canvas.getPixel(10, y) == '#');
            REQUIRE(canvas.getPixel(20, y) == '#');
        }
    }

    SECTION("Line inside square") {
        DrawRectCommand* rectCmd = new DrawRectCommand(&canvas, 10, 5, 20, 10, false, '#');
        history.executeCommand(rectCmd);

        DrawLineCommand* lineCmd = new DrawLineCommand(&canvas, 12, 7, 18, 7, '*');
        history.executeCommand(lineCmd);

        for (int x = 12; x <= 18; x++) {
            REQUIRE(canvas.getPixel(x, 7) == '*');
        }
    }
}

TEST_CASE("Drawing scenario: Cross", "[scenario]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Draw cross") {
        DrawLineCommand* horizontal = new DrawLineCommand(&canvas, 5, 10, 35, 10, '-');
        history.executeCommand(horizontal);

        DrawLineCommand* vertical = new DrawLineCommand(&canvas, 20, 2, 20, 18, '|');
        history.executeCommand(vertical);

        // В точке пересечения последняя команда (vertical) перезаписывает горизонтальную
        // Проверяем, что горизонтальная линия есть везде, кроме пересечения
        REQUIRE(canvas.getPixel(10, 10) == '-');
        REQUIRE(canvas.getPixel(30, 10) == '-');
        REQUIRE(canvas.getPixel(20, 5) == '|');
        REQUIRE(canvas.getPixel(20, 15) == '|');
        // В точке пересечения будет '|' (последняя команда)
        REQUIRE(canvas.getPixel(20, 10) == '|');
    }
}

TEST_CASE("Drawing scenario: Flood fill", "[scenario]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Fill inside frame") {
        DrawRectCommand* frame = new DrawRectCommand(&canvas, 5, 5, 15, 10, false, '#');
        history.executeCommand(frame);

        FloodFillCommand* fill = new FloodFillCommand(&canvas, 10, 7, '@');
        history.executeCommand(fill);

        for (int x = 5; x <= 15; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '#');
            REQUIRE(canvas.getPixel(x, 10) == '#');
        }
        // Левая и правая границы
        for (int y = 5; y <= 10; y++) {
            REQUIRE(canvas.getPixel(5, y) == '#');
            REQUIRE(canvas.getPixel(15, y) == '#');
        }

        REQUIRE(canvas.getPixel(10, 7) == '@');
    }
}

TEST_CASE("Drawing scenario: House", "[scenario]") {
    Canvas canvas(40, 20);
    CommandHistory history;

    SECTION("Draw house") {
        DrawRectCommand* base = new DrawRectCommand(&canvas, 15, 10, 25, 15, true, '#');
        history.executeCommand(base);

        DrawLineCommand* roof = new DrawLineCommand(&canvas, 15, 10, 20, 5, '#');
        history.executeCommand(roof);

        DrawLineCommand* roof2 = new DrawLineCommand(&canvas, 20, 5, 25, 10, '#');
        history.executeCommand(roof2);

        REQUIRE(canvas.getPixel(20, 5) == '#');
        REQUIRE(canvas.getPixel(15, 10) == '#');
        REQUIRE(canvas.getPixel(25, 10) == '#');
    }
}