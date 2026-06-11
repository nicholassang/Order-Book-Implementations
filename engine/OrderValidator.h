#pragma once

#include "Order.h"
#include <vector>
#include <string>

class OrderValidator {
public:
    // Validation rules
    struct ValidationRules {
        double minPrice = 0.01;
        double maxPrice = 100000.0;
        uint32_t minQuantity = 1;
        uint32_t maxQuantity = 1000000;
        uint32_t minOrderValue = 1;  // $1 minimum
    };
    
    static bool validate(const Order& order, const ValidationRules& rules, std::string& errorMsg);
    static bool validatePrice(double price, double min, double max, std::string& errorMsg);
    static bool validateQuantity(uint32_t qty, uint32_t minQty, uint32_t maxQty, std::string& errorMsg);
    static bool validateOrderValue(double price, uint32_t qty, uint32_t minValue, std::string& errorMsg);
};
