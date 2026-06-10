#include "MarketDataGenerator.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>

MarketDataGenerator::MarketDataGenerator(uint32_t seed) : rng(seed) {}

Order MarketDataGenerator::generateSingleOrder(double price) {
    std::uniform_int_distribution<> sideDist(0, 1);
    std::uniform_int_distribution<uint32_t> quantityDist(1, 1000);
    
    Side side = sideDist(rng) == 0 ? Side::BUY : Side::SELL;
    uint32_t quantity = quantityDist(rng);
    
    return {nextOrderId++, side, price, quantity};
}

std::vector<Order> MarketDataGenerator::generateNormalMarket(uint32_t numOrders, double basePrice, double volatility) {
    std::vector<Order> orders;
    std::normal_distribution<double> priceDist(0, volatility);
    
    for (uint32_t i = 0; i < numOrders; ++i) {
        double price = basePrice + priceDist(rng);
        price = std::max(price, 0.01);
        orders.push_back(generateSingleOrder(price));
    }
    
    return orders;
}

std::vector<Order> MarketDataGenerator::generateMarketSpike(uint32_t numOrders, double basePrice, uint32_t spikeAt) {
    std::vector<Order> orders;
    std::normal_distribution<double> normalDist(0, basePrice * 0.02);
    std::normal_distribution<double> spikeDist(0, basePrice * 0.15);
    
    for (uint32_t i = 0; i < numOrders; ++i) {
        double price;
        
        if (i >= spikeAt && i < spikeAt + 500) {
            // Spike period
            price = basePrice + spikeDist(rng);
        } else {
            // Normal period
            price = basePrice + normalDist(rng);
        }
        
        price = std::max(price, 0.01);
        orders.push_back(generateSingleOrder(price));
    }
    
    return orders;
}

std::vector<Order> MarketDataGenerator::generateTrendingMarket(uint32_t numOrders, double basePrice, double trendDirection) {
    std::vector<Order> orders;
    std::normal_distribution<double> noiseDist(0, basePrice * 0.01);
    
    double currentPrice = basePrice;
    double trendPerOrder = (basePrice * trendDirection) / numOrders;
    
    for (uint32_t i = 0; i < numOrders; ++i) {
        currentPrice += trendPerOrder + noiseDist(rng);
        currentPrice = std::max(currentPrice, 0.01);
        orders.push_back(generateSingleOrder(currentPrice));
    }
    
    return orders;
}

std::vector<Order> MarketDataGenerator::generateHighFrequency(uint32_t numOrders, double basePrice) {
    std::vector<Order> orders;
    std::uniform_real_distribution<> priceDist(basePrice - 1, basePrice + 1);
    
    // Many small orders with tight spreads
    for (uint32_t i = 0; i < numOrders; ++i) {
        double price = priceDist(rng);
        Order o = generateSingleOrder(price);
        o.quantity = 10 + (i % 50);  // Smaller quantities
        orders.push_back(o);
    }
    
    return orders;
}

std::vector<Order> MarketDataGenerator::generateRandomWalk(uint32_t numOrders, double basePrice, double stepSize) {
    std::vector<Order> orders;
    std::uniform_real_distribution<double> stepDist(-stepSize, stepSize);
    
    double currentPrice = basePrice;
    
    for (uint32_t i = 0; i < numOrders; ++i) {
        currentPrice += stepDist(rng);
        currentPrice = std::max(currentPrice, 0.01);
        orders.push_back(generateSingleOrder(currentPrice));
    }
    
    return orders;
}

void MarketDataGenerator::saveOrdersToCSV(const std::vector<Order>& orders, const std::string& filename) {
    std::ofstream file(filename);
    file << "id,side,price,quantity\n";
    
    for (const auto& order : orders) {
        file << order.id << ","
             << (order.side == Side::BUY ? "BUY" : "SELL") << ","
             << order.price << ","
             << order.quantity << "\n";
    }
    
    file.close();
    std::cout << "Saved " << orders.size() << " orders to " << filename << std::endl;
}

std::vector<Order> MarketDataGenerator::loadOrdersFromCSV(const std::string& filename) {
    std::vector<Order> orders;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return orders;
    }
    
    std::string line;
    std::getline(file, line);  // Skip header
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        
        uint64_t id;
        std::string sideStr;
        double price;
        uint32_t quantity;
        
        std::getline(ss, token, ',');
        id = std::stoull(token);
        
        std::getline(ss, sideStr, ',');
        std::getline(ss, token, ',');
        price = std::stod(token);
        std::getline(ss, token, ',');
        quantity = std::stoul(token);
        
        Side side = sideStr == "BUY" ? Side::BUY : Side::SELL;
        orders.push_back({id, side, price, quantity});
    }
    
    file.close();
    std::cout << "Loaded " << orders.size() << " orders from " << filename << std::endl;
    
    return orders;
}
