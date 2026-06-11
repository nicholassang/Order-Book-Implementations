#pragma once

#include <cstdint>
#include <string>
#include <chrono>

// ============ ENUMERATIONS ============

enum class OrderState {
    PENDING,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED,
    REJECTED
};

enum class OrderType {
    MARKET,
    LIMIT,
    STOP,
    STOP_LIMIT
};

enum class TimeInForce {
    GTC,  // Good Till Cancelled
    IOC,  // Immediate Or Cancel
    FOK,  // Fill Or Kill
    DAY   // Day order
};

enum class OrderStatus : uint8_t {
    RECEIVED,
    ACKNOWLEDGED,
    ACTIVE,
    PARTIALLY_FILLED,
    FULLY_FILLED,
    CANCELLED,
    REJECTED,
    EXPIRED
};

// ============ STRUCTURES ============

struct OrderRequest {
    uint64_t clientId;
    uint64_t orderId;
    const char* symbol;
    Side side;
    OrderType type;
    TimeInForce tif;
    double price;
    uint32_t quantity;
    uint32_t minQty;          // Minimum fill quantity (0 = no minimum)
    double maxPrice;          // Max price for buys, min price for sells
    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
};

struct ExecutionReport {
    uint64_t orderId;
    OrderStatus status;
    uint32_t filledQty;
    uint32_t remainingQty;
    double executionPrice;
    double avgPrice;
    std::string rejectReason;
    std::chrono::time_point<std::chrono::high_resolution_clock> executionTime;
};

struct OrderStatistics {
    uint64_t totalOrders;
    uint64_t totalFills;
    uint64_t totalRejections;
    double totalVolume;
    double totalValue;
    double bestBid;
    double bestAsk;
    double spread;
    double spreadBps;  // Basis points
    uint32_t activeOrders;
};

// ============ ERROR HANDLING ============

enum class ErrorCode {
    SUCCESS = 0,
    INVALID_ORDER_ID,
    INVALID_QUANTITY,
    INVALID_PRICE,
    INSUFFICIENT_MARGIN,
    POSITION_LIMIT_EXCEEDED,
    ORDER_NOT_FOUND,
    ORDER_ALREADY_FILLED,
    MARKET_CLOSED,
    INSTRUMENT_NOT_FOUND,
    INTERNAL_ERROR
};

struct Result {
    ErrorCode code;
    std::string message;
    
    Result(ErrorCode c = ErrorCode::SUCCESS, const std::string& msg = "")
        : code(c), message(msg) {}
    
    bool isSuccess() const { return code == ErrorCode::SUCCESS; }
    bool isError() const { return code != ErrorCode::SUCCESS; }
};
