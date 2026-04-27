#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include "../include/ThermalReader.hpp"
#include "../include/BoosterEngine.hpp"
#include "../include/Visualizer.hpp"

int main() {
    // Enable ANSI escape sequences on Windows
    SetConsoleOutputCP(CP_UTF8); // Fix the weird "Γ£à" characters
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }

    SystemMonitor monitor;
    BoosterEngine engine;
    Visualizer viz;

    std::cout << "--- AI-BOOSTER DASHBOARD (Intel i5-6200U) ---" << std::endl;
    std::cout << "Press Ctrl+C to stop monitoring" << std::endl;
    // Clear screen once at startup
    std::cout << "\033[2J" << std::flush;

    while (true) {
        double cpu = monitor.getCpuUsage();
        double gpu = monitor.getGpuUsage();
        double temp = monitor.getTemperature();
        auto ram = monitor.getRamStats();
        std::string advice = engine.analyzeSystem(cpu / 50.0, ram.available);
        
        viz.updateHistory(cpu, gpu);
        viz.render(cpu, gpu, temp, ram.total - ram.available, ram.total, ram.usagePct, advice);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    return 0;
}