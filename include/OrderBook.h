#pragma once

#include "Order.h"
#include "Trade.h"

#include <list>
#include <map>
#include <unordered_map>
#include <vector>

class OrderBook {
private:
    struct OrderLocation {
        Side side;
        double price;
        std::list<Order>::iterator it;
    };

    std::map<double, std::list<Order>, std::greater<>> bids;
    std::map<double, std::list<Order>> asks;

    std::unordered_map<uint64_t, OrderLocation> orderIndex;

    void matchBuy(Order incoming);
    void matchSell(Order incoming);
    
public:
    void addOrder(const Order& order);
    bool cancelOrder(uint64_t orderId);
    const auto& getBids() const { return bids; }
    const auto& getAsks() const { return asks; }
};