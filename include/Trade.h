#pragma once

#include <cstdint>

struct Trade {
    uint64_t buy_id;
    uint64_t sell_id;
    double price;
    uint32_t quantity;
};