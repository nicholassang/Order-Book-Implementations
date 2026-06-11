#pragma once

#include "Trade.h"
#include <vector>
#include <fstream>
#include <chrono>

class TradeLedger {
private:
    std::vector<Trade> trades;
    std::string filename;
    bool persistentMode = false;
    
public:
    TradeLedger() = default;
    explicit TradeLedger(const std::string& filepath) : filename(filepath), persistentMode(true) {}
    
    // Recording trades
    void recordTrade(const Trade& trade);
    void recordTrades(const std::vector<Trade>& newTrades);
    
    // Querying
    const std::vector<Trade>& getAllTrades() const { return trades; }
    const Trade* findTrade(uint64_t tradeId) const;
    std::vector<Trade> getTradesByBuyer(uint64_t buyerId) const;
    std::vector<Trade> getTradesBySeller(uint64_t sellerId) const;
    
    // Statistics
    uint64_t getTotalVolume() const;
    double getTotalValue() const;
    double getAveragePrice() const;
    
    // Persistence
    bool saveToCSV(const std::string& filename) const;
    bool loadFromCSV(const std::string& filename);
    
    void clear() { trades.clear(); }
    size_t size() const { return trades.size(); }
};
