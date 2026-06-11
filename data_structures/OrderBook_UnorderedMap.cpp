#include "OrderBook_UnorderedMap.h"
#include <cmath>
#include <limits>

// Helper: Convert double price to uint64_t key (price * 100)
static inline uint64_t priceToKey(double price) {
    return static_cast<uint64_t>(std::round(price * 100));
}

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

    uint64_t priceKey = priceIt->second;
    Side side = orderSideMap[orderId];

    auto& levels = (side == Side::BUY) ? bidLevels : askLevels;
    auto levelIt = levels.find(priceKey);
    
    if (levelIt == levels.end()) {
        return false;
    }

    auto& orders = levelIt->second.orders;
    auto orderIt = std::find_if(orders.begin(), orders.end(),
                                [orderId](const Order& o) { return o.id == orderId; });
    
    if (orderIt != orders.end()) {
        orders.erase(orderIt);
        if (orders.empty()) {
            levels.erase(levelIt);
        }
    }

    orderPriceMap.erase(priceIt);
    orderSideMap.erase(orderId);

    return true;
}

void OrderBook_UnorderedMap::matchBuy(Order incoming) {
    uint64_t incomingPriceKey = priceToKey(incoming.price);
    
    // Find best asks and match
    while (incoming.quantity > 0 && !askLevels.empty()) {
        uint64_t bestAskPrice = std::numeric_limits<uint64_t>::max();
        findBestAsk(bestAskPrice);

        // Check if best ask price is not found or exceeds incoming bid price
        if (bestAskPrice == std::numeric_limits<uint64_t>::max() || bestAskPrice > incomingPriceKey) {
            break;
        }

        auto& ordersAtPrice = askLevels[bestAskPrice].orders;
        auto it = ordersAtPrice.begin();

        uint32_t traded = std::min(incoming.quantity, it->quantity);
        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderPriceMap.erase(it->id);
            orderSideMap.erase(it->id);
            ordersAtPrice.erase(it);
        }

        if (ordersAtPrice.empty()) {
            askLevels.erase(bestAskPrice);
        }
    }

    // Add remaining quantity to bid book
    if (incoming.quantity > 0) {
        bidLevels[incomingPriceKey].orders.push_back(incoming);
        orderPriceMap[incoming.id] = incomingPriceKey;
        orderSideMap[incoming.id] = Side::BUY;
    }
}

void OrderBook_UnorderedMap::matchSell(Order incoming) {
    uint64_t incomingPriceKey = priceToKey(incoming.price);
    
    // Find best bids and match
    while (incoming.quantity > 0 && !bidLevels.empty()) {
        uint64_t bestBidPrice = 0;
        findBestBid(bestBidPrice);

        // Check if best bid price is not found or is less than incoming ask price
        if (bestBidPrice == 0 || bestBidPrice < incomingPriceKey) {
            break;
        }

        auto& ordersAtPrice = bidLevels[bestBidPrice].orders;
        auto it = ordersAtPrice.begin();

        uint32_t traded = std::min(incoming.quantity, it->quantity);
        incoming.quantity -= traded;
        it->quantity -= traded;

        if (it->quantity == 0) {
            orderPriceMap.erase(it->id);
            orderSideMap.erase(it->id);
            ordersAtPrice.erase(it);
        }

        if (ordersAtPrice.empty()) {
            bidLevels.erase(bestBidPrice);
        }
    }

    // Add remaining quantity to ask book
    if (incoming.quantity > 0) {
        askLevels[incomingPriceKey].orders.push_back(incoming);
        orderPriceMap[incoming.id] = incomingPriceKey;
        orderSideMap[incoming.id] = Side::SELL;
    }
}

void OrderBook_UnorderedMap::findBestBid(uint64_t& price) const {
    price = 0;
    for (const auto& [p, level] : bidLevels) {
        if (!level.orders.empty() && p > price) {
            price = p;
        }
    }
}

void OrderBook_UnorderedMap::findBestAsk(uint64_t& price) const {
    price = std::numeric_limits<uint64_t>::max();
    for (const auto& [p, level] : askLevels) {
        if (!level.orders.empty() && p < price) {
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
