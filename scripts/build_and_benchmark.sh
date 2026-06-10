#!/bin/bash
# Quick Build and Benchmark Script

echo "=== Order Book Build and Benchmark ==="
echo

# Create and enter build directory
mkdir -p build
cd build

# Configure and build
echo "Building project..."
cmake ..
cmake --build . --config Release

echo
echo "Build complete!"
echo

# Run demo
echo "=== Running Interactive Demo ==="
echo "Test 1: Map-based with normal market"
./orderbook_demo 1 1

echo
echo "Test 2: UnorderedMap with spike"
./orderbook_demo 2 2

echo
echo "Test 3: Vector with HFT"
./orderbook_demo 3 4

echo
echo "Test 4: PriorityQueue with trending"
./orderbook_demo 4 3

# Run benchmark
echo
echo "=== Running Full Benchmark Suite ==="
./orderbook_benchmark

echo
echo "All tests completed!"
