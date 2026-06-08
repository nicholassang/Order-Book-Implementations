cmake_minimum_required(VERSION 3.20)

project(OrderBook
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Core library
add_library(orderbook
    src/OrderBook.cpp
    src/MatchingEngine.cpp
    src/Order.cpp

    core/BookSide.cpp
    core/PriceLevel.cpp
    core/OrderIndex.cpp

    engine/Engine.cpp
    engine/EventProcessor.cpp
    engine/RiskChecks.cpp
)

target_include_directories(orderbook
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# Main executable
add_executable(orderbook_demo
    apps/main.cpp
)

target_link_libraries(orderbook_demo
    PRIVATE
        orderbook
)

# Replay executable
add_executable(replay
    apps/replay.cpp
)

target_link_libraries(replay
    PRIVATE
        orderbook
)

# Simulator executable
add_executable(simulator
    apps/simulator.cpp
)

target_link_libraries(simulator
    PRIVATE
        orderbook
)

enable_testing()