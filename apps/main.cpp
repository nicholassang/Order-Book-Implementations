#include "cli.h"
#include "OrderLoader.h"
#include "OrderBook_MapBased.h"
#include "OrderBook_UnorderedMap.h"
#include "OrderBook_Vector.h"
#include "OrderBook_PriorityQueue.h"
#include "MarketDataGenerator.h"
#include "OrderBookFactory.h"

#include <iostream>
#include <chrono>

void printUsage() {
    std::cout << "Usage: orderbook_demo [implementation] [market_condition]\n\n";
    std::cout << "Implementations:\n";
    std::cout << "  1 - Map-based (default)\n";
    std::cout << "  2 - UnorderedMap\n";
    std::cout << "  3 - Vector\n";
    std::cout << "  4 - PriorityQueue\n\n";
    std::cout << "Market Conditions:\n";
    std::cout << "  1 - Normal (default)\n";
    std::cout << "  2 - Spike\n";
    std::cout << "  3 - Trending\n";
    std::cout << "  4 - High Frequency Trading\n";
    std::cout << "  5 - Random Walk\n";
    std::cout << "  6 - Loaded from CSV\n";
}

int main(int argc, char* argv[]) {
    loadingScreen();
    
    int implChoice = 1;
    int marketChoice = 1;
    
    // Parse command line arguments
    if (argc > 1) {
        implChoice = std::stoi(argv[1]);
    }
    if (argc > 2) {
        marketChoice = std::stoi(argv[2]);
    }
    
    if (implChoice < 1 || implChoice > 4 || marketChoice < 1 || marketChoice > 6) {
        printStatus("Invalid choice. Using default (Map-based, Normal market)");
        implChoice = 1;
        marketChoice = 1;
    }
    
    // Generate market data
    printStatus("Generating market data...");
    
    MarketDataGenerator generator(42);
    std::vector<Order> orders;
    std::string marketName;
    const uint32_t numOrders = 10000;
    const double basePrice = 100.0;
    
    switch (marketChoice) {
        case 2:
            orders = generator.generateMarketSpike(numOrders, basePrice, numOrders / 2);
            marketName = "Spike";
            break;
        case 3:
            orders = generator.generateTrendingMarket(numOrders, basePrice, 0.05);
            marketName = "Trending";
            break;
        case 4:
            orders = generator.generateHighFrequency(numOrders, basePrice);
            marketName = "HFT";
            break;
        case 5:
            orders = generator.generateRandomWalk(numOrders, basePrice, 0.5);
            marketName = "RandomWalk";
            break;
        case 6:
            orders = generator.loadOrdersFromCSV("../data/orders_10000.csv");
            marketName = "Loaded CSV";
            break;
        default:
            orders = generator.generateNormalMarket(numOrders, basePrice, basePrice * 0.02);
            marketName = "Normal";
    }
    
    printf("Loaded %zu orders (%s market condition)\n", orders.size(), marketName.c_str());
    
    printStatus("Running simulation...");
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Execute based on selected implementation
    switch (implChoice) {
        case 2: {
            printStatus("Using UnorderedMap implementation");
            OrderBook_UnorderedMap book;
            for (auto& o : orders) {
                book.addOrder(o);
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            printf("Completed in %lld ms\n", duration.count());
            printf("Final state - Bids: %zu | Asks: %zu\n", book.getBidCount(), book.getAskCount());
            break;
        }
        case 3: {
            printStatus("Using Vector implementation");
            OrderBook_Vector book;
            for (auto& o : orders) {
                book.addOrder(o);
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            printf("Completed in %lld ms\n", duration.count());
            printf("Final state - Bids: %zu | Asks: %zu\n", book.getBidCount(), book.getAskCount());
            break;
        }
        case 4: {
            printStatus("Using PriorityQueue implementation");
            OrderBook_PriorityQueue book;
            for (auto& o : orders) {
                book.addOrder(o);
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            printf("Completed in %lld ms\n", duration.count());
            printf("Final state - Bids: %zu | Asks: %zu\n", book.getBidCount(), book.getAskCount());
            break;
        }
        default: {
            printStatus("Using Map-based implementation");
            OrderBook_MapBased book;
            for (auto& o : orders) {
                book.addOrder(o);
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            printf("Completed in %lld ms\n", duration.count());
            printf("Final state - Bids: %zu | Asks: %zu\n", book.getBidCount(), book.getAskCount());
        }
    }
    
    printStatus("Done");
    
    return 0;
}