#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <iostream>

class Visualizer {
private:
    std::vector<double> cpuHistory;
    std::vector<double> gpuHistory;
    const size_t maxHistory = 30;

    // ANSI Color thresholds per requirement
    std::string getColor(double val) const {
        if (val < 50.0) return "\033[92m"; // Bright Green
        if (val <= 80.0) return "\033[93m"; // Bright Yellow
        return "\033[91m";                 // Bright Red
    }

    std::string reset() const { return "\033[0m"; }
    std::string bold() const { return "\033[1m"; }

public:
    void updateHistory(double cpuUsage, double gpuUsage) {
        if (cpuHistory.size() < maxHistory) {
            cpuHistory.push_back(cpuUsage);
            gpuHistory.push_back(gpuUsage);
        } else {
            // Shift left and add new data to create scrolling effect
            std::rotate(cpuHistory.begin(), cpuHistory.begin() + 1, cpuHistory.end());
            cpuHistory.back() = cpuUsage;

            std::rotate(gpuHistory.begin(), gpuHistory.begin() + 1, gpuHistory.end());
            gpuHistory.back() = gpuUsage;
        }
    }

    void render(double cpu, double gpu, double temp, double ramUsed, double ramTotal, double ramPct, const std::string& advice) {
        std::ostringstream out;

        // \033[H moves cursor to top-left (1,1) for flicker-free rendering
        out << "\033[H";
        out << bold() << "\033[36m" << "=== AI-BOOSTER PRO CLI (Intel i5-6200U) ===" << reset() << "\033[K\n";
        
        // Professional Status Overview
        out << "System Load: [" << getColor(cpu) << (cpu < 70.0 ? "STABLE" : "STRESSED") << reset() << "]   ";
        out << "Uptime: Active\033[K\n";
        out << "-------------------------------------------\033[K\n";

        // Live CPU History Graph (Sparkline)
        out << "CPU Graph:  [";
        for (double h : cpuHistory) {
            out << getColor(h) << "█" << reset();
        }
        for (size_t i = cpuHistory.size(); i < maxHistory; ++i) out << " ";
        out << "] " << getColor(cpu) << std::fixed << std::setprecision(1) << cpu << "%" << reset() << "\033[K\n\n";

        // Live GPU History Graph
        out << "GPU Graph:  [";
        for (double h : gpuHistory) {
            out << getColor(h) << "█" << reset();
        }
        for (size_t i = gpuHistory.size(); i < maxHistory; ++i) out << " ";
        out << "] " << getColor(gpu) << std::fixed << std::setprecision(1) << gpu << "%" << reset() << "\033[K\n\n";

        out << "Temperature: " << (temp > 75.0 ? "\033[91m" : "\033[92m") << std::fixed << std::setprecision(1) << temp << "°C" << reset() << "\033[K\n";
        out << "Memory    : " << std::fixed << std::setprecision(2) << ramUsed 
            << " / " << ramTotal << " GB (" << (int)ramPct << "%)\033[K\n";
        out << "AI Engine : " << advice << "\033[K\n";
        out << "-------------------------------------------\033[K\n";
        out << "Press [Ctrl+C] to terminate process\033[K\n";

        std::cout << out.str() << std::flush;
    }
};

#endif