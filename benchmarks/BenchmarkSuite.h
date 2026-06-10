#pragma once

#include "Order.h"
#include <vector>
#include <string>
#include <chrono>
#include <map>

struct BenchmarkResult {
    std::string implementationName;
    std::string marketCondition;
    uint32_t numOrders;
    double totalTimeMs;
    double timePerOrderUs;
    size_t finalBidCount;
    size_t finalAskCount;
    double memoryMB;
};

template <typename OrderBookType>
class BenchmarkSuite {
public:
    BenchmarkResult runBenchmark(
        const std::string& implName,
        const std::string& marketCondition,
        const std::vector<Order>& orders
    );
    
    static void printResult(const BenchmarkResult& result);
    static void printComparison(const std::vector<BenchmarkResult>& results);
    
private:
    static double getMemoryUsageMB();
};

template <typename OrderBookType>
inline BenchmarkResult BenchmarkSuite<OrderBookType>::runBenchmark(
    const std::string& implName,
    const std::string& marketCondition,
    const std::vector<Order>& orders
) {
    BenchmarkResult result;
    result.implementationName = implName;
    result.marketCondition = marketCondition;
    result.numOrders = orders.size();
    
    double memBefore = getMemoryUsageMB();
    
    OrderBookType book;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    for (const auto& order : orders) {
        book.addOrder(order);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    
    double memAfter = getMemoryUsageMB();
    result.memoryMB = memAfter - memBefore;
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    result.totalTimeMs = duration.count() / 1000.0;
    result.timePerOrderUs = duration.count() / static_cast<double>(orders.size());
    
    result.finalBidCount = book.getBidCount();
    result.finalAskCount = book.getAskCount();
    
    return result;
}

template <typename OrderBookType>
inline double BenchmarkSuite<OrderBookType>::getMemoryUsageMB() {
    // Simplified memory calculation
    // In production, use platform-specific APIs
    return 0.0;
}

template <typename OrderBookType>
inline void BenchmarkSuite<OrderBookType>::printResult(const BenchmarkResult& result) {
    printf("%-15s %-15s | %8d orders | %10.2f ms | %8.2f us/order | Bids: %6zu | Asks: %6zu\n",
           result.implementationName.c_str(),
           result.marketCondition.c_str(),
           result.numOrders,
           result.totalTimeMs,
           result.timePerOrderUs,
           result.finalBidCount,
           result.finalAskCount);
}

template <typename OrderBookType>
inline void BenchmarkSuite<OrderBookType>::printComparison(const std::vector<BenchmarkResult>& results) {
    printf("\n");
    printf("%-15s %-15s | %8s | %10s | %15s | %6s | %6s\n",
           "Implementation", "Market", "Orders", "Time (ms)", "us/Order", "Bids", "Asks");
    printf("%-15s %-15s | %8s | %10s | %15s | %6s | %6s\n",
           "---------------", "----------", "--------", "----------", "---------------", "------", "------");
    
    for (const auto& result : results) {
        printResult(result);
    }
}
