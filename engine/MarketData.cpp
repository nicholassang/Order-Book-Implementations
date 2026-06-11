#include "MarketData.h"
#include <chrono>

void MarketData::updateBestPrice(double bid, double ask) {
    currentBestPrice.bid = bid;
    currentBestPrice.ask = ask;
    currentBestPrice.timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    tickCount++;
}
