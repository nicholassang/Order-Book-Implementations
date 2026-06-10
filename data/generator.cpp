#include <fstream>
#include <random>

int main() {

    std::ofstream file("orders_10000.csv");
    file << "id,side,price,qty\n";

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> sideDist(0,1);
    std::uniform_int_distribution<int> priceDist(95,105);
    std::uniform_int_distribution<int> qtyDist(1,100);

    for (int i = 1; i <= 10000; i++) {

        std::string side = sideDist(rng) ? "BUY" : "SELL";
        int price = priceDist(rng);
        int qty = qtyDist(rng);

        file << i << "," << side << "," << price << "," << qty << "\n";
    }

    return 0;
}