#include "OrderBook.h"
#include "cli.h"

void printBook(const OrderBook& book) {

    std::cout << "\n================ ORDER BOOK ================\n";

    std::cout << "\nBIDS (buy side)\n";
    std::cout << "Price\tQuantity\n";

    for (const auto& [price, orders] : book.getBids()) {
        uint32_t totalQty = 0;

        for (const auto& o : orders)
            totalQty += o.quantity;

        std::cout << price << "\t" << totalQty << "\n";
    }

    std::cout << "\nASKS (sell side)\n";
    std::cout << "Price\tQuantity\n";

    for (const auto& [price, orders] : book.getAsks()) {
        uint32_t totalQty = 0;

        for (const auto& o : orders)
            totalQty += o.quantity;

        std::cout << price << "\t" << totalQty << "\n";
    }

    std::cout << "===========================================\n";
}

int main() {

    loadingScreen();

    OrderBook book;

    printStatus("Starting simulation");

    book.addOrder({1, Side::BUY, 100, 50});
    printBook(book);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    printStatus("Processing order 2");

    book.addOrder({2, Side::SELL, 99, 20});
    printBook(book);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    printStatus("Processing order 3");

    book.addOrder({3, Side::SELL, 101, 10});
    printBook(book);

    printStatus("Done");

    return 0;
}