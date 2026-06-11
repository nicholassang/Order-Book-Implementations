#include "MatchingEngine.h"
#include <iostream>

bool MatchingEngine::validateOrder(const Order& order, std::string& rejectReason) {
    // Validate order ID
    if (order.id == 0) {
        rejectReason = "Invalid order ID (0 not allowed)";
        return false;
    }
    
    // Validate quantity
    if (order.quantity == 0 || order.quantity > 1000000) {
        rejectReason = "Invalid quantity (must be 1-1,000,000)";
        return false;
    }
    
    // Validate price
    if (order.price <= 0 || order.price > 100000) {
        rejectReason = "Invalid price (must be $0.01-$100,000)";
        return false;
    }
    
    return true;
}

bool MatchingEngine::submitOrder(const Order& order, std::string& rejectReason) {
    // Validate order
    if (!validateOrder(order, rejectReason)) {
        rejectOrder(order.id, rejectReason);
        return false;
    }
    
    // Add order to order book
    orderBook.addOrder(order);
    
    // Record in statistics
    statistics.recordTrade({order.id, 0, order.price, order.quantity, 0, 0});
    
    return true;
}

bool MatchingEngine::cancelOrder(uint64_t orderId) {
    return orderBook.cancelOrder(orderId);
}

double MatchingEngine::getClientExposure(uint64_t clientId) const {
    auto it = clientExposure.find(clientId);
    return it != clientExposure.end() ? it->second : 0.0;
}

std::string MatchingEngine::getMarketSummary() const {
    std::string summary;
    
    const auto& bids = orderBook.getBids();
    const auto& asks = orderBook.getAsks();
    
    if (!bids.empty() && !asks.empty()) {
        double bestBid = bids.begin()->first;
        double bestAsk = asks.begin()->first;
        double spread = bestAsk - bestBid;
        double spreadBps = (spread / bestBid) * 10000;
        
        summary += "Best Bid: $" + std::to_string(bestBid) + "\n";
        summary += "Best Ask: $" + std::to_string(bestAsk) + "\n";
        summary += "Spread: $" + std::to_string(spread) + " (" + std::to_string(spreadBps) + " bps)\n";
    }
    
    summary += statistics.getSummary();
    return summary;
}

void MatchingEngine::executeTrade(const Trade& trade) {
    statistics.recordTrade(trade);
    
    for (auto& callback : tradeCallbacks) {
        callback(trade);
    }
}

void MatchingEngine::rejectOrder(uint64_t orderId, const std::string& reason) {
    for (auto& callback : rejectCallbacks) {
        callback(orderId, reason);
    }
}
