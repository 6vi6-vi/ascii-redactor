cmake_minimum_required(VERSION 3.10)
project(ascii_paint VERSION 1.0.0 LANGUAGES CXX)

# Устанавливаем выходную директорию для всех исполняемых файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Платформозависимые библиотеки
if(WIN32)
    add_definitions(-D_WIN32)
    set(PLATFORM_LIBS user32 gdi32)
else()
    add_definitions(-D_LINUX)
    set(PLATFORM_LIBS pthread)
endif()

set(LIB_SOURCES
    src/canvas/Canvas.cpp
    src/canvas/Memento.cpp
    src/command/Command.cpp
    src/command/CommandHistory.cpp
    src/command/DrawLineCommand.cpp
    src/command/DrawRectCommand.cpp
    src/command/FloodFillCommand.cpp
    src/command/ClearCommand.cpp
    src/state/CursorState.cpp
    src/state/LineToolState.cpp
    src/state/RectToolState.cpp
    src/observer/ConsoleRenderer.cpp
    src/ui/EditorContext.cpp
    src/ui/InputHandler.cpp
)

add_library(ascii_paint_lib STATIC ${LIB_SOURCES})

add_executable(ascii_paint src/main.cpp)
target_link_libraries(ascii_paint ascii_paint_lib ${PLATFORM_LIBS})

set_target_properties(ascii_paint PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(MSVC)
    target_compile_options(ascii_paint PRIVATE /W4 /utf-8 /wd4100)
else()
    target_compile_options(ascii_paint PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

enable_testing()
add_subdirectory(tests)
add_subdirectory(scenarios)