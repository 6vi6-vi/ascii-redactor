#include <catch2/catch_all.hpp>
#include "state/CursorState.h"
#include "state/LineToolState.h"
#include "state/RectToolState.h"
#include "ui/EditorContext.h"
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"

TEST_CASE("CursorState", "[states]") {
    Canvas canvas(40, 20);
    CommandHistory history;
    EditorContext context(&canvas, &history);
    CursorState* state = new CursorState();
    context.setState(state);

    SECTION("Name") {
        REQUIRE(state->getName() == "Cursor");
    }

    SECTION("Status message") {
        REQUIRE(state->getStatusMessage() == "Selection mode");
    }

    SECTION("Drawing mode") {
        REQUIRE_FALSE(state->isDrawingMode());
    }

    SECTION("onCursorMove") {
        int startX = canvas.getCursorX();
        int startY = canvas.getCursorY();

        state->onCursorMove(5, 3);

        REQUIRE(canvas.getCursorX() == startX + 5);
        REQUIRE(canvas.getCursorY() == startY + 3);
    }
}

TEST_CASE("LineToolState", "[states]") {
    Canvas canvas(40, 20);
    CommandHistory history;
    EditorContext context(&canvas, &history);
    LineToolState* state = new LineToolState();
    context.setState(state);

    SECTION("Name") {
        REQUIRE(state->getName() == "Line");
    }

    SECTION("Drawing mode") {
        REQUIRE(state->isDrawingMode());
    }

    SECTION("Initial status message") {
        REQUIRE(state->getStatusMessage() == "Select first point (Enter)");
    }

    SECTION("onKeyPress - Enter") {
        canvas.setCursorPosition(10, 10);

        state->onKeyPress(13); 

        REQUIRE(state->getStatusMessage() == "Select second point (Enter)");
    }

    SECTION("onKeyPress - Escape") {
        state->onKeyPress(27);
        REQUIRE(context.getCurrentToolName() == "Cursor");
    }

    SECTION("onCursorMove") {
        int startX = canvas.getCursorX();
        int startY = canvas.getCursorY();

        state->onCursorMove(5, 3);

        REQUIRE(canvas.getCursorX() == startX + 5);
        REQUIRE(canvas.getCursorY() == startY + 3);
    }
}

TEST_CASE("RectToolState", "[states]") {
    Canvas canvas(40, 20);
    CommandHistory history;
    EditorContext context(&canvas, &history);
    RectToolState* state = new RectToolState();
    context.setState(state);

    SECTION("Name") {
        REQUIRE(state->getName() == "Rectangle");
    }

    SECTION("Drawing mode") {
        REQUIRE(state->isDrawingMode());
    }

    SECTION("Initial status message") {
        REQUIRE(state->getStatusMessage() == "Select first corner (Enter)");
    }

    SECTION("onKeyPress - Enter") {
        canvas.setCursorPosition(5, 5);
        state->onKeyPress(13);
        REQUIRE(state->getStatusMessage() == "Select second corner (Enter)");
    }

    SECTION("onKeyPress - Escape") {
        state->onKeyPress(27);
        REQUIRE(context.getCurrentToolName() == "Cursor");
    }

    SECTION("onCursorMove") {
        int startX = canvas.getCursorX();
        int startY = canvas.getCursorY();

        state->onCursorMove(5, 3);

        REQUIRE(canvas.getCursorX() == startX + 5);
        REQUIRE(canvas.getCursorY() == startY + 3);
    }
}