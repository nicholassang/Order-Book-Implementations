#include "OrderBook_PriorityQueue.h"

void OrderBook_PriorityQueue::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        matchBuy(order);
    } else {
        matchSell(order);
    }
}

bool OrderBook_PriorityQueue::cancelOrder(uint64_t orderId) {
    auto it = orderIndex.find(orderId);
    if (it == orderIndex.end()) {
        return false;
    }

    Side side = it->second.first;
    double price = it->second.second;
    uint64_t priceKey = static_cast<uint64_t>(price * 100);

    auto& priceLevel = priceLevelMap[priceKey];
    auto orderIt = std::find_if(priceLevel.begin(), priceLevel.end(),
                                 [orderId](const Order& o) { return o.id == orderId; });
    if (orderIt != priceLevel.end()) {
        priceLevel.erase(orderIt);
        if (priceLevel.empty()) {
            priceLevelMap.erase(priceKey);
        }
    }

    orderIndex.erase(it);
    return true;
}

void OrderBook_PriorityQueue::matchBuy(Order incoming) {
    // Match against asks
    while (incoming.quantity > 0 && !priceLevelMap.empty()) {
        // Find best ask
        double bestAskPrice = std::numeric_limits<double>::max();
        uint64_t bestAskKey = 0;

        for (const auto& [key, orders] : priceLevelMap) {
            double price = key / 100.0;
            if (!orders.empty() && orders[0].side == Side::SELL && price < bestAskPrice) {
                bestAskPrice = price;
                bestAskKey = key;
            }
        }

        if (bestAskPrice > incoming.price || bestAskKey == 0) {
            break;
        }

        auto& ordersAtPrice = priceLevelMap[bestAskKey];
        auto it = ordersAtPrice.begin();

        uint32_t traded = std::min(incoming.quantity, it->quantity);
        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderIndex.erase(it->id);
            ordersAtPrice.erase(it);
        }

        if (ordersAtPrice.empty()) {
            priceLevelMap.erase(bestAskKey);
        }
    }

    if (incoming.quantity > 0) {
        uint64_t priceKey = static_cast<uint64_t>(incoming.price * 100);
        priceLevelMap[priceKey].push_back(incoming);
        orderIndex[incoming.id] = {Side::BUY, incoming.price};

        PriceLevel level;
        level.price = incoming.price;
        bidQueue.push(level);
    }
}

void OrderBook_PriorityQueue::matchSell(Order incoming) {
    // Match against bids
    while (incoming.quantity > 0 && !priceLevelMap.empty()) {
        // Find best bid
        double bestBidPrice = 0;
        uint64_t bestBidKey = 0;

        for (const auto& [key, orders] : priceLevelMap) {
            double price = key / 100.0;
            if (!orders.empty() && orders[0].side == Side::BUY && price > bestBidPrice) {
                bestBidPrice = price;
                bestBidKey = key;
            }
        }

        if (bestBidPrice < incoming.price || bestBidKey == 0) {
            break;
        }

        auto& ordersAtPrice = priceLevelMap[bestBidKey];
        auto it = ordersAtPrice.begin();

        uint32_t traded = std::min(incoming.quantity, it->quantity);
        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderIndex.erase(it->id);
            ordersAtPrice.erase(it);
        }

        if (ordersAtPrice.empty()) {
            priceLevelMap.erase(bestBidKey);
        }
    }

    if (incoming.quantity > 0) {
        uint64_t priceKey = static_cast<uint64_t>(incoming.price * 100);
        priceLevelMap[priceKey].push_back(incoming);
        orderIndex[incoming.id] = {Side::SELL, incoming.price};

        PriceLevel level;
        level.price = incoming.price;
        askQueue.push(level);
    }
}

size_t OrderBook_PriorityQueue::getBidCount() const {
    size_t count = 0;
    for (const auto& [key, orders] : priceLevelMap) {
        if (!orders.empty() && orders[0].side == Side::BUY) {
            count += orders.size();
        }
    }
    return count;
}

size_t OrderBook_PriorityQueue::getAskCount() const {
    size_t count = 0;
    for (const auto& [key, orders] : priceLevelMap) {
        if (!orders.empty() && orders[0].side == Side::SELL) {
            count += orders.size();
        }
    }
    return count;
}
