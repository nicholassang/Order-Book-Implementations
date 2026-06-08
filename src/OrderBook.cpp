#include "OrderBook.h"

void OrderBook::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        matchBuy(order);
    } else {
        matchSell(order);
    }
}

bool OrderBook::cancelOrder(uint64_t orderId) {
    auto idx = orderIndex.find(orderId);

    if (idx == orderIndex.end()) {
        return false;
    }

    auto& loc = idx->second;

    if (loc.side == Side::BUY) {
        bids[loc.price].erase(loc.it);
    } else {
        asks[loc.price].erase(loc.it);
    }

    orderIndex.erase(idx);
    return true;
}

void OrderBook::matchBuy(Order incoming) {

    while (incoming.quantity > 0 &&
           !asks.empty() &&
           asks.begin()->first <= incoming.price) {

        auto& level = asks.begin()->second;
        auto it = level.begin();

        uint32_t traded =
            std::min(incoming.quantity, it->quantity);

        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderIndex.erase(it->id);
            level.erase(it);
        }

        if (level.empty()) {
            asks.erase(asks.begin());
        }
    }

    if (incoming.quantity > 0) {
        auto& level = bids[incoming.price];
        level.push_back(incoming);

        auto it = std::prev(level.end());

        orderIndex[incoming.id] =
            {Side::BUY, incoming.price, it};
    }
}

void OrderBook::matchSell(Order incoming) {

    while (incoming.quantity > 0 &&
           !bids.empty() &&
           bids.begin()->first >= incoming.price) {

        auto& level = bids.begin()->second;
        auto it = level.begin();

        uint32_t traded =
            std::min(incoming.quantity, it->quantity);

        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderIndex.erase(it->id);
            level.erase(it);
        }

        if (level.empty()) {
            bids.erase(bids.begin());
        }
    }

    if (incoming.quantity > 0) {
        auto& level = asks[incoming.price];
        level.push_back(incoming);

        auto it = std::prev(level.end());

        orderIndex[incoming.id] =
            {Side::SELL, incoming.price, it};
    }
}