cmake_minimum_required(VERSION 3.20)

project(OrderBook)

set(CMAKE_CXX_STANDARD 20)

add_library(orderbook
    src/OrderBook.cpp
)

target_include_directories(orderbook
    PUBLIC include
)

add_executable(orderbook_demo
    apps/main.cpp
)

target_link_libraries(orderbook_demo
    PRIVATE orderbook
)