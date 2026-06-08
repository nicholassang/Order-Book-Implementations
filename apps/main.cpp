#include "OrderBook.h"
#include <iostream>

int main() {

    OrderBook book;

    book.addOrder({
        1,
        Side::BUY,
        100.0,
        50
    });

    book.addOrder({
        2,
        Side::SELL,
        101.0,
        20
    });

    book.addOrder({
        3,
        Side::SELL,
        99.0,
        25
    });

    book.cancelOrder(2);

    std::cout << "Order book processing completed." << "\n";

    return 0;
}