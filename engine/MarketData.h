#pragma once

#include <cstdint>
#include <chrono>

struct BestPrice {
    double bid;
    double ask;
    uint64_t timestamp;
    double spread() const { return ask - bid; }
};

class MarketData {
private:
    BestPrice currentBestPrice{0, 0, 0};
    uint64_t tickCount = 0;
    
public:
    void updateBestPrice(double bid, double ask);
    BestPrice getBestPrice() const { return currentBestPrice; }
    uint64_t getTickCount() const { return tickCount; }
    
    bool isValid() const { return currentBestPrice.bid > 0 && currentBestPrice.ask > 0; }
};
