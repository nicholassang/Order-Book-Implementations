#pragma once

#include "Order.h"
#include <vector>
#include <string>
#include <random>

class MarketDataGenerator {
private:
    std::mt19937 rng;
    uint64_t nextOrderId = 1;
    Order generateSingleOrder(double price);
    
public:
    MarketDataGenerator(uint32_t seed = 42);
    
    // Generate orders for different market conditions
    std::vector<Order> generateNormalMarket(uint32_t numOrders, double basePrice, double volatility);
    std::vector<Order> generateMarketSpike(uint32_t numOrders, double basePrice, uint32_t spikeAt);
    std::vector<Order> generateTrendingMarket(uint32_t numOrders, double basePrice, double trendDirection);
    std::vector<Order> generateHighFrequency(uint32_t numOrders, double basePrice);
    std::vector<Order> generateRandomWalk(uint32_t numOrders, double basePrice, double stepSize);
    
    // Save and load
    void saveOrdersToCSV(const std::vector<Order>& orders, const std::string& filename);
    std::vector<Order> loadOrdersFromCSV(const std::string& filename);
};
