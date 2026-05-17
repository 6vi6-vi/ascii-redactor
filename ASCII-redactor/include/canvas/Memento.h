#pragma once
#include <vector>

using namespace std;

class Memento {
private:
    vector<vector<char>> snapshot;
    int cursorX;
    int cursorY;
    char currentChar;

public:
    Memento(const vector<vector<char>>& grid, int x, int y, char ch);
    
    vector<vector<char>> getGrid() const;
    int getCursorX() const;
    int getCursorY() const;
    char getCurrentChar() const;
};