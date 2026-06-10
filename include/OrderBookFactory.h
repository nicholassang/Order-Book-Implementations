#pragma once

#include <cstdint>

enum class OrderBookImpl {
    MAP,
    UNORDERED_MAP,
    VECTOR,
    PRIORITY_QUEUE
};
