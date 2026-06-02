#include <catch2/catch_all.hpp>
#include "canvas/Canvas.h"
#include "observer/CanvasObserver.h"
#include <fstream>
#include <cstdio>

// Мок-наблюдатель для тестирования
class MockObserver : public CanvasObserver {
public:
    int canvasChangedCount = 0;
    int stateChangedCount = 0;
    int toolChangedCount = 0;
    string lastStateMessage;
    string lastToolName;
    string lastToolStatus;

    void onCanvasChanged(const Canvas& canvas) override {
        canvasChangedCount++;
    }

    void onStateChanged(const string& message) override {
        stateChangedCount++;
        lastStateMessage = message;
    }

    void onToolChanged(const string& toolName, const string& statusMsg) override {
        toolChangedCount++;
        lastToolName = toolName;
        lastToolStatus = statusMsg;
    }
};

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

    SECTION("Overwrite existing pixel") {
        canvas.setPixel(5, 5, '#');
        REQUIRE(canvas.getPixel(5, 5) == '#');
        canvas.setPixel(5, 5, '*');
        REQUIRE(canvas.getPixel(5, 5) == '*');
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

TEST_CASE("setCursorPosition tests", "[canvas]") {
    Canvas canvas(40, 20);

    SECTION("Set to valid position") {
        canvas.setCursorPosition(15, 8);
        REQUIRE(canvas.getCursorX() == 15);
        REQUIRE(canvas.getCursorY() == 8);
    }

    SECTION("Set to X out of bounds") {
        canvas.setCursorPosition(50, 10);
        REQUIRE(canvas.getCursorX() == 0);   
        REQUIRE(canvas.getCursorY() == 10);  
    }

    SECTION("Set to Y out of bounds") {
        canvas.setCursorPosition(10, 30);
        REQUIRE(canvas.getCursorX() == 10);  
        REQUIRE(canvas.getCursorY() == 0);
    }

    SECTION("Set both out of bounds") {
        canvas.setCursorPosition(100, 100);
        REQUIRE(canvas.getCursorX() == 0);
        REQUIRE(canvas.getCursorY() == 0);
    }

    SECTION("Set negative coordinates") {
        canvas.setCursorPosition(-5, -5);
        REQUIRE(canvas.getCursorX() == 0);
        REQUIRE(canvas.getCursorY() == 0);
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

    SECTION("Top and bottom borders") {
        canvas.drawRectImpl(5, 5, 15, 10, false, '#');
        for (int x = 5; x <= 15; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '#');
            REQUIRE(canvas.getPixel(x, 10) == '#');
        }
    }

    SECTION("Left and right borders") {
        canvas.drawRectImpl(5, 5, 15, 10, false, '#');
        for (int y = 5; y <= 10; y++) {
            REQUIRE(canvas.getPixel(5, y) == '#');
            REQUIRE(canvas.getPixel(15, y) == '#');
        }
    }

    SECTION("Filled rectangle") {
        canvas.drawRectImpl(5, 5, 10, 10, true, '@');
        for (int x = 5; x <= 10; x++) {
            for (int y = 5; y <= 10; y++) {
                REQUIRE(canvas.getPixel(x, y) == '@');
            }
        }
    }

    SECTION("1x1 rectangle (single pixel)") {
        canvas.drawRectImpl(10, 10, 10, 10, false, '#');
        REQUIRE(canvas.getPixel(10, 10) == '#');
    }

    SECTION("1xN rectangle (vertical line)") {
        canvas.drawRectImpl(10, 5, 10, 15, false, '|');
        for (int y = 5; y <= 15; y++) {
            REQUIRE(canvas.getPixel(10, y) == '|');
        }
    }
}

TEST_CASE("floodFillImpl tests", "[canvas]") {
    Canvas canvas(40, 20);

    SECTION("Fill single pixel") {
        canvas.setPixel(5, 5, '.');
        canvas.floodFillImpl(5, 5, '@');
        REQUIRE(canvas.getPixel(5, 5) == '@');
    }

    SECTION("Fill connected area") {
        canvas.setPixel(5, 5, '.');
        canvas.setPixel(5, 6, '.');
        canvas.setPixel(6, 5, '.');
        canvas.floodFillImpl(5, 5, '@');
        REQUIRE(canvas.getPixel(5, 5) == '@');
        REQUIRE(canvas.getPixel(5, 6) == '@');
        REQUIRE(canvas.getPixel(6, 5) == '@');
    }

    SECTION("Do not fill different character") {
        canvas.setPixel(5, 5, '.');
        canvas.setPixel(5, 6, '#');
        canvas.floodFillImpl(5, 5, '@');
        REQUIRE(canvas.getPixel(5, 5) == '@');
        REQUIRE(canvas.getPixel(5, 6) == '#');
    }

    SECTION("Fill same character") {
        canvas.setPixel(5, 5, '@');
        canvas.floodFillImpl(5, 5, '@');
        REQUIRE(canvas.getPixel(5, 5) == '@');
    }
}

TEST_CASE("Clear canvas", "[canvas]") {
    Canvas canvas(40, 20);

    SECTION("Clear after drawing line") {
        canvas.drawLineImpl(0, 0, 10, 10, '#');
        canvas.clearImpl();
        for (int i = 0; i <= 10; i++) {
            REQUIRE(canvas.getPixel(i, i) == '.');
        }
    }

    SECTION("Clear after drawing rectangle") {
        canvas.drawRectImpl(5, 5, 15, 10, false, '#');
        canvas.clearImpl();
        for (int x = 5; x <= 15; x++) {
            REQUIRE(canvas.getPixel(x, 5) == '.');
        }
    }

    SECTION("Clear empty canvas") {
        canvas.clearImpl();
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 40; x++) {
                REQUIRE(canvas.getPixel(x, y) == '.');
            }
        }
    }

    SECTION("Clear twice") {
        canvas.setPixel(10, 10, '#');
        canvas.clearImpl();
        canvas.clearImpl();
        REQUIRE(canvas.getPixel(10, 10) == '.');
    }
}

TEST_CASE("Current character", "[canvas]") {
    Canvas canvas(40, 20);

    SECTION("Set current char") {
        canvas.setCurrentChar('#');
        REQUIRE(canvas.getCurrentChar() == '#');

        canvas.setCurrentChar('*');
        REQUIRE(canvas.getCurrentChar() == '*');

        canvas.setCurrentChar('!');
        REQUIRE(canvas.getCurrentChar() == '!');

        canvas.setCurrentChar('/');
        REQUIRE(canvas.getCurrentChar() == '/');

        canvas.setCurrentChar('5');
        REQUIRE(canvas.getCurrentChar() == '5');
    }
}

TEST_CASE("Observer tests", "[observer]") {
    Canvas canvas(40, 20);
    MockObserver observer;

    SECTION("Single observer") {
        canvas.attachObserver(&observer);

        canvas.notifyCanvasChanged();
        REQUIRE(observer.canvasChangedCount == 1);

        canvas.notifyStateChanged("Test message");
        REQUIRE(observer.stateChangedCount == 1);
        REQUIRE(observer.lastStateMessage == "Test message");

        canvas.notifyToolChanged("Line", "Draw line");
        REQUIRE(observer.toolChangedCount == 1);
        REQUIRE(observer.lastToolName == "Line");
    }

    SECTION("Multiple observers") {
        MockObserver observer2;
        canvas.attachObserver(&observer);
        canvas.attachObserver(&observer2);

        canvas.notifyCanvasChanged();
        REQUIRE(observer.canvasChangedCount == 1);
        REQUIRE(observer2.canvasChangedCount == 1);
    }

    SECTION("detachObserver") {
        canvas.attachObserver(&observer);
        canvas.notifyCanvasChanged();
        REQUIRE(observer.canvasChangedCount == 1);

        canvas.detachObserver(&observer);
        canvas.notifyCanvasChanged();
        REQUIRE(observer.canvasChangedCount == 1);
    }

    SECTION("notifyStateChanged with different messages") {
        canvas.attachObserver(&observer);

        canvas.notifyStateChanged("Message 1");
        REQUIRE(observer.lastStateMessage == "Message 1");

        canvas.notifyStateChanged("Message 2");
        REQUIRE(observer.lastStateMessage == "Message 2");

        canvas.notifyStateChanged("");
        REQUIRE(observer.lastStateMessage == "");
    }

    SECTION("notifyToolChanged with different tools") {
        canvas.attachObserver(&observer);

        canvas.notifyToolChanged("Line", "Draw line");
        REQUIRE(observer.lastToolName == "Line");
        REQUIRE(observer.lastToolStatus == "Draw line");

        canvas.notifyToolChanged("Rectangle", "Draw rectangle");
        REQUIRE(observer.lastToolName == "Rectangle");
    }
}

TEST_CASE("restoreFromMemento tests", "[memento]") {
    Canvas canvas(40, 20);

    SECTION("Restore full state") {
        canvas.setPixel(10, 5, '#');
        canvas.moveCursor(5, 5);
        canvas.setCurrentChar('*');
        Memento memento(
            canvas.getGridSnapshot(),
            canvas.getCursorX(),
            canvas.getCursorY(),
            canvas.getCurrentChar()
        );

        canvas.setPixel(10, 5, '.');
        canvas.moveCursor(0, 0);
        canvas.setCurrentChar('@');

        canvas.restoreFromMemento(memento);

        REQUIRE(canvas.getPixel(10, 5) == '#');
        REQUIRE(canvas.getCursorX() == 5);
        REQUIRE(canvas.getCursorY() == 5);
        REQUIRE(canvas.getCurrentChar() == '*');
    }

    SECTION("Restore different cursor positions") {
        vector<pair<int, int>> positions = { {0,0}, {10,5}, {30,15}, {39,19} };

        for (auto [x, y] : positions) {
            Memento memento(canvas.getGridSnapshot(), x, y, '@');
            canvas.restoreFromMemento(memento);
            REQUIRE(canvas.getCursorX() == x);
            REQUIRE(canvas.getCursorY() == y);
        }
    }

    SECTION("Restore different current chars") {
        vector<char> chars = { '@', '#', '%', '*', '+', '-', '=', '|', '/' };

        for (char ch : chars) {
            Memento memento(canvas.getGridSnapshot(), 0, 0, ch);
            canvas.restoreFromMemento(memento);
            REQUIRE(canvas.getCurrentChar() == ch);
        }
    }
}