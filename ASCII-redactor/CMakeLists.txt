cmake_minimum_required(VERSION 3.10)
project(AsciiDrawStudio VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Подключаем заголовочные файлы
include_directories(${CMAKE_SOURCE_DIR}/include)

# Исходные файлы
set(SOURCES
    src/main.cpp
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

add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} user32 gdi32)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /utf-8 /wd4100)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()