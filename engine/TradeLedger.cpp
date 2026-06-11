#include "TradeLedger.h"
#include <algorithm>

void TradeLedger::recordTrade(const Trade& trade) {
    trades.push_back(trade);
}

void TradeLedger::recordTrades(const std::vector<Trade>& newTrades) {
    for (const auto& trade : newTrades) {
        recordTrade(trade);
    }
}

const Trade* TradeLedger::findTrade(uint64_t tradeId) const {
    for (const auto& trade : trades) {
        if (trade.buy_id == tradeId || trade.sell_id == tradeId) {
            return &trade;
        }
    }
    return nullptr;
}

std::vector<Trade> TradeLedger::getTradesByBuyer(uint64_t buyerId) const {
    std::vector<Trade> result;
    for (const auto& trade : trades) {
        if (trade.buy_id == buyerId) {
            result.push_back(trade);
        }
    }
    return result;
}

std::vector<Trade> TradeLedger::getTradesBySeller(uint64_t sellerId) const {
    std::vector<Trade> result;
    for (const auto& trade : trades) {
        if (trade.sell_id == sellerId) {
            result.push_back(trade);
        }
    }
    return result;
}

uint64_t TradeLedger::getTotalVolume() const {
    uint64_t volume = 0;
    for (const auto& trade : trades) {
        volume += trade.quantity;
    }
    return volume;
}

double TradeLedger::getTotalValue() const {
    double value = 0;
    for (const auto& trade : trades) {
        value += trade.price * trade.quantity;
    }
    return value;
}

double TradeLedger::getAveragePrice() const {
    uint64_t volume = getTotalVolume();
    return volume > 0 ? getTotalValue() / volume : 0.0;
}

bool TradeLedger::saveToCSV(const std::string& filepath) const {
    std::ofstream file(filepath);
    file << "buy_id,sell_id,price,quantity,timestamp,fee\n";
    
    for (const auto& trade : trades) {
        file << trade.buy_id << ","
             << trade.sell_id << ","
             << trade.price << ","
             << trade.quantity << ","
             << trade.timestamp << ","
             << trade.fee << "\n";
    }
    
    file.close();
    return true;
}

bool TradeLedger::loadFromCSV(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;
    
    std::string line;
    std::getline(file, line);  // Skip header
    
    while (std::getline(file, line)) {
        // Parse CSV... (simplified for now)
    }
    
    file.close();
    return true;
}
