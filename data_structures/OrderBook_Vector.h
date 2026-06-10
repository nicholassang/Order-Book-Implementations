#pragma once

#include "Order.h"
#include "Trade.h"

#include <vector>
#include <algorithm>
#include <unordered_map>

// Vector-based implementation (good cache locality, requires sorting)
class OrderBook_Vector {
private:
    struct PriceLevel {
        double price;
        std::vector<Order> orders;
    };

    std::vector<PriceLevel> bids;
    std::vector<PriceLevel> asks;
    std::unordered_map<uint64_t, std::pair<Side, size_t>> orderIndex;  // orderId -> (side, price_level_index)

    int findPriceLevelIndex(const std::vector<PriceLevel>& levels, double price, Side side) const;
    void insertPriceLevel(std::vector<PriceLevel>& levels, double price, Side side);
    
    void matchBuy(Order incoming);
    void matchSell(Order incoming);
    
public:
    void addOrder(const Order& order);
    bool cancelOrder(uint64_t orderId);
    
    size_t getBidCount() const;
    size_t getAskCount() const;
};
