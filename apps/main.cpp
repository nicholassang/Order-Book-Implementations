#include "cli.h"
#include "OrderLoader.h"

int main() {

    loadingScreen();

    OrderBook book;

    printStatus("Loading 10k dataset...");

    auto orders = loadOrders("../data/orders_10000.csv");

    printStatus("Running simulation...");

    for (auto& o : orders) {

        book.addOrder(o);

        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    printBook(book);

    printStatus("Done");

    return 0;
}