#pragma once
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include "canvas/Memento.h"
#include "observer/CanvasObserver.h"

using namespace std;

class Canvas {
private:
    int width;
    int height;
    vector<vector<char>> grid;
    int cursorX;
    int cursorY;
    char currentChar;
    vector<CanvasObserver*> observers;

public:
    Canvas(int w, int h);
    
    void attachObserver(CanvasObserver* observer);
    void detachObserver(CanvasObserver* observer);
    void notifyCanvasChanged();
    void notifyStateChanged(const string& message);
    void notifyToolChanged(const string& toolName, const string& statusMsg);
    
    int getWidth() const;
    int getHeight() const;
    int getCursorX() const;
    int getCursorY() const;
    char getCurrentChar() const;
    char getPixel(int x, int y) const;
    vector<vector<char>> getGridSnapshot() const;
    
    void restoreFromMemento(const Memento& memento);
    void setCurrentChar(char ch);
    void setPixel(int x, int y, char ch);
    void moveCursor(int dx, int dy);
    void setCursorPosition(int x, int y);
    
    void drawLineImpl(int x1, int y1, int x2, int y2, char ch);
    void drawRectImpl(int x1, int y1, int x2, int y2, bool fill, char ch);
    void floodFillImpl(int x, int y, char newChar);
    void clearImpl();
    
    bool saveToFile(const string& filename);
    bool loadFromFile(const string& filename);
};