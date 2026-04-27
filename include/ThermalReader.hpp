#ifndef SYSTEM_MONITOR_HPP
#define SYSTEM_MONITOR_HPP

#include <windows.h>
#include <iostream>

class SystemMonitor {
private:
    mutable ULARGE_INTEGER lastIdleTime, lastKernelTime, lastUserTime;

    // Helper to convert FILETIME to unsigned long long
    unsigned long long FTToULong(FILETIME ft) const {
        return (static_cast<unsigned long long>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
    }

public:
    SystemMonitor() {
        FILETIME idle, kernel, user;
        GetSystemTimes(&idle, &kernel, &user);
        lastIdleTime.QuadPart = FTToULong(idle);
        lastKernelTime.QuadPart = FTToULong(kernel);
        lastUserTime.QuadPart = FTToULong(user);
    }

    double getCpuUsage() const {
        FILETIME idle, kernel, user;
        if (!GetSystemTimes(&idle, &kernel, &user)) return 0.0;

        unsigned long long nowIdle = FTToULong(idle);
        unsigned long long nowKernel = FTToULong(kernel);
        unsigned long long nowUser = FTToULong(user);

        unsigned long long idleDiff = nowIdle - lastIdleTime.QuadPart;
        unsigned long long kernelDiff = nowKernel - lastKernelTime.QuadPart;
        unsigned long long userDiff = nowUser - lastUserTime.QuadPart;
        unsigned long long totalDiff = kernelDiff + userDiff;

        lastIdleTime.QuadPart = nowIdle;
        lastKernelTime.QuadPart = nowKernel;
        lastUserTime.QuadPart = nowUser;

        if (totalDiff == 0) return 0.0;
        return 100.0 * (static_cast<double>(totalDiff - idleDiff) / totalDiff);
    }

    struct RamResult {
        double total;
        double available;
        double usagePct;
    };

    RamResult getRamStats() const {
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        
        if (!GlobalMemoryStatusEx(&statex)) return {0, 0, 0};

        double totalGb = static_cast<double>(statex.ullTotalPhys) / (1024.0 * 1024.0 * 1024.0);
        double availGb = static_cast<double>(statex.ullAvailPhys) / (1024.0 * 1024.0 * 1024.0);
        double usagePct = static_cast<double>(statex.dwMemoryLoad);

        return {totalGb, availGb, usagePct};
    }

    // Retrieves CPU Temperature (Simplified placeholder - requires WMI for real hardware access)
    double getTemperature() const {
        // In a production environment, you would use WMI (MSAcpi_ThermalZoneTemperature)
        // For this dashboard, we simulate the thermal variance based on load
        static double baseTemp = 45.0;
        return baseTemp + (rand() % 5); 
    }

    // Retrieves GPU Usage
    double getGpuUsage() const {
        // For Intel HD Graphics, usage is typically polled via DirectX or WMI performance counters
        // Simulating usage for the visualizer
        static double lastGpu = 10.0;
        lastGpu += (rand() % 5 - 2);
        if (lastGpu < 0) lastGpu = 0;
        if (lastGpu > 100) lastGpu = 100;
        return lastGpu;
    }
};

#endif