#include "OrderValidator.h"
#include <sstream>

bool OrderValidator::validate(const Order& order, const ValidationRules& rules, std::string& errorMsg) {
    // Check price
    if (!validatePrice(order.price, rules.minPrice, rules.maxPrice, errorMsg)) {
        return false;
    }
    
    // Check quantity
    if (!validateQuantity(order.quantity, rules.minQuantity, rules.maxQuantity, errorMsg)) {
        return false;
    }
    
    // Check order value
    if (!validateOrderValue(order.price, order.quantity, rules.minOrderValue, errorMsg)) {
        return false;
    }
    
    return true;
}

bool OrderValidator::validatePrice(double price, double min, double max, std::string& errorMsg) {
    if (price < min) {
        errorMsg = "Price too low";
        return false;
    }
    if (price > max) {
        errorMsg = "Price too high";
        return false;
    }
    return true;
}

bool OrderValidator::validateQuantity(uint32_t qty, uint32_t minQty, uint32_t maxQty, std::string& errorMsg) {
    if (qty < minQty) {
        errorMsg = "Quantity too low";
        return false;
    }
    if (qty > maxQty) {
        errorMsg = "Quantity too high";
        return false;
    }
    return true;
}

bool OrderValidator::validateOrderValue(double price, uint32_t qty, uint32_t minValue, std::string& errorMsg) {
    double orderValue = price * qty;
    if (orderValue < minValue) {
        errorMsg = "Order value too low";
        return false;
    }
    return true;
}
