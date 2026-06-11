#pragma once

#include <cstdint>

enum class OrderState {
    PENDING,
    ACKNOWLEDGED,
    PARTIALLY_FILLED,
    FULLY_FILLED,
    CANCELLED,
    REJECTED,
    EXPIRED
};

inline const char* toString(OrderState state) {
    switch (state) {
        case OrderState::PENDING: return "PENDING";
        case OrderState::ACKNOWLEDGED: return "ACKNOWLEDGED";
        case OrderState::PARTIALLY_FILLED: return "PARTIALLY_FILLED";
        case OrderState::FULLY_FILLED: return "FULLY_FILLED";
        case OrderState::CANCELLED: return "CANCELLED";
        case OrderState::REJECTED: return "REJECTED";
        case OrderState::EXPIRED: return "EXPIRED";
        default: return "UNKNOWN";
    }
}
