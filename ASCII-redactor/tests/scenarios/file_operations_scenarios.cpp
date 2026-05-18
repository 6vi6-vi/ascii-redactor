#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include <fstream>
#include <cstdio>

TEST_CASE("File operations scenario: Save and load", "[scenario][file]") {
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
}

TEST_CASE("File operations scenario: Save complex drawing", "[scenario][file]") {
    Canvas canvas(40, 20);

    SECTION("Save rectangle with fill") {
        canvas.drawRectImpl(5, 5, 15, 10, true, '@');

        std::string filename = "test_complex.ascii";
        bool saved = canvas.saveToFile(filename);
        REQUIRE(saved);

        Canvas newCanvas(40, 20);
        newCanvas.loadFromFile(filename);

        // Проверяем внутренность прямоугольника
        for (int x = 6; x <= 14; x++) {
            for (int y = 6; y <= 9; y++) {
                REQUIRE(newCanvas.getPixel(x, y) == '@');
            }
        }

        std::remove(filename.c_str());
    }
}

// Остальные тесты file_operations_scenarios оставляем как были