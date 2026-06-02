# Устанавливаем выходную директорию для сценариев
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Сценарии
add_executable(scenario_draw_line draw_line.cpp)
target_link_libraries(scenario_draw_line PRIVATE ascii_paint_lib)

add_executable(scenario_draw_rectangle draw_rectangle.cpp)
target_link_libraries(scenario_draw_rectangle PRIVATE ascii_paint_lib)

add_executable(scenario_flood_fill flood_fill.cpp)
target_link_libraries(scenario_flood_fill PRIVATE ascii_paint_lib)

add_executable(scenario_undo_redo undo_redo.cpp)
target_link_libraries(scenario_undo_redo PRIVATE ascii_paint_lib)

add_executable(scenario_save_load save_load.cpp)
target_link_libraries(scenario_save_load PRIVATE ascii_paint_lib)

# Для Linux добавляем pthread
if(NOT WIN32)
    target_link_libraries(scenario_draw_line PRIVATE pthread)
    target_link_libraries(scenario_draw_rectangle PRIVATE pthread)
    target_link_libraries(scenario_flood_fill PRIVATE pthread)
    target_link_libraries(scenario_undo_redo PRIVATE pthread)
    target_link_libraries(scenario_save_load PRIVATE pthread)
endif()