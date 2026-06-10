#pragma once

#include "Order.h"
#include "Trade.h"

#include <unordered_map>
#include <vector>
#include <algorithm>

// Unordered map implementation (fastest insertion/deletion, slower matching)
class OrderBook_UnorderedMap {
private:
    struct PriceLevel {
        std::vector<Order> orders;
    };

    std::unordered_map<uint64_t, uint64_t> orderPriceMap;  // orderId -> price
    std::unordered_map<uint64_t, size_t> orderIndexMap;     // orderId -> index in price level
    std::unordered_map<uint64_t, Side> orderSideMap;        // orderId -> side
    
    std::unordered_map<uint64_t, PriceLevel> bidLevels;
    std::unordered_map<uint64_t, PriceLevel> askLevels;

    void matchBuy(Order incoming);
    void matchSell(Order incoming);
    void findBestBid(uint64_t& price) const;
    void findBestAsk(uint64_t& price) const;
    
public:
    void addOrder(const Order& order);
    bool cancelOrder(uint64_t orderId);
    
    size_t getBidCount() const;
    size_t getAskCount() const;
};
