#pragma once

#include <fstream>
#include <sstream>

#include "OrderBook.h"

Order parseOrder(const std::string& line) {

    std::stringstream ss(line);

    std::string id, side, price, qty;

    std::getline(ss, id, ',');
    std::getline(ss, side, ',');
    std::getline(ss, price, ',');
    std::getline(ss, qty, ',');

    return {
        std::stoull(id),
        (side == "BUY") ? Side::BUY : Side::SELL,
        std::stod(price),
        std::stoul(qty)
    };
}

std::vector<Order> loadOrders(const std::string& path) {

    std::ifstream file(path);
    std::vector<Order> orders;

    std::string line;

    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        if (!line.empty())
            orders.push_back(parseOrder(line));
    }

    return orders;
}