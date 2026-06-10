#include "BenchmarkSuite.h"
#include "OrderBook_MapBased.h"
#include "OrderBook_UnorderedMap.h"
#include "OrderBook_Vector.h"
#include "OrderBook_PriorityQueue.h"
#include "MarketDataGenerator.h"

#include <iostream>
#include <iomanip>

int main() {
    std::cout << "=== Order Book Benchmark Suite ===" << std::endl << std::endl;
    
    MarketDataGenerator generator(42);
    
    // Test parameters
    const uint32_t numOrders = 10000;
    const double basePrice = 100.0;
    
    std::cout << "Generating market data for " << numOrders << " orders..." << std::endl;
    
    // Generate different market conditions
    auto normalMarket = generator.generateNormalMarket(numOrders, basePrice, basePrice * 0.02);
    auto spikeMarket = generator.generateMarketSpike(numOrders, basePrice, numOrders / 2);
    auto trendingMarket = generator.generateTrendingMarket(numOrders, basePrice, 0.05);
    auto hftMarket = generator.generateHighFrequency(numOrders, basePrice);
    auto randomWalk = generator.generateRandomWalk(numOrders, basePrice, 0.5);
    
    std::cout << "Market data generated. Running benchmarks...\n" << std::endl;
    
    std::vector<BenchmarkResult> allResults;
    
    // Benchmark Map-based implementation
    std::cout << "--- Testing Map-based Implementation ---" << std::endl;
    BenchmarkSuite<OrderBook_MapBased> mapBench;
    allResults.push_back(mapBench.runBenchmark("Map", "Normal", normalMarket));
    allResults.push_back(mapBench.runBenchmark("Map", "Spike", spikeMarket));
    allResults.push_back(mapBench.runBenchmark("Map", "Trending", trendingMarket));
    allResults.push_back(mapBench.runBenchmark("Map", "HFT", hftMarket));
    allResults.push_back(mapBench.runBenchmark("Map", "RandomWalk", randomWalk));
    
    // Benchmark UnorderedMap implementation
    std::cout << "--- Testing UnorderedMap Implementation ---" << std::endl;
    BenchmarkSuite<OrderBook_UnorderedMap> umapBench;
    allResults.push_back(umapBench.runBenchmark("UnorderedMap", "Normal", normalMarket));
    allResults.push_back(umapBench.runBenchmark("UnorderedMap", "Spike", spikeMarket));
    allResults.push_back(umapBench.runBenchmark("UnorderedMap", "Trending", trendingMarket));
    allResults.push_back(umapBench.runBenchmark("UnorderedMap", "HFT", hftMarket));
    allResults.push_back(umapBench.runBenchmark("UnorderedMap", "RandomWalk", randomWalk));
    
    // Benchmark Vector implementation
    std::cout << "--- Testing Vector Implementation ---" << std::endl;
    BenchmarkSuite<OrderBook_Vector> vecBench;
    allResults.push_back(vecBench.runBenchmark("Vector", "Normal", normalMarket));
    allResults.push_back(vecBench.runBenchmark("Vector", "Spike", spikeMarket));
    allResults.push_back(vecBench.runBenchmark("Vector", "Trending", trendingMarket));
    allResults.push_back(vecBench.runBenchmark("Vector", "HFT", hftMarket));
    allResults.push_back(vecBench.runBenchmark("Vector", "RandomWalk", randomWalk));
    
    // Benchmark PriorityQueue implementation
    std::cout << "--- Testing PriorityQueue Implementation ---" << std::endl;
    BenchmarkSuite<OrderBook_PriorityQueue> pqBench;
    allResults.push_back(pqBench.runBenchmark("PriorityQueue", "Normal", normalMarket));
    allResults.push_back(pqBench.runBenchmark("PriorityQueue", "Spike", spikeMarket));
    allResults.push_back(pqBench.runBenchmark("PriorityQueue", "Trending", trendingMarket));
    allResults.push_back(pqBench.runBenchmark("PriorityQueue", "HFT", hftMarket));
    allResults.push_back(pqBench.runBenchmark("PriorityQueue", "RandomWalk", randomWalk));
    
    // Print results
    std::cout << "\n\n=== BENCHMARK RESULTS ===" << std::endl;
    BenchmarkSuite<OrderBook_MapBased>::printComparison(allResults);
    
    // Analysis
    std::cout << "\n\n=== PERFORMANCE ANALYSIS ===" << std::endl;
    
    // Find fastest for each market condition
    std::map<std::string, double> fastestTimes;
    std::map<std::string, std::string> fastestImpl;
    
    for (const auto& result : allResults) {
        if (fastestTimes.find(result.marketCondition) == fastestTimes.end() ||
            result.timePerOrderUs < fastestTimes[result.marketCondition]) {
            fastestTimes[result.marketCondition] = result.timePerOrderUs;
            fastestImpl[result.marketCondition] = result.implementationName;
        }
    }
    
    std::cout << "Fastest implementation by market condition:" << std::endl;
    for (const auto& [market, impl] : fastestImpl) {
        printf("  %-20s : %s (%.2f us/order)\n", 
               market.c_str(), impl.c_str(), fastestTimes[market]);
    }
    
    return 0;
}
