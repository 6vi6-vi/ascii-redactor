#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include "canvas/Memento.h"

TEST_CASE("Memento operations", "[memento]") {
    Canvas canvas(40, 20);
    canvas.setPixel(10, 5, '#');
    canvas.setPixel(15, 10, '*');
    canvas.moveCursor(5, 5);
    canvas.setCurrentChar('@');

    SECTION("Create snapshot") {
        Memento memento(
            canvas.getGridSnapshot(),
            canvas.getCursorX(),
            canvas.getCursorY(),
            canvas.getCurrentChar()
        );

        REQUIRE(memento.getCursorX() == 5);
        REQUIRE(memento.getCursorY() == 5);
        REQUIRE(memento.getCurrentChar() == '@');
    }

    SECTION("Restore from snapshot") {
        Memento memento(
            canvas.getGridSnapshot(),
            canvas.getCursorX(),
            canvas.getCursorY(),
            canvas.getCurrentChar()
        );

        canvas.setPixel(10, 5, '.');
        canvas.moveCursor(0, 0);
        canvas.setCurrentChar('*');

        canvas.restoreFromMemento(memento);

        REQUIRE(canvas.getPixel(10, 5) == '#');
        REQUIRE(canvas.getCursorX() == 5);
        REQUIRE(canvas.getCursorY() == 5);
        REQUIRE(canvas.getCurrentChar() == '@');
    }
}