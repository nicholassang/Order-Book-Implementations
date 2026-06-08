#pragma once
#include <iostream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline void loadingScreen() {
    // Force Windows console to use UTF-8 encoding
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    std::string bars = "";
    for (int i = 0; i < 30; i++) {
        clearScreen();
        std::cout << "[Loading] " << bars << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        bars += "█";
    }
}

inline void printStatus(const std::string& msg) {
    std::cout << "[STATUS] " << msg << "\n";
}