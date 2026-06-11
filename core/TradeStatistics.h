#pragma once

#include <cstdint>
#include <vector>
#include <string>

struct Trade {
    uint64_t buy_id;
    uint64_t sell_id;
    double price;
    uint32_t quantity;
    uint64_t timestamp;  // Unix microseconds
    double fee;
    
    double getTotalValue() const { return price * quantity; }
};

struct OrderStatistics {
    uint64_t totalTradeCount = 0;
    uint64_t totalVolumeTraded = 0;
    double totalValueTraded = 0.0;
    double totalFeesPaid = 0.0;
    
    double averageTradePrice() const {
        return totalVolumeTraded > 0 ? totalValueTraded / totalVolumeTraded : 0.0;
    }
};

class TradeStatistics {
private:
    std::vector<Trade> trades;
    OrderStatistics stats;
    
public:
    void recordTrade(const Trade& trade);
    void recordTrades(const std::vector<Trade>& newTrades);
    
    const std::vector<Trade>& getTrades() const { return trades; }
    const OrderStatistics& getStatistics() const { return stats; }
    
    uint64_t getTradeCount() const { return stats.totalTradeCount; }
    uint64_t getTotalVolume() const { return stats.totalVolumeTraded; }
    double getTotalValue() const { return stats.totalValueTraded; }
    double getAveragePrice() const { return stats.averageTradePrice(); }
    double getTotalFees() const { return stats.totalFeesPaid; }
    
    void clear();
    std::string getSummary() const;
};
