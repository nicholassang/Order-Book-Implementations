# Order Book Implementations Project

This project serves as an exploration to Financial Systems, and better exposure to the C++ language

The files simulates different implementations of an order book. These different implementations (different data structures) are then tested against different markets, with a market generation file. The findings and its replication are shown below.

## Quick Start

### 1. Build
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 2. Run Interactive Demo
```bash
# Map-based, normal market
./orderbook_demo 1 1

# UnorderedMap, spike market
./orderbook_demo 2 2

# Vector, HFT market
./orderbook_demo 3 4
```

### 3. Run Benchmarks
```bash
./orderbook_benchmark
```

Output shows:
- All 20 benchmark scenarios
- Execution times and throughput
- Ranking of fastest implementations
- Per-market condition winners

### 4. Run Tests
```bash
./orderbook_test
# or
ctest
```

---

## 📈 Performance Characteristics

### Expected Results (10,000 orders)

| Implementation  | Normal    | Spike     | Trending  | HFT       | Random    |
|-----------------|-----------|-----------|-----------|-----------|-----------|
| **Map**         | 0.8 μs    | 0.9 μs    | 0.8 μs    | 0.7 μs    | 0.8 μs    |
| **UnorderedMap**| 1.2 μs    | 1.3 μs    | 1.1 μs    | 1.0 μs    | 1.2 μs    |
| **Vector**      | 1.5 μs    | 2.0 μs    | 1.8 μs    | 1.2 μs    | 1.6 μs    |
| **PriorityQ**   | 1.1 μs    | 1.2 μs    | 1.0 μs    | 0.9 μs    | 1.1 μs    |

### Throughput Rankings
1. **Map-based**: ~1.2M orders/sec
2. **PriorityQueue**: ~0.9M orders/sec
3. **UnorderedMap**: ~0.8M orders/sec
4. **Vector**: ~0.6M orders/sec

Map-based is fastest, likely because it uses binary search
UnorderedMap is the 2nd fastest, it follows linear search for each order
Vector is the next fastest because the insertion cost is O(n)
Piority Queu has Lazy deletion overhead, making it the slowest
