#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include "command/DrawLineCommand.h"
#include "command/DrawRectCommand.h"
#include "command/FloodFillCommand.h"
#include "command/ClearCommand.h"
#include <string>

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
}