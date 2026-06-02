#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <clocale>

#ifdef _WIN32
// Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <conio.h>

inline void platform_clear_screen() {
    system("cls");
}
#else
// Linux / macOS
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

typedef void* HANDLE;

#define STD_OUTPUT_HANDLE 0
#define CP_UTF8 65001

// ANSI цветовые коды
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_BOLD    "\033[1m"

#define BG_BLUE       "\033[44m"
#define BG_WHITE      "\033[47m"

// Состояние терминала
static struct termios original_tio;
static bool terminal_configured = false;

inline void configure_terminal() {
    if (!terminal_configured) {
        tcgetattr(STDIN_FILENO, &original_tio);
        terminal_configured = true;
    }
}

inline void restore_terminal() {
    if (terminal_configured) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
    }
}

inline int _kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

inline int _getch() {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

inline void Sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline void SetConsoleOutputCP(int code) {}
inline void SetConsoleCP(int code) {}
inline HANDLE GetStdHandle(int handle) { return nullptr; }

inline void SetConsoleTextAttribute(HANDLE hConsole, int color) {
    switch (color) {
    case 7:   std::cout << COLOR_RESET; break;
    case 14:  std::cout << COLOR_YELLOW; break;
    case 10:  std::cout << COLOR_GREEN; break;
    case 112: std::cout << BG_BLUE << COLOR_WHITE; break;
    default:  std::cout << COLOR_RESET; break;
    }
}

inline void platform_clear_screen() {
    // Очищаем экран и перемещаем курсор в начало
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}

// Нормализация Enter: в Linux Enter = 10, в Windows = 13
inline int normalize_key(int key) {
    if (key == 10) return 13;  // LF - CR (Enter)
    return key;
}

#endif