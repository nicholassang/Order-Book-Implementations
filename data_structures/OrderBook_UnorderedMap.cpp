#include "OrderBook_UnorderedMap.h"
#include <cmath>

void OrderBook_UnorderedMap::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        matchBuy(order);
    } else {
        matchSell(order);
    }
}

bool OrderBook_UnorderedMap::cancelOrder(uint64_t orderId) {
    auto priceIt = orderPriceMap.find(orderId);
    if (priceIt == orderPriceMap.end()) {
        return false;
    }

    uint64_t price = priceIt->second;
    Side side = orderSideMap[orderId];
    size_t index = orderIndexMap[orderId];

    auto& level = (side == Side::BUY) ? bidLevels[price] : askLevels[price];
    level.orders.erase(level.orders.begin() + index);

    orderPriceMap.erase(priceIt);
    orderIndexMap.erase(orderId);
    orderSideMap.erase(orderId);

    return true;
}

void OrderBook_UnorderedMap::matchBuy(Order incoming) {
    // Find best asks and match
    while (incoming.quantity > 0 && !askLevels.empty()) {
        uint64_t bestAskPrice = 0;
        findBestAsk(bestAskPrice);

        if (askLevels.find(bestAskPrice) == askLevels.end() || bestAskPrice > static_cast<uint64_t>(incoming.price)) {
            break;
        }

        auto& ordersAtPrice = askLevels[bestAskPrice].orders;
        auto it = ordersAtPrice.begin();

        uint32_t traded = std::min(incoming.quantity, it->quantity);
        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderPriceMap.erase(it->id);
            orderIndexMap.erase(it->id);
            orderSideMap.erase(it->id);
            ordersAtPrice.erase(it);
        }

        if (ordersAtPrice.empty()) {
            askLevels.erase(bestAskPrice);
        }
    }

    if (incoming.quantity > 0) {
        uint64_t priceKey = static_cast<uint64_t>(incoming.price * 100);
        bidLevels[priceKey].orders.push_back(incoming);
        orderPriceMap[incoming.id] = priceKey;
        orderIndexMap[incoming.id] = bidLevels[priceKey].orders.size() - 1;
        orderSideMap[incoming.id] = Side::BUY;
    }
}

void OrderBook_UnorderedMap::matchSell(Order incoming) {
    // Find best bids and match
    while (incoming.quantity > 0 && !bidLevels.empty()) {
        uint64_t bestBidPrice = 0;
        findBestBid(bestBidPrice);

        if (bidLevels.find(bestBidPrice) == bidLevels.end() || bestBidPrice < static_cast<uint64_t>(incoming.price)) {
            break;
        }

        auto& ordersAtPrice = bidLevels[bestBidPrice].orders;
        auto it = ordersAtPrice.begin();

        uint32_t traded = std::min(incoming.quantity, it->quantity);
        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderPriceMap.erase(it->id);
            orderIndexMap.erase(it->id);
            orderSideMap.erase(it->id);
            ordersAtPrice.erase(it);
        }

        if (ordersAtPrice.empty()) {
            bidLevels.erase(bestBidPrice);
        }
    }

    if (incoming.quantity > 0) {
        uint64_t priceKey = static_cast<uint64_t>(incoming.price * 100);
        askLevels[priceKey].orders.push_back(incoming);
        orderPriceMap[incoming.id] = priceKey;
        orderIndexMap[incoming.id] = askLevels[priceKey].orders.size() - 1;
        orderSideMap[incoming.id] = Side::SELL;
    }
}

void OrderBook_UnorderedMap::findBestBid(uint64_t& price) const {
    price = 0;
    for (const auto& [p, level] : bidLevels) {
        if (p > price) {
            price = p;
        }
    }
}

void OrderBook_UnorderedMap::findBestAsk(uint64_t& price) const {
    price = UINT64_MAX;
    for (const auto& [p, level] : askLevels) {
        if (p < price) {
            price = p;
        }
    }
}

size_t OrderBook_UnorderedMap::getBidCount() const {
    size_t count = 0;
    for (const auto& [price, level] : bidLevels) {
        count += level.orders.size();
    }
    return count;
}

size_t OrderBook_UnorderedMap::getAskCount() const {
    size_t count = 0;
    for (const auto& [price, level] : askLevels) {
        count += level.orders.size();
    }
    return count;
}
