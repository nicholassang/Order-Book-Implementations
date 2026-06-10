#include "OrderBook_Vector.h"

int OrderBook_Vector::findPriceLevelIndex(const std::vector<PriceLevel>& levels, double price, Side side) const {
    if (side == Side::BUY) {
        // Bids sorted descending
        for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
            if (levels[i].price == price) return i;
            if (levels[i].price < price) return -1;
        }
    } else {
        // Asks sorted ascending
        for (int i = 0; i < static_cast<int>(levels.size()); ++i) {
            if (levels[i].price == price) return i;
            if (levels[i].price > price) return -1;
        }
    }
    return -1;
}

void OrderBook_Vector::insertPriceLevel(std::vector<PriceLevel>& levels, double price, Side side) {
    if (side == Side::BUY) {
        // Insert bids in descending order
        auto it = levels.begin();
        while (it != levels.end() && it->price > price) {
            ++it;
        }
        levels.insert(it, {price, {}});
    } else {
        // Insert asks in ascending order
        auto it = levels.begin();
        while (it != levels.end() && it->price < price) {
            ++it;
        }
        levels.insert(it, {price, {}});
    }
}

void OrderBook_Vector::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        matchBuy(order);
    } else {
        matchSell(order);
    }
}

bool OrderBook_Vector::cancelOrder(uint64_t orderId) {
    auto it = orderIndex.find(orderId);
    if (it == orderIndex.end()) {
        return false;
    }

    Side side = it->second.first;
    size_t levelIndex = it->second.second;
    auto& levels = (side == Side::BUY) ? bids : asks;

    if (levelIndex < levels.size()) {
        auto& level = levels[levelIndex];
        auto orderIt = std::find_if(level.orders.begin(), level.orders.end(),
                                     [orderId](const Order& o) { return o.id == orderId; });
        if (orderIt != level.orders.end()) {
            level.orders.erase(orderIt);
            if (level.orders.empty()) {
                levels.erase(levels.begin() + levelIndex);
            }
        }
    }

    orderIndex.erase(it);
    return true;
}

void OrderBook_Vector::matchBuy(Order incoming) {
    while (incoming.quantity > 0 && !asks.empty() && asks[0].price <= incoming.price) {
        auto& ordersAtPrice = asks[0].orders;
        auto it = ordersAtPrice.begin();

        uint32_t traded = std::min(incoming.quantity, it->quantity);
        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderIndex.erase(it->id);
            ordersAtPrice.erase(it);
        }

        if (ordersAtPrice.empty()) {
            asks.erase(asks.begin());
        }
    }

    if (incoming.quantity > 0) {
        int levelIdx = findPriceLevelIndex(bids, incoming.price, Side::BUY);
        if (levelIdx == -1) {
            insertPriceLevel(bids, incoming.price, Side::BUY);
            levelIdx = findPriceLevelIndex(bids, incoming.price, Side::BUY);
        }

        bids[levelIdx].orders.push_back(incoming);
        orderIndex[incoming.id] = {Side::BUY, levelIdx};
    }
}

void OrderBook_Vector::matchSell(Order incoming) {
    while (incoming.quantity > 0 && !bids.empty() && bids[0].price >= incoming.price) {
        auto& ordersAtPrice = bids[0].orders;
        auto it = ordersAtPrice.begin();

        uint32_t traded = std::min(incoming.quantity, it->quantity);
        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderIndex.erase(it->id);
            ordersAtPrice.erase(it);
        }

        if (ordersAtPrice.empty()) {
            bids.erase(bids.begin());
        }
    }

    if (incoming.quantity > 0) {
        int levelIdx = findPriceLevelIndex(asks, incoming.price, Side::SELL);
        if (levelIdx == -1) {
            insertPriceLevel(asks, incoming.price, Side::SELL);
            levelIdx = findPriceLevelIndex(asks, incoming.price, Side::SELL);
        }

        asks[levelIdx].orders.push_back(incoming);
        orderIndex[incoming.id] = {Side::SELL, levelIdx};
    }
}

size_t OrderBook_Vector::getBidCount() const {
    size_t count = 0;
    for (const auto& level : bids) {
        count += level.orders.size();
    }
    return count;
}

size_t OrderBook_Vector::getAskCount() const {
    size_t count = 0;
    for (const auto& level : asks) {
        count += level.orders.size();
    }
    return count;
}
