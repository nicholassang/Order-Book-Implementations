#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include "OrderBook.h"

inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline void printStatus(const std::string& msg) {
    std::cout << "[STATUS] " << msg << "\n";
}


inline void loadingScreen() {
    std::string dots = "";
    for (int i = 0; i < 5; i++) {
        clearScreen();
        std::cout << "Loading " << dots << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        dots += ".";
    }
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
} 

// ---- PRINT BOOK ----
inline void printBook(const OrderBook& book) {

    std::cout << "\n\n================ ORDER BOOK ================\n";

    std::cout << "\nBIDS\nPrice\tQty\n";
    for (const auto& [price, orders] : book.getBids()) {
        uint32_t qty = 0;
        for (const auto& o : orders) qty += o.quantity;
        std::cout << price << "\t" << qty << "\n";
    }

    std::cout << "\nASKS\nPrice\tQty\n";
    for (const auto& [price, orders] : book.getAsks()) {
        uint32_t qty = 0;
        for (const auto& o : orders) qty += o.quantity;
        std::cout << price << "\t" << qty << "\n";
    }

    if (!book.getBids().empty() && !book.getAsks().empty()) {
        double bid = book.getBids().begin()->first;
        double ask = book.getAsks().begin()->first;

        std::cout << "\nBest Bid: " << bid;
        std::cout << "\nBest Ask: " << ask;
        std::cout << "\nSpread: " << (ask - bid);
    }

    std::cout << "\n===========================================\n";
    std::cout.flush();
}