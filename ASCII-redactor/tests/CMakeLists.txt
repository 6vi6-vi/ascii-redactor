include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

set(TEST_SOURCES
    test_main.cpp
    test_canvas.cpp
    test_memento.cpp
    test_commands.cpp
    test_command_history.cpp
    test_tool_states.cpp
    test_editor_context.cpp
)

add_executable(ascii_paint_tests ${TEST_SOURCES})

target_link_libraries(ascii_paint_tests PRIVATE Catch2::Catch2WithMain)

target_include_directories(ascii_paint_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_sources(ascii_paint_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/canvas/Canvas.cpp
    ${CMAKE_SOURCE_DIR}/src/canvas/Memento.cpp
    ${CMAKE_SOURCE_DIR}/src/command/Command.cpp
    ${CMAKE_SOURCE_DIR}/src/command/CommandHistory.cpp
    ${CMAKE_SOURCE_DIR}/src/command/DrawLineCommand.cpp
    ${CMAKE_SOURCE_DIR}/src/command/DrawRectCommand.cpp
    ${CMAKE_SOURCE_DIR}/src/command/FloodFillCommand.cpp
    ${CMAKE_SOURCE_DIR}/src/command/ClearCommand.cpp
    ${CMAKE_SOURCE_DIR}/src/state/CursorState.cpp
    ${CMAKE_SOURCE_DIR}/src/state/LineToolState.cpp
    ${CMAKE_SOURCE_DIR}/src/state/RectToolState.cpp
    ${CMAKE_SOURCE_DIR}/src/observer/ConsoleRenderer.cpp
    ${CMAKE_SOURCE_DIR}/src/ui/EditorContext.cpp
)

if(WIN32)
    target_link_libraries(ascii_paint_tests PRIVATE user32 gdi32)
endif()

enable_testing()
add_test(NAME ascii_paint_tests COMMAND ascii_paint_tests)