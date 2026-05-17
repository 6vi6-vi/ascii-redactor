#pragma once

// Решаем конфликт byte
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Запрещаем использование std::byte (для C++17)
#define _HAS_STD_BYTE 0

#include <windows.h>
#include <conio.h>