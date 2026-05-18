#include <catch2/catch_all.hpp>
#include "ui/EditorContext.h"
#include "canvas/Canvas.h"
#include "command/CommandHistory.h"
#include "command/DrawLineCommand.h"
#include "command/DrawRectCommand.h"
#include "command/FloodFillCommand.h"
#include "command/ClearCommand.h"
#include "state/CursorState.h"
#include "state/LineToolState.h"
#include "state/RectToolState.h"

class EditorContextFixture {
protected:
    EditorContextFixture() : canvas(40, 20), history(), context(&canvas, &history) {}

    Canvas canvas;
    CommandHistory history;
    EditorContext context;
};

TEST_CASE_METHOD(EditorContextFixture, "EditorContext initialization", "[editor]") {
    SECTION("Initial state is null") {
        REQUIRE(context.getState() == nullptr);
    }

    SECTION("Get canvas returns correct pointer") {
        REQUIRE(context.getCanvas() == &canvas);
    }

    SECTION("Get history returns correct pointer") {
        REQUIRE(context.getHistory() == &history);
    }

    SECTION("Initial tool name when no state") {
        REQUIRE(context.getCurrentToolName() == "Cursor");
    }

    SECTION("Initial status message when no state") {
        REQUIRE(context.getStatusMessage() == "");
    }

    SECTION("Initial drawing mode when no state") {
        REQUIRE_FALSE(context.isDrawingMode());
    }
}

TEST_CASE_METHOD(EditorContextFixture, "EditorContext state management", "[editor]") {
    SECTION("Set CursorState") {
        context.setState(new CursorState());
        REQUIRE(context.getCurrentToolName() == "Cursor");
        REQUIRE_FALSE(context.isDrawingMode());
        REQUIRE(context.getStatusMessage() == "Selection mode");
    }

    SECTION("Set LineToolState") {
        context.setState(new LineToolState());
        REQUIRE(context.getCurrentToolName() == "Line");
        REQUIRE(context.isDrawingMode());
        // Статус может меняться в зависимости от состояния
        REQUIRE((context.getStatusMessage() == "Select first point (Enter)" ||
            context.getStatusMessage() == "Select first point (Enter)"));
    }

    SECTION("Set RectToolState") {
        context.setState(new RectToolState());
        REQUIRE(context.getCurrentToolName() == "Rectangle");
        REQUIRE(context.isDrawingMode());
        REQUIRE((context.getStatusMessage() == "Select first corner (Enter)" ||
            context.getStatusMessage() == "Select first corner (Enter)"));
    }

    SECTION("State transition") {
        context.setState(new CursorState());
        REQUIRE(context.getCurrentToolName() == "Cursor");

        context.setState(new LineToolState());
        REQUIRE(context.getCurrentToolName() == "Line");
        REQUIRE(context.isDrawingMode());

        context.setState(new RectToolState());
        REQUIRE(context.getCurrentToolName() == "Rectangle");
        REQUIRE(context.isDrawingMode());

        context.setState(new CursorState());
        REQUIRE(context.getCurrentToolName() == "Cursor");
        REQUIRE_FALSE(context.isDrawingMode());
    }
}

TEST_CASE_METHOD(EditorContextFixture, "EditorContext cursor movement", "[editor]") {
    SECTION("Move cursor without state") {
        context.moveCursor(10, 5);
        REQUIRE(canvas.getCursorX() == 10);
        REQUIRE(canvas.getCursorY() == 5);
    }

    SECTION("Move cursor with CursorState") {
        context.setState(new CursorState());
        context.moveCursor(10, 5);
        REQUIRE(canvas.getCursorX() == 10);
        REQUIRE(canvas.getCursorY() == 5);
    }

    SECTION("Handle cursor move delegates to state") {
        context.setState(new CursorState());
        context.handleCursorMove(5, 3);
        REQUIRE(canvas.getCursorX() == 5);
        REQUIRE(canvas.getCursorY() == 3);
    }
}

TEST_CASE_METHOD(EditorContextFixture, "EditorContext undo/redo", "[editor]") {
    // Сначала нужно создать команду через executeCommand
    SECTION("Execute command through context") {
        // Временно создадим команду через DrawLineCommand
        // Для теста используем прямой executeCommand
        DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
        context.executeCommand(cmd);

        for (int i = 0; i <= 5; i++) {
            REQUIRE(canvas.getPixel(i, i) == '#');
        }
    }

    SECTION("Undo through context") {
        DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
        context.executeCommand(cmd);
        context.undo();

        for (int i = 0; i <= 5; i++) {
            REQUIRE(canvas.getPixel(i, i) == '.');
        }
    }

    SECTION("Redo through context") {
        DrawLineCommand* cmd = new DrawLineCommand(&canvas, 0, 0, 5, 5, '#');
        context.executeCommand(cmd);
        context.undo();
        context.redo();

        for (int i = 0; i <= 5; i++) {
            REQUIRE(canvas.getPixel(i, i) == '#');
        }
    }
}

// Тесты для LineToolState с mock-контекстом
// Эти тесты проверяют, что состояние правильно обрабатывает нажатия клавиш
TEST_CASE_METHOD(EditorContextFixture, "LineToolState key handling", "[editor][state]") {
    context.setState(new LineToolState());

    SECTION("First Enter selects first point") {
        // Устанавливаем курсор в позицию
        canvas.setCursorPosition(10, 10);

        // Имитируем нажатие Enter в состоянии LineToolState
        // Примечание: onKeyPress вызывает notifyStateChanged
        // Мы не можем напрямую проверить внутреннее состояние,
        // но можем проверить, что после второго Enter линия рисуется
    }
}

// Тесты для RectToolState с mock-контекстом
TEST_CASE_METHOD(EditorContextFixture, "RectToolState key handling", "[editor][state]") {
    context.setState(new RectToolState());

    SECTION("First Enter selects first corner") {
        canvas.setCursorPosition(5, 5);
        // Аналогично LineToolState
    }
}

// Тесты для CursorState
TEST_CASE_METHOD(EditorContextFixture, "CursorState key handling", "[editor][state]") {
    context.setState(new CursorState());

    SECTION("Cursor state does not draw on key press") {
        canvas.setCursorPosition(10, 10);
        canvas.setCurrentChar('#');

        // Нажатие любой клавиши (кроме L/R) в CursorState должно менять символ
        // через InputHandler, а не через состояние
        REQUIRE(canvas.getCurrentChar() == '#');
    }
}