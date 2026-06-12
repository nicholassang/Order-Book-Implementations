cmake_minimum_required(VERSION 3.20)

project(OrderBook)

set(CMAKE_CXX_STANDARD 20)

# Get Google Benchmark
include(FetchContent)


set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_USE_BUNDLED_GTEST OFF)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.4
)

set(BENCHMARK_ENABLE_TESTING OFF)

FetchContent_MakeAvailable(benchmark)

# Core library with map-based implementation
add_library(orderbook
    src/OrderBook.cpp
    data_structures/OrderBook_MapBased.cpp
    data_structures/OrderBook_UnorderedMap.cpp
    data_structures/OrderBook_Vector.cpp
    data_structures/OrderBook_PriorityQueue.cpp
    data/MarketDataGenerator.cpp
)

target_include_directories(
    orderbook
    PUBLIC include
    PUBLIC data_structures
    PUBLIC data
)

# Main demo executable
add_executable(orderbook_demo
    apps/main.cpp
)

target_link_libraries(orderbook_demo
    PRIVATE orderbook
)

# Benchmark executable
add_executable(orderbook_benchmark
    benchmarks/BenchmarkRunner.cpp
)

# Benchmark
target_link_libraries(orderbook_benchmark
    PRIVATE
        orderbook
        benchmark::benchmark
        benchmark::benchmark_main
)

# Test executable
add_executable(orderbook_test
    tests/test_implementations.cpp
)

target_link_libraries(orderbook_test
    PRIVATE orderbook
)

enable_testing()
add_test(NAME OrderBookTests COMMAND orderbook_test)