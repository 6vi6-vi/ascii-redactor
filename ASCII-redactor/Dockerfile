# ==================== Многостадийная сборка ====================

# Стадия 1: Сборка проекта
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем Catch2
WORKDIR /opt
RUN git clone --depth 1 --branch v3.4.0 https://github.com/catchorg/Catch2.git && \
    cd Catch2 && \
    cmake -B build -S . -DBUILD_TESTING=OFF && \
    cmake --build build --target install && \
    cd /opt && rm -rf Catch2

# Копируем исходный код
WORKDIR /app
COPY . .

# Создаём директорию для сборки
RUN mkdir -p build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# Стадия 2: Финальный образ
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

# Копируем собранные файлы из bin/
COPY --from=builder /app/build/bin/ascii_paint /usr/local/bin/
COPY --from=builder /app/build/bin/ascii_paint_tests /usr/local/bin/
COPY --from=builder /app/build/bin/scenario_draw_line /usr/local/bin/
COPY --from=builder /app/build/bin/scenario_draw_rectangle /usr/local/bin/
COPY --from=builder /app/build/bin/scenario_flood_fill /usr/local/bin/
COPY --from=builder /app/build/bin/scenario_undo_redo /usr/local/bin/
COPY --from=builder /app/build/bin/scenario_save_load /usr/local/bin/

WORKDIR /app

# Команда по умолчанию
CMD ["/usr/local/bin/ascii_paint_tests"]