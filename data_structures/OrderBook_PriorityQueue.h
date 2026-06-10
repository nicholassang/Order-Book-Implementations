#pragma once

#include "Order.h"
#include "Trade.h"

#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>

// Priority queue-based implementation (optimized for fast best price access)
class OrderBook_PriorityQueue {
private:
    struct PriceLevel {
        double price;
        std::vector<Order> orders;
    };

    struct CompareBids {
        bool operator()(const PriceLevel& a, const PriceLevel& b) const {
            return a.price < b.price;  // max heap
        }
    };

    struct CompareAsks {
        bool operator()(const PriceLevel& a, const PriceLevel& b) const {
            return a.price > b.price;  // min heap
        }
    };

    std::priority_queue<PriceLevel, std::vector<PriceLevel>, CompareBids> bidQueue;
    std::priority_queue<PriceLevel, std::vector<PriceLevel>, CompareAsks> askQueue;
    
    std::unordered_map<uint64_t, std::pair<Side, double>> orderIndex;  // orderId -> (side, price)
    std::unordered_map<uint64_t, std::vector<Order>> priceLevelMap;    // price_key -> orders at price

    void matchBuy(Order incoming);
    void matchSell(Order incoming);
    
public:
    void addOrder(const Order& order);
    bool cancelOrder(uint64_t orderId);
    
    size_t getBidCount() const;
    size_t getAskCount() const;
};
