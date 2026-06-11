#include "TradeStatistics.h"
#include <sstream>
#include <iomanip>

void TradeStatistics::recordTrade(const Trade& trade) {
    trades.push_back(trade);
    stats.totalTradeCount++;
    stats.totalVolumeTraded += trade.quantity;
    stats.totalValueTraded += trade.getTotalValue();
    stats.totalFeesPaid += trade.fee;
}

void TradeStatistics::recordTrades(const std::vector<Trade>& newTrades) {
    for (const auto& trade : newTrades) {
        recordTrade(trade);
    }
}

void TradeStatistics::clear() {
    trades.clear();
    stats = OrderStatistics();
}

std::string TradeStatistics::getSummary() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Trade Statistics:\n"
        << "  Total Trades: " << stats.totalTradeCount << "\n"
        << "  Total Volume: " << stats.totalVolumeTraded << " shares\n"
        << "  Total Value: $" << stats.totalValueTraded << "\n"
        << "  Average Price: $" << stats.averageTradePrice() << "\n"
        << "  Total Fees: $" << stats.totalFeesPaid << "\n";
    return oss.str();
}
