# Сценарий 1: Рисование линии
add_executable(scenario_draw_line draw_line.cpp)
target_link_libraries(scenario_draw_line PRIVATE ascii_paint_lib user32 gdi32)

# Сценарий 2: Рисование прямоугольника
add_executable(scenario_draw_rectangle draw_rectangle.cpp)
target_link_libraries(scenario_draw_rectangle PRIVATE ascii_paint_lib user32 gdi32)

# Сценарий 3: Заливка области
add_executable(scenario_flood_fill flood_fill.cpp)
target_link_libraries(scenario_flood_fill PRIVATE ascii_paint_lib user32 gdi32)

# Сценарий 4: Undo/Redo
add_executable(scenario_undo_redo undo_redo.cpp)
target_link_libraries(scenario_undo_redo PRIVATE ascii_paint_lib user32 gdi32)

# Сценарий 5: Сохранение и загрузка
add_executable(scenario_save_load save_load.cpp)
target_link_libraries(scenario_save_load PRIVATE ascii_paint_lib user32 gdi32)