#include "canvas/Memento.h"

Memento::Memento(const vector<vector<char>>& grid, int x, int y, char ch)
    : snapshot(grid), cursorX(x), cursorY(y), currentChar(ch) {}

vector<vector<char>> Memento::getGrid() const { return snapshot; }
int Memento::getCursorX() const { return cursorX; }
int Memento::getCursorY() const { return cursorY; }
char Memento::getCurrentChar() const { return currentChar; }