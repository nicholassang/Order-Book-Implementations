#include "OrderBook_MapBased.h"
#include "OrderBook_UnorderedMap.h"
#include "OrderBook_Vector.h"
#include "OrderBook_PriorityQueue.h"

#include <cassert>
#include <iostream>

// Helper function to test an order book implementation
template <typename OrderBookType>
void testOrderBook(const std::string& implName) {
    std::cout << "Testing " << implName << "..." << std::endl;
    
    OrderBookType book;
    
    // Create test orders
    Order buy1{1, Side::BUY, 99.5, 100};
    Order buy2{2, Side::BUY, 99.0, 50};
    Order sell1{3, Side::SELL, 100.5, 75};
    Order sell2{4, Side::SELL, 101.0, 25};
    
    // Add orders
    book.addOrder(buy1);
    book.addOrder(buy2);
    book.addOrder(sell1);
    book.addOrder(sell2);
    
    // Check state
    size_t bidCount = book.getBidCount();
    size_t askCount = book.getAskCount();
    
    std::cout << "  Bid count: " << bidCount << std::endl;
    std::cout << "  Ask count: " << askCount << std::endl;
    
    assert(bidCount == 2 && "Bid count should be 2");
    assert(askCount == 2 && "Ask count should be 2");
    
    // Cancel an order
    bool cancelled = book.cancelOrder(1);
    assert(cancelled && "Order 1 should be cancelled");
    
    bidCount = book.getBidCount();
    std::cout << "  Bid count after cancel: " << bidCount << std::endl;
    assert(bidCount == 1 && "Bid count should be 1 after cancel");
    
    std::cout << "  Status: PASSED" << std::endl << std::endl;
}

// Test matching logic
template <typename OrderBookType>
void testMatching(const std::string& implName) {
    std::cout << "Testing matching for " << implName << "..." << std::endl;
    
    OrderBookType book;
    
    // Place a buy order
    Order buy{1, Side::BUY, 100.0, 100};
    book.addOrder(buy);
    
    // Place a matching sell order
    Order sell{2, Side::SELL, 99.0, 50};
    book.addOrder(sell);
    
    // After matching, buy should have 50 remaining, sell should be fully matched
    size_t bidCount = book.getBidCount();
    size_t askCount = book.getAskCount();
    
    std::cout << "  After partial match - Bids: " << bidCount << ", Asks: " << askCount << std::endl;
    assert(bidCount == 1 && "Should have 1 bid after partial match");
    assert(askCount == 0 && "Should have 0 asks after full match");
    
    std::cout << "  Status: PASSED" << std::endl << std::endl;
}

int main() {
    std::cout << "\n=== Order Book Unit Tests ===" << std::endl << std::endl;
    
    try {
        // Test each implementation
        testOrderBook<OrderBook_MapBased>("OrderBook_MapBased");
        testOrderBook<OrderBook_UnorderedMap>("OrderBook_UnorderedMap");
        testOrderBook<OrderBook_Vector>("OrderBook_Vector");
        testOrderBook<OrderBook_PriorityQueue>("OrderBook_PriorityQueue");
        
        // Test matching logic
        testMatching<OrderBook_MapBased>("OrderBook_MapBased");
        testMatching<OrderBook_UnorderedMap>("OrderBook_UnorderedMap");
        testMatching<OrderBook_Vector>("OrderBook_Vector");
        testMatching<OrderBook_PriorityQueue>("OrderBook_PriorityQueue");
        
        std::cout << "=== All Tests Passed! ===" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
