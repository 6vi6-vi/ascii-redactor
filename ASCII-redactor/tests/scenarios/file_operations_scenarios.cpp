#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include <fstream>
#include <cstdio>

TEST_CASE("Save and load", "[scenario][file]") {
    Canvas canvas(40, 20);

    SECTION("Save and load simple shape") {
        canvas.drawLineImpl(0, 0, 10, 10, '#');
        canvas.setPixel(15, 5, '*');

        std::string filename = "test_save.ascii";
        bool saved = canvas.saveToFile(filename);
        REQUIRE(saved);

        Canvas newCanvas(40, 20);
        bool loaded = newCanvas.loadFromFile(filename);
        REQUIRE(loaded);

        for (int i = 0; i <= 10; i++) {
            REQUIRE(newCanvas.getPixel(i, i) == '#');
        }
        REQUIRE(newCanvas.getPixel(15, 5) == '*');

        std::remove(filename.c_str());
    }

    SECTION("Save and load empty canvas") {
        std::string filename = "test_empty.ascii";
        bool saved = canvas.saveToFile(filename);
        REQUIRE(saved);

        Canvas newCanvas(40, 20);
        newCanvas.loadFromFile(filename);

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                REQUIRE(newCanvas.getPixel(x, y) == '.');
            }
        }

        std::remove(filename.c_str());
    }
}

TEST_CASE("Load non-existent file", "[scenario][file]") {
    Canvas canvas(40, 20);
    bool loaded = canvas.loadFromFile("nonexistent_file.ascii");
    REQUIRE_FALSE(loaded);
}

TEST_CASE("Save with different extensions", "[scenario][file]") {
    Canvas canvas(40, 20);
    canvas.setPixel(10, 10, '#');

    SECTION("Save as .ascii") {
        std::string filename = "test_ascii.ascii";
        bool saved = canvas.saveToFile(filename);
        REQUIRE(saved);

        Canvas newCanvas(40, 20);
        newCanvas.loadFromFile(filename);
        REQUIRE(newCanvas.getPixel(10, 10) == '#');

        std::remove(filename.c_str());
    }

    SECTION("Save as .txt") {
        std::string filename = "test.txt";
        bool saved = canvas.saveToFile(filename);
        REQUIRE(saved);

        Canvas newCanvas(40, 20);
        newCanvas.loadFromFile(filename);
        REQUIRE(newCanvas.getPixel(10, 10) == '#');

        std::remove(filename.c_str());
    }
}

TEST_CASE("Save complex drawing", "[scenario][file]") {
    Canvas canvas(40, 20);

    SECTION("Save rectangle and line") {
        canvas.drawLineImpl(0, 0, 5, 5, '#');
        canvas.drawRectImpl(20, 10, 30, 15, true, '@');

        std::string filename = "test_complex.ascii";
        bool saved = canvas.saveToFile(filename);
        REQUIRE(saved);

        Canvas newCanvas(40, 20);
        newCanvas.loadFromFile(filename);

        for (int i = 0; i <= 5; i++) {
            REQUIRE(newCanvas.getPixel(i, i) == '#');
        }

        for (int x = 20; x <= 30; x++) {
            for (int y = 10; y <= 15; y++) {
                REQUIRE(newCanvas.getPixel(x, y) == '@');
            }
        }

        std::remove(filename.c_str());
    }
}