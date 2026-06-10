#include "OrderBook_MapBased.h"

void OrderBook_MapBased::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        matchBuy(order);
    } else {
        matchSell(order);
    }
}

bool OrderBook_MapBased::cancelOrder(uint64_t orderId) {
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

void OrderBook_MapBased::matchBuy(Order incoming) {
    while (incoming.quantity > 0 &&
           !asks.empty() &&
           asks.begin()->first <= incoming.price) {

        auto& ordersAtPrice = asks.begin()->second;
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
        auto& ordersAtPrice = bids[incoming.price];
        ordersAtPrice.push_back(incoming);

        auto it = std::prev(ordersAtPrice.end());

        orderIndex[incoming.id] = {Side::BUY, incoming.price, it};
    }
}

void OrderBook_MapBased::matchSell(Order incoming) {
    while (incoming.quantity > 0 &&
           !bids.empty() &&
           bids.begin()->first >= incoming.price) {

        auto& ordersAtPrice = bids.begin()->second;
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
        auto& ordersAtPrice = asks[incoming.price];
        ordersAtPrice.push_back(incoming);

        auto it = std::prev(ordersAtPrice.end());

        orderIndex[incoming.id] = {Side::SELL, incoming.price, it};
    }
}

size_t OrderBook_MapBased::getBidCount() const {
    size_t count = 0;
    for (const auto& [price, orders] : bids) {
        count += orders.size();
    }
    return count;
}

size_t OrderBook_MapBased::getAskCount() const {
    size_t count = 0;
    for (const auto& [price, orders] : asks) {
        count += orders.size();
    }
    return count;
}
