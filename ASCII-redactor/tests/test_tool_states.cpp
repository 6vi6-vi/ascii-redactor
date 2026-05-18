#include <catch2/catch_all.hpp>
#include "state/CursorState.h"
#include "state/LineToolState.h"
#include "state/RectToolState.h"

TEST_CASE("CursorState", "[states]") {
    CursorState state;

    SECTION("Name") {
        REQUIRE(state.getName() == "Cursor");
    }

    SECTION("Status message") {
        REQUIRE(state.getStatusMessage() == "Selection mode");
    }

    SECTION("Drawing mode") {
        REQUIRE_FALSE(state.isDrawingMode());
    }
}

TEST_CASE("LineToolState", "[states]") {
    LineToolState state;

    SECTION("Name") {
        REQUIRE(state.getName() == "Line");
    }

    SECTION("Initial status message") {
        REQUIRE(state.getStatusMessage() == "Select first point (Enter)");
    }

    SECTION("Drawing mode") {
        REQUIRE(state.isDrawingMode());
    }

    // Примечание: onKeyPress и onCursorMove требуют mock-контекста
    // Их тестирование будет в test_editor_context.cpp
}

TEST_CASE("RectToolState", "[states]") {
    RectToolState state;

    SECTION("Name") {
        REQUIRE(state.getName() == "Rectangle");
    }

    SECTION("Initial status message") {
        REQUIRE(state.getStatusMessage() == "Select first corner (Enter)");
    }

    SECTION("Drawing mode") {
        REQUIRE(state.isDrawingMode());
    }
}