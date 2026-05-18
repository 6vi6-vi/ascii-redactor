#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"

TEST_CASE("Canvas creation", "[canvas]") {
    SECTION("Valid sizes") {
        Canvas canvas(40, 20);
        REQUIRE(canvas.getWidth() == 40);
        REQUIRE(canvas.getHeight() == 20);
        REQUIRE(canvas.getCurrentChar() == '@');
        REQUIRE(canvas.getCursorX() == 0);
        REQUIRE(canvas.getCursorY() == 0);
    }

    SECTION("Min size correction") {
        Canvas canvas(10, 5);
        REQUIRE(canvas.getWidth() == 40);
        REQUIRE(canvas.getHeight() == 20);
    }

    SECTION("Max size correction") {
        Canvas canvas(300, 150);
        REQUIRE(canvas.getWidth() == 200);
        REQUIRE(canvas.getHeight() == 100);
    }
}

TEST_CASE("Pixel operations", "[canvas]") {
    Canvas canvas(40, 20);

    SECTION("Set and get pixel") {
        canvas.setPixel(10, 5, '#');
        REQUIRE(canvas.getPixel(10, 5) == '#');

        canvas.setPixel(0, 0, '*');
        REQUIRE(canvas.getPixel(0, 0) == '*');
    }

    SECTION("Pixel out of bounds") {
        canvas.setPixel(-1, 0, '#');
        REQUIRE(canvas.getPixel(0, 0) == '.');

        canvas.setPixel(40, 0, '#');
        REQUIRE(canvas.getPixel(39, 0) == '.');

        canvas.setPixel(0, -1, '#');
        REQUIRE(canvas.getPixel(0, 0) == '.');

        canvas.setPixel(0, 20, '#');
        REQUIRE(canvas.getPixel(0, 19) == '.');
    }
}

TEST_CASE("Cursor movement", "[canvas]") {
    Canvas canvas(40, 20);

    SECTION("Normal movement") {
        canvas.moveCursor(10, 5);
        REQUIRE(canvas.getCursorX() == 10);
        REQUIRE(canvas.getCursorY() == 5);

        canvas.moveCursor(-5, -2);
        REQUIRE(canvas.getCursorX() == 5);
        REQUIRE(canvas.getCursorY() == 3);
    }

    SECTION("Out of bounds") {
        canvas.moveCursor(-10, -10);
        REQUIRE(canvas.getCursorX() == 0);
        REQUIRE(canvas.getCursorY() == 0);

        canvas.moveCursor(100, 100);
        REQUIRE(canvas.getCursorX() == 39);
        REQUIRE(canvas.getCursorY() == 19);
    }
}

TEST_CASE("Drawing lines", "[canvas]") {
    Canvas canvas(40, 20);

    SECTION("Diagonal line") {
        canvas.drawLineImpl(0, 0, 10, 10, '#');
        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '#');
        }
    }

    SECTION("Horizontal line") {
        canvas.drawLineImpl(0, 5, 20, 5, '-');
        for (int x = 0; x <= 20; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '-');
        }
    }

    SECTION("Vertical line") {
        canvas.drawLineImpl(10, 0, 10, 15, '|');
        for (int y = 0; y <= 15; y++) {
            REQUIRE(canvas.getPixel(10, y) == '|');
        }
    }
}

TEST_CASE("Drawing rectangles", "[canvas]") {
    Canvas canvas(40, 20);
    canvas.drawRectImpl(5, 5, 15, 10, false, '#');

    SECTION("Top and bottom borders") {
        for (int x = 5; x <= 15; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '#');
            REQUIRE(canvas.getPixel(x, 10) == '#');
        }
    }

    SECTION("Left and right borders") {
        for (int y = 5; y <= 10; y++) {
            REQUIRE(canvas.getPixel(5, y) == '#');
            REQUIRE(canvas.getPixel(15, y) == '#');
        }
    }
}

TEST_CASE("Clear canvas", "[canvas]") {
    Canvas canvas(40, 20);
    canvas.setPixel(10, 5, '#');
    canvas.setPixel(20, 10, '*');

    REQUIRE(canvas.getPixel(10, 5) == '#');
    REQUIRE(canvas.getPixel(20, 10) == '*');

    canvas.clearImpl();

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 40; x++) {
            REQUIRE(canvas.getPixel(x, y) == '.');
        }
    }
}

TEST_CASE("Current character", "[canvas]") {
    Canvas canvas(40, 20);

    SECTION("Set current char") {
        canvas.setCurrentChar('#');
        REQUIRE(canvas.getCurrentChar() == '#');

        canvas.setCurrentChar('*');
        REQUIRE(canvas.getCurrentChar() == '*');
    }
}