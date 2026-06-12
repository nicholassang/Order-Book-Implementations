#include <benchmark/benchmark.h>

#include "OrderBook_MapBased.h"
#include "OrderBook_UnorderedMap.h"
#include "OrderBook_Vector.h"
#include "OrderBook_PriorityQueue.h"
#include "MarketDataGenerator.h"

static std::vector<Order> generateOrders()
{
    MarketDataGenerator gen(42);
    return gen.generateNormalMarket(10000, 100.0, 2.0);
}

// =========================
// MAP BASED
// =========================
static void BM_MapBased_Insert(benchmark::State& state)
{
    auto orders = generateOrders();

    for (auto _ : state)
    {
        OrderBook_MapBased book;

        for (const auto& o : orders)
        {
            book.addOrder(o);
        }

        benchmark::DoNotOptimize(book);
    }

    state.SetItemsProcessed(state.iterations() * orders.size());
}

BENCHMARK(BM_MapBased_Insert);

// =========================
// UNORDERED MAP
// =========================
static void BM_UnorderedMap_Insert(benchmark::State& state)
{
    auto orders = generateOrders();

    for (auto _ : state)
    {
        OrderBook_UnorderedMap book;

        for (const auto& o : orders)
        {
            book.addOrder(o);
        }

        benchmark::DoNotOptimize(book);
    }

    state.SetItemsProcessed(state.iterations() * orders.size());
}

BENCHMARK(BM_UnorderedMap_Insert);

// =========================
// VECTOR
// =========================
static void BM_Vector_Insert(benchmark::State& state)
{
    auto orders = generateOrders();

    for (auto _ : state)
    {
        OrderBook_Vector book;

        for (const auto& o : orders)
        {
            book.addOrder(o);
        }

        benchmark::DoNotOptimize(book);
    }

    state.SetItemsProcessed(state.iterations() * orders.size());
}

BENCHMARK(BM_Vector_Insert);

// =========================
// PRIORITY QUEUE
// =========================
static void BM_PriorityQueue_Insert(benchmark::State& state)
{
    auto orders = generateOrders();

    for (auto _ : state)
    {
        OrderBook_PriorityQueue book;

        for (const auto& o : orders)
        {
            book.addOrder(o);
        }

        benchmark::DoNotOptimize(book);
    }

    state.SetItemsProcessed(state.iterations() * orders.size());
}

BENCHMARK(BM_PriorityQueue_Insert);

BENCHMARK_MAIN();