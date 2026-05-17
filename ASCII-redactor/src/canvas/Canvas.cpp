#include "canvas/Canvas.h"
#include <fstream>

Canvas::Canvas(int w, int h) : width(w), height(h), cursorX(0), cursorY(0), currentChar('@') {
    if (width < 40) width = 40;
    if (width > 200) width = 200;
    if (height < 20) height = 20;
    if (height > 100) height = 100;
    grid = vector<vector<char>>(height, vector<char>(width, '.'));
}

void Canvas::attachObserver(CanvasObserver* observer) {
    observers.push_back(observer);
}

void Canvas::detachObserver(CanvasObserver* observer) {
    auto it = find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

void Canvas::notifyCanvasChanged() {
    for (auto observer : observers) {
        observer->onCanvasChanged(*this);
    }
}

void Canvas::notifyStateChanged(const string& message) {
    for (auto observer : observers) {
        observer->onStateChanged(message);
    }
}

void Canvas::notifyToolChanged(const string& toolName, const string& statusMsg) {
    for (auto observer : observers) {
        observer->onToolChanged(toolName, statusMsg);
    }
}

int Canvas::getWidth() const { return width; }
int Canvas::getHeight() const { return height; }
int Canvas::getCursorX() const { return cursorX; }
int Canvas::getCursorY() const { return cursorY; }
char Canvas::getCurrentChar() const { return currentChar; }
char Canvas::getPixel(int x, int y) const { return grid[y][x]; }
vector<vector<char>> Canvas::getGridSnapshot() const { return grid; }

void Canvas::restoreFromMemento(const Memento& memento) {
    grid = memento.getGrid();
    cursorX = memento.getCursorX();
    cursorY = memento.getCursorY();
    currentChar = memento.getCurrentChar();
    notifyCanvasChanged();
}

void Canvas::setCurrentChar(char ch) {
    currentChar = ch;
    notifyStateChanged("Current character changed to '" + string(1, ch) + "'");
    notifyCanvasChanged();
}

void Canvas::setPixel(int x, int y, char ch) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = ch;
    }
}

void Canvas::moveCursor(int dx, int dy) {
    cursorX += dx;
    cursorY += dy;
    if (cursorX < 0) cursorX = 0;
    if (cursorX >= width) cursorX = width - 1;
    if (cursorY < 0) cursorY = 0;
    if (cursorY >= height) cursorY = height - 1;
    notifyCanvasChanged();
}

void Canvas::setCursorPosition(int x, int y) {
    if (x >= 0 && x < width) cursorX = x;
    if (y >= 0 && y < height) cursorY = y;
    notifyCanvasChanged();
}

void Canvas::drawLineImpl(int x1, int y1, int x2, int y2, char ch) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int x = x1, y = y1;
    while (true) {
        setPixel(x, y, ch);
        if (x == x2 && y == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }
    notifyCanvasChanged();
}

void Canvas::drawRectImpl(int x1, int y1, int x2, int y2, bool fill, char ch) {
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);

    if (fill) {
        for (int y = y1; y <= y2; y++) {
            for (int x = x1; x <= x2; x++) {
                setPixel(x, y, ch);
            }
        }
    }
    else {
        for (int x = x1; x <= x2; x++) {
            setPixel(x, y1, ch);
            setPixel(x, y2, ch);
        }
        for (int y = y1; y <= y2; y++) {
            setPixel(x1, y, ch);
            setPixel(x2, y, ch);
        }
    }
    notifyCanvasChanged();
}

void Canvas::floodFillImpl(int x, int y, char newChar) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    char targetChar = grid[y][x];
    if (targetChar == newChar) return;

    stack<pair<int, int>> st;
    st.push({ x, y });

    while (!st.empty()) {
        auto [cx, cy] = st.top();
        st.pop();

        if (cx < 0 || cx >= width || cy < 0 || cy >= height) continue;
        if (grid[cy][cx] != targetChar) continue;

        grid[cy][cx] = newChar;

        st.push({ cx + 1, cy });
        st.push({ cx - 1, cy });
        st.push({ cx, cy + 1 });
        st.push({ cx, cy - 1 });
    }
    notifyCanvasChanged();
    notifyStateChanged("Area filled with '" + string(1, newChar) + "'");
}

void Canvas::clearImpl() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = '.';
        }
    }
    notifyCanvasChanged();
    notifyStateChanged("Canvas cleared");
}

bool Canvas::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file << grid[y][x];
        }
        file << endl;
    }
    notifyStateChanged("Saved to file: " + filename);
    return true;
}

bool Canvas::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    vector<vector<char>> newGrid(height, vector<char>(width, '.'));
    string line;
    int y = 0;

    while (getline(file, line) && y < height) {
        for (int x = 0; x < width && x < (int)line.length(); x++) {
            newGrid[y][x] = line[x];
        }
        y++;
    }

    grid = newGrid;
    notifyCanvasChanged();
    notifyStateChanged("Loaded from file: " + filename);
    return true;
}