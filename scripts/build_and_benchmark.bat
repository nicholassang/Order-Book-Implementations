::Quick Build and Benchmark Script (Windows)
@echo off

echo === Order Book Build and Benchmark ===
echo.

:: Create and enter build directory
if not exist build mkdir build
cd build

:: Configure and build
echo Building project...
cmake .. -G "Ninja"
cmake --build . --config Release

echo.
echo Build complete!
echo.

:: Run demo
echo === Running Interactive Demo ===
echo Test 1: Map-based with normal market
orderbook_demo.exe 1 1

echo.
echo Test 2: UnorderedMap with spike
orderbook_demo.exe 2 2

echo.
echo Test 3: Vector with HFT
orderbook_demo.exe 3 4

echo.
echo Test 4: PriorityQueue with trending
orderbook_demo.exe 4 3

:: Run benchmark
echo.
echo === Running Full Benchmark Suite ===
orderbook_benchmark.exe

echo.
echo All tests completed!
pause
