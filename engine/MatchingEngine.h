#pragma once

#include "Order.h"
#include "OrderBook.h"
#include "Types.h"
#include "TradeStatistics.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

// Callback for trade executions
using TradeCallback = std::function<void(const Trade&)>;
using OrderRejectCallback = std::function<void(uint64_t orderId, const std::string& reason)>;

class MatchingEngine {
private:
    OrderBook orderBook;
    TradeStatistics statistics;
    std::vector<TradeCallback> tradeCallbacks;
    std::vector<OrderRejectCallback> rejectCallbacks;
    
    // Risk management
    std::unordered_map<uint64_t, double> clientExposure;  // Client ID -> total exposure
    double maxExposurePerClient = 1000000.0;  // $1M default
    
public:
    MatchingEngine() = default;
    
    // Order submission
    bool submitOrder(const Order& order, std::string& rejectReason);
    
    // Order management
    bool cancelOrder(uint64_t orderId);
    
    // Query
    const OrderBook& getOrderBook() const { return orderBook; }
    const TradeStatistics& getStatistics() const { return statistics; }
    
    // Callbacks
    void onTrade(TradeCallback callback) { tradeCallbacks.push_back(callback); }
    void onReject(OrderRejectCallback callback) { rejectCallbacks.push_back(callback); }
    
    // Risk management
    void setMaxExposurePerClient(double maxExposure) { maxExposurePerClient = maxExposure; }
    double getClientExposure(uint64_t clientId) const;
    
    // Statistics
    std::string getMarketSummary() const;
    
private:
    bool validateOrder(const Order& order, std::string& rejectReason);
    void executeTrade(const Trade& trade);
    void rejectOrder(uint64_t orderId, const std::string& reason);
};
